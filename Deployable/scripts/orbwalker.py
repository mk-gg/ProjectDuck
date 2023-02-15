from duck import *
from helpers.targeting import TargetSelector
from helpers.prediction import *
from time import time
from enum import Enum

script_info = {
    'author': 'Mk',
    'description': 'none',
    'name': 'Orbwalker',
    'icon': 'menu-script'
}

target_selector = TargetSelector()
max_atk_speed   = 2.2
key_kite        = 0
key_last_hit    = 0
key_lane_push   = 0
move_interval   = 0.10

class OrbwalkKite:
    def get_target(self, ctx):
        return target_selector.get_target(ctx.player, ctx.champs, ctx.player.atk_range + ctx.player.static.gameplay_radius)

class OrbwalkLastHit:
    def get_target(self, ctx):
        minions  = [m for m in ctx.minions if ctx.is_on_screen(m.pos)]
        lasthits = predict_minions_lasthit(ctx, minions)
        if len(lasthits) == 0:
            return None
            
        lasthits = sorted(lasthits, key = lambda p: p[0].health - p[1], reverse = True)
        for minion, predicted_hp, player_dmg in lasthits:
            if predicted_hp - player_dmg <= 0.0:
                return minion
            
        return None

class OrbwalkLanePush:
    def get_target(self, ctx):
        player              = ctx.player
        
        # Try getting jungle mob
        jungle_target = target_selector.get_target(player, ctx.jungle, player.atk_range + player.static.gameplay_radius)
        if jungle_target:
            return jungle_target
            
        # Try getting the last minion
        minions  = [m for m in ctx.minions if ctx.is_on_screen(m.pos)]
        lasthits = predict_minions_lasthit(ctx, minions)
        if len(lasthits) == 0:
            return None
            
        lasthits = sorted(lasthits, key = lambda p: p[0].health - p[1], reverse = True)
        for minion, predicted_hp, player_dmg in lasthits:
            if predicted_hp - player_dmg <= 0.0:
                return minion
        
        # No last hit, we try to push or wait for last hit
        basic_atk_speed     = player.static.basic_atk.speed
        basic_atk_delay     = player.static.basic_atk_windup / player.static.base_atk_speed
    
        for minion, predicted_hp, player_dmg in lasthits:
            predicted_dmg = minion.health - predicted_hp
            #t_until_player_hits = basic_atk_delay + ctx.ping / 2000.0 + player.pos.distance(minion.pos) / basic_atk_speed
            #double_prediction = predict_minion_health(ctx, minion, minions, t_until_player_hits*2.0)
            
            # Wait for last hit, this method is heuristic definitely not perfect
            if predicted_hp - player_dmg < player_dmg + predicted_dmg:
                return None
            
            if predicted_hp - player_dmg > predicted_dmg:
                return minion
            
        return None
        
kite_mode       = OrbwalkKite()
last_hit_mode   = OrbwalkLastHit()
lane_push_mode  = OrbwalkLanePush()

def duck_menu(ctx):
    global target_selector, max_atk_speed, move_interval
    global key_kite, key_last_hit, key_lane_push
    ui = ctx.ui
    
    target_selector.ui(ctx)
    max_atk_speed  = ui.sliderfloat("Attack speed cap", max_atk_speed, 1.0, 5.0)
    move_interval  = ui.sliderfloat("Move command interval (ms)", move_interval, 0.05, 0.20)
    key_kite       = ui.keyselect("Key kite champions", key_kite)
    key_last_hit   = ui.keyselect("Key last hit minions (No Turret Farming Yet)", key_last_hit)
    key_lane_push  = ui.keyselect("Key lane push", key_lane_push)

def duck_on_load(ctx):
    global target_selector, max_atk_speed, move_interval
    global key_kite, key_last_hit, key_lane_push
    cfg = ctx.cfg
    
    target_selector = TargetSelector.from_str(cfg.get_str("target", str(target_selector)))
    max_atk_speed   = cfg.get_float("max_atk_speed", max_atk_speed)
    move_interval   = cfg.get_float("move_interval", move_interval)
    key_kite        = cfg.get_int("key_kite", key_kite)
    key_last_hit    = cfg.get_int("key_last_hit", key_last_hit)
    key_lane_push   = cfg.get_int("key_lane_push", key_lane_push)
    
def duck_on_save(ctx):
    cfg = ctx.cfg
    
    cfg.set_str("target", str(target_selector))
    cfg.set_float("max_atk_speed", max_atk_speed)
    cfg.set_float("move_interval", move_interval)
    cfg.set_int("key_kite", key_kite)
    cfg.set_int("key_last_hit", key_last_hit)
    cfg.set_int("key_lane_push", key_lane_push)

last_moved    = 0
last_attacked = 0



def duck_exec(ctx):
    global last_moved, last_attacked
    
    mode = None
    if ctx.is_held(key_kite):
        mode = kite_mode
    elif ctx.is_held(key_last_hit):
        mode = last_hit_mode
    elif ctx.is_held(key_lane_push):
        mode = lane_push_mode
    else:
        return
    
    player        = ctx.player   
    atk_speed     = player.atk_speed
    b_windup_time = player.static.basic_atk_windup/player.static.base_atk_speed#player.static.basic_atk_windup/(atk_speed if atk_speed >= 0.6 else player.static.base_atk_speed)                                                                    
    c_atk_time    = 1.0/atk_speed
    max_atk_time  = 1.0/max_atk_speed
    
    target = None
    now = time()
    dt = now - last_attacked - ctx.ping / 2000.0
    if dt > max(c_atk_time, max_atk_time):
        target = mode.get_target(ctx)
        if target:
            ctx.attack(target)
            last_attacked = now
            
    if not target and dt > b_windup_time and now - last_moved > move_interval:
        ctx.move()
        last_moved = now