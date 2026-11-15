# Package Development and Shipping, then profile A (sky on), B (sky hidden), L (low effects) on Development.
$U = "K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal"; $d = "K:\Full Throttle\SkyScratch\DevLog\perf"
powershell -NoProfile -File "$U\package.ps1" -Config Development | Select-String "UAT exit|MB total"
foreach ($r in "A","B","L","C") { $o = powershell -NoProfile -File "$U\profile.ps1" -Run $r; $o | Select-String "csv ->" }
foreach ($r in "A","B","L") { & "K:\UE_5.9\Engine\Binaries\DotNET\CsvTools\csvinfo.exe" "$d\$r\profile.csv" -showAllStats -statFilters "FrameTime,GPUTime,RenderThreadTime,GameThreadTime,RHIThreadTime,RHI/DrawCalls,RHI/PrimitivesDrawn,GPU/*,PSO/*" -toJson "$d\$r\stats.json" | Out-Null }
python -I "$U\analyse_perf.py" "K:/Full Throttle/SkyScratch/DevLog/perf" > "$d\AB_comparison.txt"
Get-Content "$d\AB_comparison.txt" | Select-Object -First 12
powershell -NoProfile -File "$U\package.ps1" -Config Shipping | Select-String "UAT exit|MB total"
# low-quality summary and the Insights hitch check on the final Development build
python -I -c "import json,statistics as st;L=lambda r:{k:v['all'] for k,v in json.load(open(r'$d\'+r+r'\stats.json'))['stats'].items() if 'all' in v};A,B,Q=L('A'),L('B'),L('L');m=lambda d,k:st.mean(d[k][240:]);open(r'$d\summary_low.txt','w').write(''.join('%-16s A %.3f  L %.3f  B %.3f   sky(A-B) %+.3f  sky low(L-B) %+.3f\n'%(k,m(A,k),m(Q,k),m(B,k),m(A,k)-m(B,k),m(Q,k)-m(B,k)) for k in ['GPUTime','RHI/DrawCalls','GameThreadTime','RHIThreadTime']))"
Get-Content "$d\summary_low.txt"
$o = powershell -NoProfile -File "$U\profile.ps1" -Run D; $o | Select-String "exit"
$dir = "K:/Full Throttle/SkyScratch/DevLog/perf/D"
"TimingInsights.ExportTimingEvents `"$dir/export/pso_events.csv`" -columns=ThreadName,TimerName,StartTime,Duration`nTimingInsights.ExportTimingEvents `"$dir/export/frames.csv`" -columns=ThreadName,TimerName,StartTime,Duration -threads=GameThread" | Out-File -Encoding ascii "$dir/export/cmds3.rsp"
$p = Start-Process "K:\UE_5.9\Engine\Binaries\Win64\UnrealInsights.exe" -NoNewWindow -PassThru -WorkingDirectory "K:\Full Throttle\SkyScratch\Saved\AgentTemp" -RedirectStandardOutput "K:\Full Throttle\SkyScratch\Saved\AgentTemp\insights_stdout3.txt" -ArgumentList ("-OpenTraceFile=`"$dir/run.utrace`" -unattended -autoquit -noui -nullrhi -ExecOnAnalysisCompleteCmd=`"@=$dir/export/cmds3.rsp`" -abslog=`"$dir/export/insights3.log`"")
$null = $p.Handle; $null = $p.WaitForExit(900000)
python -I -c "import csv;r=list(csv.DictReader(open(r'$dir/export/pso_events.csv')));t=sorted(float(x['StartTime']) for x in r if x['TimerName']=='FEngineLoop::Tick');t=t or sorted(float(x['StartTime']) for x in csv.DictReader(open(r'$dir/export/frames.csv')) if x['TimerName']=='FEngineLoop::Tick');w=t[240];s=[x for x in r if x['TimerName'] in ('FCompilePipelineStateTask','FIoStoreShaderCodeArchive::CreateShader') and float(x['StartTime'])>w];open(r'$dir/hitch_check.txt','w').write('warm-up ends %.2fs; PSO compiles or shader creations after warm-up: %d\n'%(w,len(s)));print(open(r'$dir/hitch_check.txt').read())"
