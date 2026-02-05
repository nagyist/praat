#
# This leaks in the combination of "View & Edit" and "removeObject".
# it leaks 5 Things every time (on the Mac, not on Windows).
#
# TODO: figure out what these are.
# Steps:
#    1. create a Melder_debug number that aims to trace all existing Things.
#    2. create an unordered_map that keeps track every time a Thing is created or deleted.
#

objects# = zero# (5)
for i to size (objects#)
    objects# [i] = Create Sound as pure tone: "tone", 1, 0, 0.4, 44100, 440, 0.2, 0.01, 0.01
endfor

for i to size (objects#)
    selectObject: objects# [i]
    View & Edit

    editor: objects# [i]
    beginPause: "test"
    clicked = endPause: "Next", 1
    endeditor

    removeObject: objects# [i]
endfor

memoryReport$ = Report memory use
appendInfoLine: mid$ (memoryReport$, 22, 160)
