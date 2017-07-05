# insert this path into sys.path
import sys
from os.path import dirname
sys.path.append(dirname(__file__))

from object_proxy import ObjectProxy
from browser import window


class Console(ObjectProxy):
    """docstring for Console"""

    def __init__(self):
        target = window.console
        super(Console, self).__init__(target)
