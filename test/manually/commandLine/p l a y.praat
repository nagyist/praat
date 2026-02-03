form Play
	infile File_to_play e x a m p l e.wav
	natural Number 1
endform
writeInfoLine: "play.praat: ", file_to_play$
appendInfoLine: number, " times"
Read from file: file_to_play$
for i to number
	appendInfoLine: "Playing ", i
	Play
endfor
