stopwatch
for i to 100
	sound = Create Sound from formula: "sineWithNoise", 1, 0, 1, 44100, "1/2 * sin(2*pi*377*x) + randomGauss(0,0.1)"
	textgrid = To TextGrid: "spraak", ""
	removeObject: sound, textgrid
endfor
writeInfoLine: stopwatch
beginPause: "hallo"
endPause: "OK", 1
stopwatch
for i to 100
	sound = Create Sound from formula: "sineWithNoise", 1, 0, 1, 44100, "1/2 * sin(2*pi*377*x) + randomGauss(0,0.1)"
	textgrid = To TextGrid: "spraak", ""
	removeObject: sound, textgrid
endfor
appendInfoLine: stopwatch
