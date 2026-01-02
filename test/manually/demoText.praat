#
# First version by Xavier St-Gelais, posted to the Praat list by Thea Knowles on 2018-09-18
#
comment$ = ""
label TEXTINPUT
	demo Select outer viewport: 0, 100, 0, 100
	demo Axes: 0, 100, 0, 100
	demo Erase all
	demo Text: 40, "centre", 80, "half",
	... "Enter some text using alphanumeric characters. To erase, press BACKSPACE. To finish, press → (RIGHT ARROW)."
	demo Red
	demo Font size: 15
	demo Text: 40, "left", 50, "half", comment$
	demo Font size: 10
	demo Black
	while demoWaitForInput ( )
		if demoKeyPressed ( )
			car$ = demoKey$ ( )
			if car$ = "→"
				goto END_TEXTINPUT
			elsif car$ = unicode$ (8) or car$ = unicode$ (127)
				comment$ = left$ (comment$, length (comment$) - 1)
				goto REFRESH
			elsif car$ = unicode$ (10) or car$ = unicode$ (13)
				comment$ += "😀"
				goto REFRESH
			else
				comment$ += car$
				goto REFRESH
			endif
		endif
	endwhile
label REFRESH
	comment$ = replace$ (comment$, newline$, "", 0)
	comment$ = replace$ (comment$, tab$, "", 0)
	goto TEXTINPUT
label END_TEXTINPUT
