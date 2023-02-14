from duck import *
from helpers.targeting import TargetSelector
from time import time
from enum import Enum

script_info = {
    'author': 'MK',
    'description': 'none',
    'name': 'Orbwalker'
}

target_selector = TargetSelector()
max_atk_speed   = 2.2
key_kite        = 57
key_last_hit    = 0
key_lane_push   = 0
ping            = 23


class OrbwalkKite:
    def get_target(self, ctx):
        player              = ctx.player
        basic_atk_speed     = player.static.basic_atk.speed
        basic_atk_cast_time = player.static.basic_atk_cast_time

        best_target        = None
        min_score          = 1000000

        minions = [m for m in ctx.minions if ctx.is_on_screen(m.pos)]
        for enemy_minion in minions:
            if enemy_minion.dead or enemy_minion.ally_to(player):
                continue

            hit_dmg             = items.get_onhit_physical(player, enemy_minion) + items.get_onhit_magical(player, enemy_minion)
            t_until_player_hits = 0.3087 + player.pos.distance(enemy_minion.pos) / basic_atk_speed  #0.3087
            enemy_minion_hp     = predict_minion_health(ctx, enemy_minion, minions, t_until_player_hits)

            if enemy_minion_hp > 0.0 and enemy_minion_hp - hit_dmg <= 0.0:
                if enemy_minion_hp < min_score and enemy_minion.pos.distance(player.pos) < player.atk_range:
                    min_score = enemy_minion_hp
                    best_target = enemy_minion

        return best_target

class OrbwalkLastHit:
    def get_target(self, ctx):
        return None

class OrbwalkLanePush:
    def get_target(self, ctx):
        return None

kite_mode       = OrbwalkKite()
last_hit_mode   = OrbwalkLastHit()
lane_push_mode  = OrbwalkLanePush()




def duck_menu(ctx):
    global target_selector, max_atk_speed, ping
    global key_kite, key_last_hit, key_lane_push
    ui = ctx.ui

    target_selector.ui(ctx)
    max_atk_speed  = ui.sliderfloat("Attack speed cap", max_atk_speed, 1.0, 3.0)
    ping           = ui.sliderfloat("Your average ping", ping, 1.0, 200.0)
    key_kite       = ui.keyselect("Key kite champions", key_kite)
    key_last_hit   = ui.keyselect("Key last hit minions", key_last_hit)
    key_lane_push  = ui.keyselect("Key lane push", key_lane_push)

def duck_on_load(ctx):
    global target_selector, max_atk_speed, ping
    global key_kite, key_last_hit, key_lane_push

    target_selector = TargetSelector.from_str(str(target_selector))
    max_atk_speed   = max_atk_speed
    ping            = ping
    key_kite        = key_kite
    key_last_hit    = key_last_hit
    key_lane_push   = key_lane_push



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
    b_windup_time = player.static.basic_atk_windup/player.static.base_atk_speed
    c_atk_time    = 1.0/atk_speed
    max_atk_time  = 1.0/max_atk_speed

    target = None
    now = time()
    dt = now - last_attacked - ping / 2000.0
    if dt > max(c_atk_time, max_atk_time):
        target = mode.get_target(ctx)
        if target:
            ctx.attack(target)
            last_attacked = now

    if not target and dt > b_windup_time and now - last_moved > 0.10:
        ctx.move()
        last_moved = now