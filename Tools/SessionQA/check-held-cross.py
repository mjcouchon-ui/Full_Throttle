"""Read-only regression checker: python check-held-cross.py GAME_LOG [REPORT_JSON]."""
import json,re,sys
from pathlib import Path
p=Path(sys.argv[1]); raw=p.read_bytes()
log=raw.decode("utf-16" if raw.startswith((b"\xff\xfe",b"\xfe\xff")) else "utf-8-sig",errors="replace")
checks={}
exitpos=log.find("FT BRAWL SYSTEM exit=title discard=0")
titlepos=log.find("FT BRAWL SYSTEM screen=1",exitpos)
injectpos=log.find("command=ftb.Press Gamepad_FaceButton_Bottom 2",titlepos)
readypos=log.find("FT BRAWL SYSTEM input_ready=1",titlepos)
shots=log.find("command=shot held-cross-title-stable",injectpos)
# Log lines may have CRLF; use a full line boundary rather than matching the earlier long hold.
match=re.search(r"command=press Gamepad_FaceButton_Bottom\r?$",log[shots:],re.M) if shots>=0 else None
freshpos=shots+match.start() if match else -1
checks["actual_quit_title"]=exitpos>=0 and titlepos>exitpos
checks["held_event_injected_before_neutral_gate_open"]=titlepos<injectpos<readypos
checks["no_title_accept_before_fresh_press"]=freshpos>shots>=injectpos and "FT BRAWL SYSTEM activate screen=1 row=0" not in log[titlepos:freshpos]
states=re.findall(r"FT BRAWL QA STATE screen=(\d+) wave=(-?\d+) cut=(\d+) time=([\d.]+) worldT=([\d.]+)",log[injectpos:]) if injectpos>=0 else []
checks["title_remains_paused_during_and_after_hold"]=len(states)>=3 and [x[0] for x in states[:3]]==["1","1","0"]
checks["title_scene_clock_frozen"]=len(states)>=2 and states[0][3:]==states[1][3:]
checks["fresh_confirm_continues"]=freshpos>=0 and log.find("FT BRAWL SYSTEM activate screen=1 row=0",freshpos)>freshpos and log.find("FTBRAWL start DAY 1 ON SET before",freshpos)>freshpos
checks["actual_desktop_exit"]=log.find("FT BRAWL SYSTEM exit=desktop discard=0",freshpos)>freshpos and "Engine exit requested" in log
checks["no_timeout_fatal_ensure"]=not re.search(r"result=timeout|Fatal error:|Assertion failed:|Ensure condition failed",log)
report={"log":str(p.resolve()),"passed":all(checks.values()),"checks":checks,"states":states}
text=json.dumps(report,indent=2)
print(text)
if len(sys.argv)>2: Path(sys.argv[2]).write_text(text+"\n",encoding="utf-8")
sys.exit(0 if report["passed"] else 1)
