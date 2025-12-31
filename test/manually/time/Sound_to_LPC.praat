# test/manually/Sound_to_LPC.praat
# Paul Boersma, 31 December 2025

Debug multi-threading: "yes", 0, 7, "no"

timeStep = 0.001 ; seconds
samplingFrequency = 11000
sound1000 = Create Sound from formula: "sineWithNoise", 1, 0, 1000, samplingFrequency, "1/2 * sin(2*pi*377*x) + randomGauss(0,1)"
stopwatch
formant1000 = noprogress To LPC (burg): 10, 0.025, timeStep, 50.0
writeInfoLine: stopwatch
sound100 = Create Sound from formula: "sineWithNoise", 1, 0, 100, samplingFrequency, "1/2 * sin(2*pi*377*x) + randomGauss(0,1)"
stopwatch
formant100 = noprogress To LPC (burg): 10, 0.025, timeStep, 50.0
appendInfoLine: stopwatch
sound10 = Create Sound from formula: "sineWithNoise", 1, 0, 10, samplingFrequency, "1/2 * sin(2*pi*377*x) + randomGauss(0,1)"
stopwatch
formant10 = noprogress To LPC (burg): 10, 0.025, timeStep, 50.0
appendInfoLine: stopwatch
sound1 = Create Sound from formula: "sineWithNoise", 1, 0, 1, samplingFrequency, "1/2 * sin(2*pi*377*x) + randomGauss(0,1)"
stopwatch
formant1 = noprogress To LPC (burg): 10, 0.025, timeStep, 50.0
appendInfoLine: stopwatch
sound01 = Create Sound from formula: "sineWithNoise", 1, 0, 0.1, samplingFrequency, "1/2 * sin(2*pi*377*x) + randomGauss(0,1)"
stopwatch
formant01 = noprogress To LPC (burg): 10, 0.025, timeStep, 50.0
appendInfoLine: stopwatch

removeObject: sound01, sound1, sound10, sound100, sound1000, formant01, formant1, formant10, formant100, formant1000

margin = 0.0495 ; seconds
numberOfReplications = 100
maximumNumberOfFrames = 100
numberOfTimings = numberOfReplications * maximumNumberOfFrames
time# = zero# (maximumNumberOfFrames)
numbersOfFrames# = shuffle#: repeat# (to# (maximumNumberOfFrames), numberOfReplications)
for timing to numberOfTimings 
	desiredNumberOfFrames = numbersOfFrames# [timing]
	durationOfFrames = desiredNumberOfFrames * timeStep
	durationOfSound = durationOfFrames + margin
	sound = Create Sound from formula: "sineWithNoise", 1, 0, durationOfSound, samplingFrequency, "1/2 * sin(2*pi*377*x) + randomGauss(0,1)"
	stopwatch
	lpc = noprogress To LPC (burg): 10, 0.025, timeStep, 50.0
	time# [desiredNumberOfFrames] += stopwatch
	resultingNumberOfFrames = Get number of frames
	assert resultingNumberOfFrames = desiredNumberOfFrames   ; 'desiredNumberOfFrames' 'resultingNumberOfFrames'
	removeObject: sound, lpc
endfor

for numberOfFrames from 1 to maximumNumberOfFrames
	appendInfoLine: numberOfFrames, " ", round (time# [numberOfFrames] / numberOfReplications * 1e6), "μ"
endfor

appendInfoLine: "Mean: ", round (mean (time#) / numberOfReplications * 1e6), "μ"