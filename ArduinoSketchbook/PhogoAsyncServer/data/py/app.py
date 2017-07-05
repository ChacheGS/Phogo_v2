#!/usr/bin/env python3
#  -*- coding: utf-8 -*-

import sys
from os.path import dirname
sys.path.append(dirname(__file__))

from phogo import Phogo, test

Phogo
test




try:
    # if this succeeds, we are running in the browser
    from browser import document, DOMEvent
    
    document['loading'].dispatchEvent(DOMEvent('apploaded'))

except:
    # not in browser
    pass
