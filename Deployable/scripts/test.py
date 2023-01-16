from duck import *

script_info = {
    'author': 'MK',
    'description': 'none',
    'name': 'Pretty Test'
}

def duck_menu(ctx):
    ctx.log("hellos menu")

def duck_on_load(ctx):
    ctx.log("hello on load")

def duck_exec(ctx):
    return