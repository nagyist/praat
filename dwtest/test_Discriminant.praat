# test_Discriminant.praat
# djmw 20110518, 20141030, 20150128
# ppgb 20200505 added abs in four places

appendInfoLine: "test_Discriminant:"

@testOlderFormats

include readingAndWritingOfObjects.praat
@testReadAndWrite

@testClassify

appendInfoLine: "test_Discriminant: OK"


procedure testOlderFormats
	.test$ = tab$ + "Read older Discriminant formats:"
	appendInfoLine: .test$
	.evals# = {21.139001553325844, 4.355295817120748, 0.6828853876561858}
	
	appendInfo: tab$, tab$, "format 0:"
	.discriminant0 = Read from file: "pols1973_format0.Discriminant"
	.eigen0 = Extract Eigen
	.eigenvalues# = List eigenvalues
	for .i to size (.evals#)
		assert abs ((.eigenvalues# [.i] - .evals# [.i]) / .evals# [.i]) < 1e-14 ; '.i' 
	endfor
	removeObject: .discriminant0, .eigen0
	appendInfoLine: " OK"

	appendInfo: tab$, tab$, "format 1:"
	.discriminant1 = Read from file: "pols1973_format1.Discriminant"
	.eigen1 = Extract Eigen
	.eigenvalues# = List eigenvalues
	for .i to size (.evals#)
		assert abs ((.eigenvalues# [.i] - .evals# [.i]) / .evals# [.i]) < 1e-10 ; '.i' 
	endfor
	removeObject: .discriminant1, .eigen1
	appendInfoLine: " OK"

	appendInfoLine: .test$ + " OK"
endproc

procedure testReadAndWrite
	.tor = Create TableOfReal (Pols 1973): "yes"
	Formula: ~ log10 (self)
	.discriminant = To Discriminant
	@testReadingAndWritingOfObject: .discriminant, "Discriminant",
	removeObject: .tor, .discriminant
endproc

procedure testClassify
	appendInfo: tab$ + "test classify:"
	.tableOfReal = Create TableOfReal (Pols 1973): "no"
	Formula: "log10(self)"
	.discriminant = To Discriminant
	selectObject: .discriminant, .tableOfReal
	.classificationTable = To ClassificationTable: "yes", "yes"
	.confusion = To Confusion: "yes"
	.fractionCorrect = Get fraction correct
	assert abs (.fractionCorrect - 0.74) < 0.00001   ; 'fractionCorrect'
	removeObject: .tableOfReal, .discriminant, .classificationTable, .confusion
	appendInfoLine: " OK"
endproc
