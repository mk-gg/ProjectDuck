from duck import *
from helpers.drawings import Circle

script_info = {
    'author': 'Mk',
    'description': 'none',
    'name': 'Indicators',
    'icon': 'menu-script'
}

player_circle = Circle(0.0, 30, 1.0, Col.Green, False, True)

def duck_menu(ctx):
    global player_circle
    ui = ctx.ui
    
    player_circle.ui("Attack range circle settings", ctx)
    
def duck_on_load(ctx):
    global player_circle
    cfg = ctx.cfg
    
    player_circle = Circle.from_str(cfg.get_str("player_circle", str(player_circle)))                                                                                                                                                   
    
def duck_on_save(ctx):
    cfg = ctx.cfg
    
    cfg.set_str("player_circle", str(player_circle))
    
def duck_exec(ctx):
    
    player_circle.radius = ctx.player.atk_range + ctx.player.static.gameplay_radius
    player_circle.draw_at(ctx, ctx.player.pos)