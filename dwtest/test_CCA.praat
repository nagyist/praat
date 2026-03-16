# test_CCA.praat
# djmw 20260314

appendInfoLine: "test_CCA.praat"

@testOlderFormats

include readingAndWritingOfObjects.praat

@testReadAndWrite

appendInfoLine: "test_CCA.praat: OK"

procedure testOlderFormats
	appendInfo: tab$, "test older formats:"
	.cca = Read from file: "pols1973_version0.CCA"
	.numberOfCoefficients = 3
	.numberOfCorrelation = 3
	.numberOfObservations = 600
	.ncorr = Get number of correlations
	assert .ncorr = .numberOfCorrelation
	.numberOfEigenvalues = 3
	.eigen_y = Extract Eigen: "Dependent"
	.neigen_y = Get number of eigenvalues
	assert .neigen_y = .numberOfEigenvalues
	selectObject: .cca
	.eigen_x = Extract Eigen: "Independent"
	.neigen_x = Get number of eigenvalues
	assert .neigen_x = .numberOfEigenvalues
	removeObject: .cca, .eigen_x, .eigen_y
	appendInfoLine: " OK" 
endproc


procedure testReadAndWrite
	.tor = Create TableOfReal (Pols 1973): "yes"
	.cca = To CCA: 3
	@testReadingAndWritingOfObject: .cca, "CCA",
	removeObject: .tor, .cca
endproc


