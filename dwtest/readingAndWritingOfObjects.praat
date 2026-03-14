# readingAndWritingOfObjects.praat
# djmw 20260314

procedure testReadingAndWritingOfObject: .object, .typeName$
	.test$ = tab$ + "test read and write of " + .typeName$ + ":"
	appendInfoLine: .test$

	appendInfo: tab$, tab$, "as text"
	selectObject: .object
	.fileName$ = "kanweg_text." + .typeName$
	Save as text file: .fileName$
	.object_text = Read from file: .fileName$
	deleteFile: .fileName$
	appendInfoLine: " OK"

	appendInfo: tab$, tab$, "as short text"
	selectObject: .object
	.fileName$ = "kanweg_shortText." + .typeName$
	Save as short text file: .fileName$
	.object_shortText = Read from file: .fileName$
	deleteFile: .fileName$
	appendInfoLine: " OK"

	appendInfo: tab$, tab$, "as binary"
	selectObject: .object
	.fileName$ = "kanweg_binary." + .typeName$
	Save as binary file: .fileName$
	.object_binary = Read from file: .fileName$
	deleteFile: .fileName$
	appendInfoLine: " OK"

	removeObject: .object_text, .object_shortText, .object_binary
	appendInfoLine: .test$ + " OK"
endproc





