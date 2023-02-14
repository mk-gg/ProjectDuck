from duck import *
from helpers.drawings import Circle
import json

script_info = {
    'author': 'MK',
    'description': 'none',
    'name': 'Indicators'
}

player_circle = Circle(0.0, 30, 1.0, Col.Green, False, True)

      
def duck_menu(ctx):
    global player_circle
    ui = ctx.ui

    player_circle.ui("Player attack range", ctx)

def duck_on_load(ctx):
    global player_circle
    cfg = ctx.cfg

    player_circle = Circle.from_serializable(cfg.get_str("player_circle", player_circle.to_serializable()))  

def duck_on_save(ctx):
    cfg = ctx.cfg

    cfg.set_str("player_circle", player_circle.to_serializable())

def duck_exec(ctx):
    player_circle.radius = ctx.player.atk_range
    player_circle.draw_at(ctx, ctx.player.pos) 