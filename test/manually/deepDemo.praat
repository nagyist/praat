writeInfo: "1a "
demo Draw line: 0, 0, 100, 100
appendInfo: "1b "
demoWaitForInput ()
appendInfo: "1c "
demoWaitForInput ()
runScript: "deepDemo/folder2/script2.praat"
appendInfo: "1d "
demoWaitForInput ()
appendInfo: "1e "
demoWaitForInput ()
runScript: "deepDemo/folder3/script3.praat"
appendInfo: "1f "
demoWaitForInput ()
appendInfo: "1g "
demoWaitForInput ()
runScript: "deepDemo/folder4/script4.praat"
appendInfo: "1h "
info$ = info$()
assert info$ = "1a 1b 1c 2a 2+ 2b 1d 1e 3a 3+ 3b 1f 1g 4a 4+ 4b 1h "
