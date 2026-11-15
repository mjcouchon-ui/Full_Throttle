"""Run 21219: import the pickups (ArtSource/frames/item_*) and Nova's held-item poses, nothing else.
Headless: UnrealEditor-Cmd FullThrottleBrawl.uproject -run=pythonscript -script="<abs>/Content/Python/brawl_items.py" -nullrhi"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import brawl_2d
NAMES = ['item_beer', 'item_bottle', 'item_cue', 'item_hotdog', 'item_knucks', 'item_jacket', 'item_pan',
         'nova_drink', 'nova_pickup', 'nova_overhead', 'nova_bash']
print('BRAWL_ITEMS', brawl_2d.only(NAMES))
