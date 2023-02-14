from duck import *
from helpers.drawings import Circle

script_info = {
    'author': 'Mk',
    'description': 'none',
    'name': 'Indicators'
}


player_circle = Circle(190.0, 100, 2.5, Col.Green, False, True)

def duck_menu(ctx):
    global player_circle
    ui = ctx.ui

    player_circle.ui("Attack range circle settings", ctx)

def duck_on_load(ctx):
    global player_circle

    #player_circle = [190.0, 100, 2.5, [0.4274509847164154, 0.8274509906768799, 0.125490203499794, 1.0], False, True]
    player_circle = Circle.from_str(str(player_circle))                                                                                                                                                



def duck_exec(ctx):

    player_circle.radius = ctx.player.atk_range  +  55
    player_circle.draw_at(ctx, ctx.player.pos) 