writeInfoLine: "script 1, first time"
runScript: "deepPause/script2.praat"
appendInfoLine: "script 1, second time"
info$ = info$()
assert info$ =
... "script 1, first time" + newline$ +
... "script 2, first time" + newline$ +
... "script 3, first time" + newline$ +
... "script 4, first time" + newline$ +
... "script 4, second time" + newline$ +
... "script 3, second time" + newline$ +
... "script 2, second time" + newline$ +
... "script 1, second time" + newline$