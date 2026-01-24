#
# This script should be added to the Praat menu or so using "Add to fixed menu".
# It tests whether a script that has been added as a menu command (e.g. by a plug-in)
# can have both an initial input form and a pause form.
#

form: "Form and pause form"
	natural: "Count", "3"
endform

for try to count

	beginPause: "Pausing..."
		natural: "Number", 1
	endPause: "OK", 1
	
	appendInfoLine: number

endfor
