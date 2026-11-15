"""Nights 4-7's own regulars (run 97578): import their frames (ArtSource/frames/<who>_*.png, cut by tools/n47foes.py)
into /Game/Brawl/Sprites/<Who>/. Headless:
  wispos --backstage -- UnrealEditor-Cmd <abs FullThrottleBrawl.uproject> -run=pythonscript -script="<abs>/brawl_n47foes.py"
then copy Content/Brawl/Sprites/<Who> to full-throttle-unreal (same paths)."""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import brawl_2d
WHO = ['firedancer', 'surfer', 'tourist', 'pitcook', 'pinsetter', 'leaguer', 'shoeclerk', 'lanehog',
       'skibum', 'linemate', 'boarder', 'goalie', 'deckhand', 'firstmate', 'waiter', 'tuxbrute']
fr = os.path.join(brawl_2d.SRC, 'frames')
names = sorted(f[:-4] for f in os.listdir(fr) if f.endswith('.png') and f.split('_')[0] in WHO)
got = brawl_2d.only(names)
print('N47FOES imported', len(got))
