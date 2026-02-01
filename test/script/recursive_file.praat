writeFileLine: "recursive_file/kanweg.txt", "script 1a"
appendFileLine: "recursive_file/kanweg.txt", "script 1b"
appendFileLine: "recursive_file/kanweg.txt", "script 1c"
runScript: "recursive_file/script2.praat"
appendFileLine: "recursive_file/kanweg.txt", "script 1d"
appendFileLine: "recursive_file/kanweg.txt", "script 1e"
appendFileLine: "recursive_file/kanweg.txt", "script 1f"
output$ = readFile$: "recursive_file/kanweg.txt"
writeInfo: output$
assert output$ =
... "script 1a" + newline$ + "script 1b" + newline$ + "script 1c" + newline$ +
... "script 2a" + newline$ + "script 2b" + newline$ + "script 2c" + newline$ +
... "script 3a" + newline$ + "script 3b" + newline$ + "script 3c" + newline$ +
... "script 4a" + newline$ + "script 4b" + newline$ + "script 4c" + newline$ +
... "script 4d" + newline$ + "script 4e" + newline$ + "script 4f" + newline$ +
... "script 3d" + newline$ + "script 3e" + newline$ + "script 3f" + newline$ +
... "script 2d" + newline$ + "script 2e" + newline$ + "script 2f" + newline$ +
... "script 1d" + newline$ + "script 1e" + newline$ + "script 1f" + newline$
