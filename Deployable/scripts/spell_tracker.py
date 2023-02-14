from duck import *

script_info = {
    'author': 'MK',
    'description': 'none',
    'name': 'Spell Tracker'
}

size_img_skill = 24

settings = [
	["Q", [35, 10],   10, 25, True], 
	["W", [60, 10],   10, 25, True], 
	["E", [85, 10],   10, 25, True], 
	["R", [109, 10],  10, 25, True], 
	["D", [149, -15], 10, 25, True], 
	["F", [176, -15], 10, 25, True]
]

def duck_menu(ctx):
    pass

def duck_on_load(ctx):
    pass

def draw_spell(ctx, spell, pos):
    global size_img_skill

    cd = spell.cd
    color = Col.White
    if spell.lvl == 0:
        color = Col.Gray
    if cd > 0.0:
        color = Col.Red

    ctx.image(spell.static.icon if spell.static else 'none', pos, pos + Vec2(size_img_skill, size_img_skill), color, 10)
    if cd > 0.0:
        ctx.text(pos + Vec2(3, 5), str(int(cd)), Col.White)

def draw_tracker_for(ctx, champ):
    global size_img_skill

    pos = champ.hpbar_pos
    for i in range(0, 4):
        pos.x += size_img_skill
        draw_spell(ctx, champ.spells[i], pos)

    pos.y -= 26.0
    pos.x += 15.0
    for i in range(4, 6):
        pos.x += size_img_skill
        draw_spell(ctx, champ.spells[i], pos)

def duck_exec(ctx):
    for champ in ctx.champs:
        draw_tracker_for(ctx, champ)
        
        #https://www.youtube.com/watch?v=PZ7lDrwYdZc