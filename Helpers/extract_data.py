import sys, os, json
from pprint import pprint

'''
	Generates two JSONs. One with unit data and one with spell data for unit spells. It extracts this data from the unit data downloaded with DownloadUnitData.py.
'''
def extract_unit_info(folder):

    def find_key_ending_with(dictionary, partial_key):
        for key, val in dictionary.items():
            if key.endswith(partial_key):
                return val
        return None
	
    units, spells = {}, {}
    infos = ''
    unit_tags = set()

    for fname in os.listdir(folder):
        print("Processing: "+ fname)
        if fname.startswith(('brush_', 'nexusblitz_', 'slime_', 'tft4_', 'tft_')):
            print('Object blacklisted. Skipping...')
            continue
            
        props = {}
        with open(os.path.join(folder, fname)) as file:
            props = json.loads(file.read())
        
        # Find character property node
        root = find_key_ending_with(props, '/Root')	
        if not root:
            print('[Fail] No root found for: ' + fname)
            continue
        
        # Get character name
        name = root.get('mCharacterName', '')
        if len(name) == 0:
            print('[Fail] No character name found for: ' + fname)
            continue
            
        # Get basic attack info
        ba_windup    = 0.0
        ba_cast_time = 0.0
        ba_name      = "?"
                    
        if 'basicAttack' in root:
            basic_attack = root['basicAttack']
            ba_name      = basic_attack.get("mAttackName", name + "basicattack")
            ba_cast_time = basic_attack.get("mAttackCastTime", 0.0)
            if ba_cast_time == 0:
                ba_cast_time  = 0.5 + basic_attack.get("mAttackDelayCastOffsetPercent", 0.0) * 0.5
                
            if 'mAttackTotalTime' in basic_attack and 'mAttackCastTime' in basic_attack:
                ba_windup = basic_attack['mAttackCastTime']/basic_attack['mAttackTotalTime']
            else:
                ba_windup = 0.3 + basic_attack.get('mAttackDelayCastOffsetPercent', 0.0)

        tags = set(['Unit_' + x.strip().replace('=', '_') for x in root.get("unitTagsString", "").split('|')])
        unit = {
            "name":                 name.lower(),
            "healthBarHeight":      root.get("healthBarHeight", 100.0),
            "baseMoveSpeed":        root.get("baseMoveSpeed", 0.0),
            "attackRange":          root.get("attackRange", 0.0),
            "attackSpeed":          root.get("attackSpeed", 0.0), 
            "attackSpeedRatio":     root.get("attackSpeedRatio", 0.0), 
            "acquisitionRange":     root.get("acquisitionRange", 0.0),
            "selectionRadius":      root.get("selectionRadius", 0.0),
            "pathingRadius":        root.get("pathfindingCollisionRadius", 0.0),
            "gameplayRadius":       root.get("overrideGameplayCollisionRadius", 65.0),
            "basicAtkWindup":       ba_windup,
            "basicAtkCastTime":     ba_cast_time,
            "basicAtk":             ba_name.lower(),
            "tags":                 list(tags)
        }
        units[unit["name"]] = unit
        
        # Read spells
        for key, val in props.items():
            if "mSpell" not in val:
                continue
            
            s = val["mSpell"]
            if s:
                icon_name = os.path.basename(s.get("mImgIconName", [""])[0]).replace(".dds", "")
                spell = {
                    "name":               os.path.basename(key),
                    "icon":               icon_name,
                    "flags":              s.get("mAffectsTypeFlags", 0),
                    "castTime":           s.get("mCastTime", 0.0),
                    "castRange":          s.get("castRangeDisplayOverride", s.get("castRange", [s.get("castConeDistance", 0.0)]))[0],
                    "castRadius":         s.get("castRadiusSecondary", s.get("castRadius", [0.0]))[0],
                    "width":              s.get("mLineWidth", 0.0),
                    "height":             0.0,
                    "speed":              s.get("missileSpeed", 0.0),
                    "travelTime":         0.0,
                    "projectDestination": False
                }
                
                if spell["castTime"] == 0:
                    if "delayCastOffsetPercent" in s:
                        spell["castTime"] = 0.5 + 0.5 * s.get("delayCastOffsetPercent", 0.5)
                    
                if spell['speed'] == 0.0:
                    spell['speed'] = 10000.0
                
                if 'mCastRangeGrowthMax' in s:
                    spell['castRange'] = s['mCastRangeGrowthMax'][4]
                
                missile = s.get("mMissileSpec", None)
                if missile:
                    movcomp = missile.get("movementComponent", None)
                    if movcomp:
                        spell["speed"]              = movcomp.get("mSpeed", spell["speed"])
                        spell["height"]             = movcomp.get("mOffsetInitialTargetHeight", 100.0)
                        spell["projectDestination"] = movcomp.get("mProjectTargetToCastRange", False)
                        spell["travelTime"]         = movcomp.get("mTravelTime", 0.0)
                        
                spells[spell["name"]] = spell
                
    print(f'Found {len(units)} units and {len(spells)} spells')
    
    with open("UnitData.json", 'w') as f:
        f.write(json.dumps(list(units.values()), indent=4))
        
    with open("SpellData.json", 'w') as f:
        f.write(json.dumps(list(spells.values()), indent=4))
	

def extract_skin_info(folder):
    result = {}
    for file in os.listdir(folder):
        d = None
        with open(os.path.join(folder, file), 'rb') as f:
            d = json.loads(f.read())
        
        skins = []
        for skininfo in d['skins']:
            chromas = []
            if 'chromas' in skininfo:
                for chroma in skininfo['chromas']:
                    chromas.append({'id': chroma['id'] % 100, 'color': int(chroma['colors'][0][1:], 16)})
                    
            skins.append({
                "name": skininfo["name"],
                "id": skininfo['id'] % 100,
                "chromas": chromas
           }) 
        
        result[d['alias'].lower()] = skins
        
    with open('SkinInfo.json', 'w') as f:
        f.write(json.dumps(result, indent=4))
    
mode = 'unitinfo'
if mode == 'skininfo':
    extract_skin_info('champ_skin_infos')
elif mode == 'unitinfo':
    extract_unit_info('champ_unit_infos')