#  -*- coding: utf-8 -*-

# bring Phogo to this namespace
from .phogo import Phogo, BROWSER, test
from browser import window # Brython specific module

json_endpoint = '//{}/json'.format(window.location.hostname)

phogo = Phogo(json_endpoint)

distance = phogo.distance
forward = phogo.forward
backward = phogo.backward
right = phogo.right
left = phogo.left
pen_up = phogo.pen_up
pen_down = phogo.pen_down

# and make it importable
__all__ = ["distance", "forward", "backward", "right", "left", "pen_up", "pen_down", 'Phogo', 'BROWSER', 'test']
