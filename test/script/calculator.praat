# Praat script test/script/calculator.praat
# Paul Boersma, 31 January 2026

a$ = Calculator: ~ 5 * 6
assert a$ = "30"

writeInfoLine: "OK"