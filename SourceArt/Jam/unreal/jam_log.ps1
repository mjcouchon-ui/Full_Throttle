# Pulls the lines that matter out of an editor job's log: the track fingerprints, what the dressing placed and what the
# clearance test moved or dropped, kit pieces, and errors.   powershell -File jam_log.ps1 -Log <file> [-Out <file>]
param([Parameter(Mandatory=$true)][string]$Log, [string]$Out = "", [string]$Pattern = "FTTRACK|FTDRESS|FTBUILD|FTCLEAR|FTWORLD|FTLIFE|did not compile|LogPython: Error|Traceback|error X|JAM ")
$lines = @(Select-String -LiteralPath $Log -Pattern $Pattern | Where-Object { $_.Line -notmatch 'RHIResources|HttpListener|ModelContext' } | ForEach-Object {
  $l = $_.Line; $i = $l.IndexOf(']Log'); if ($i -ge 0 -and $i -lt 40) { $l = $l.Substring($i + 1) }; $l.Substring(0, [Math]::Min(600, $l.Length)) })
if ($Out) { Set-Content -LiteralPath $Out -Value $lines -Encoding utf8 }
"log lines kept: {0}" -f $lines.Count
$lines | Where-Object { $_ -match "FTTRACK|FTDRESS level|JAM ERR|JAM kit|JAM build steps|LogPython: Error|did not compile|error X|FAILED" } | Select-Object -First 60
