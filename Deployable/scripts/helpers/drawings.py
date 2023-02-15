from duck import *
import json


class Circle:

    radius  = 0.0
    num_pts = 4
    width   = 1.0
    color   = Col.White
    filled  = False
    enabled = True

    def __init__(self, rad, pts, width, col, fill, enabled):
        self.radius  = rad
        self.num_pts = pts
        self.width   = width
        self.color   = col
        self.filled    = fill
        self.enabled = enabled

    def ui(self, label, ctx, fixed_radius = True):
        ui = ctx.ui

        if ui.treenode(label):
            self.enabled = ui.checkbox("Enabled", self.enabled)
            self.filled  = ui.checkbox("Filled", self.filled)
            if not fixed_radius:
                self.radius = ui.dragfloat("Radius", self.radius)

            self.num_pts = ui.sliderint("Num Points", self.num_pts, 4, 100)
            self.width   = ui.sliderfloat("Width", self.width, 1.0, 20.0)
            self.color   = ui.colorpick("Color", self.color)
            ui.treepop()

    def draw_at(self, ctx, pos):
        if self.enabled:
            if self.filled:
                ctx.circle_fill(pos, self.radius, self.num_pts, self.color)
            else:
                ctx.circle(pos, self.radius, self.num_pts, self.width, self.color)

    @classmethod
    def from_str(self, serializable):
        serializable = json.loads(serializable)
        return Circle(serializable[0], serializable[1], serializable[2], Col(*(serializable[3])), serializable[4], serializable[5])

    def __str__(self):
        return json.dumps([self.radius, self.num_pts, self.width, [self.color.r, self.color.g, self.color.b, self.color.a], self.filled, self.enabled])