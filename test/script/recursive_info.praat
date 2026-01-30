writeInfoLine: "script 1a"
appendInfoLine: "script 1b"
appendInfoLine: "script 1c"
runScript: "recursive_info/script2.praat"
appendInfoLine: "script 1d"
appendInfoLine: "script 1e"
appendInfoLine: "script 1f"
info$ = info$()
assert info$ =
... "script 1a" + newline$ + "script 1b" + newline$ + "script 1c" + newline$ +
... "script 2a" + newline$ + "script 2b" + newline$ + "script 2c" + newline$ +
... "script 3a" + newline$ + "script 3b" + newline$ + "script 3c" + newline$ +
... "script 4a" + newline$ + "script 4b" + newline$ + "script 4c" + newline$ +
... "script 4d" + newline$ + "script 4e" + newline$ + "script 4f" + newline$ +
... "script 3d" + newline$ + "script 3e" + newline$ + "script 3f" + newline$ +
... "script 2d" + newline$ + "script 2e" + newline$ + "script 2f" + newline$ +
... "script 1d" + newline$ + "script 1e" + newline$ + "script 1f" + newline$
