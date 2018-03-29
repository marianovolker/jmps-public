"""
.. module:: GVRlib.cx_imports 
   :platform: Unix, Windows
   :synopsis: Extra imports needed by "cx_freeze".
This module contains extra imports needed by ``cx_freeze`` to import dynamic modules or restrict module visibility.
"""

# System modules
import sys, os

parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if parent_dir not in sys.path:
    sys.path.append(parent_dir)
import importlib
try:
    thirdparty = importlib.import_module('3rdparty')
except ImportError:
    thirdparty = None
else:
    try:
        attrlist = thirdparty.__all__
    except AttributeError:
        attrlist = dir(thirdparty)
    for attr in attrlist:
        globals()[attr] = getattr(thirdparty, attr)
