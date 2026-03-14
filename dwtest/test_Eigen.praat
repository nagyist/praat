# test_Eigen.praat
# djmw 20161116, 20180829, 20210609, 20260314

appendInfoLine: "test_Eigen.praat"

@testOlderFormats
include readingAndWritingOfObjects.praat
@testReadAndWrite

@testInterface
eps = 1e-7
@testDiagonals: 10


@test2by2
@test3by3
@testgeneralSquare

procedure testOlderFormats
	appendInfo: tab$, "test older formats:"
	.eigen = Read from file: "3x3s_version0.Eigen"
	.eigenvalues# = {11, 2, 1}
	.eigenvectors## = {{0, 0.4472135954999579,0.8944271909999159},
		... {1, 0, 0},
		... {0, -0.8944271909999159, 0.4472135954999579}}
	@testeigen: .eigen, 3, .eigenvalues#, .eigenvectors##, 1e-10
	removeObject: .eigen
	appendInfoLine: " OK" 
endproc

procedure testReadAndWrite
	.eigen = Read from file: "3x3s_version0.Eigen"
	@testReadingAndWritingOfObject: .eigen, "Eigen",
	removeObject: .eigen
endproc

procedure appendInterfaceName: .i, .text$
	if  .i = 1
		appendInfoLine: tab$, tab$, .text$
	endif
endproc

procedure testInterface
	appendInfoLine: tab$, "interface test:"
	for .i to 5
		.numberOfColumns = randomInteger (3, 12)
		.tableofreal = Create TableOfReal: "t", 100, .numberOfColumns
		Formula: ~ randomGauss (0, 1)
		.pca = To PCA
		.eigen = Extract Eigen
		@appendInterfaceName: .i, "Get number of eigenvalues"
		.numberOfEigenvalues = Get number of eigenvalues
		assert .numberOfEigenvalues == .numberOfColumns
		@appendInterfaceName: .i, "Get eigenvector dimension"
		.dimension = Get eigenvector dimension
		assert .dimension == .numberOfColumns
		for .j to .numberOfEigenvalues
			@appendInterfaceName: .i, "Get eigenvalue: "+ string$ (.j)
			.eigenvalue [.j] = Get eigenvalue: .j
		endfor
		for .j to .numberOfEigenvalues
			.sump = Get eigenvalue: .j
			for .k from .j to .numberOfEigenvalues
				@appendInterfaceName: .i, "Get sum of eigenvalues: " + string$ (.j) + ", " + string$ (.k)
				.sum = Get sum of eigenvalues: .j, .k
				assert .sum >= .eigenvalue [.j]
			endfor
		endfor

		for .j to .numberOfEigenvalues
			for .k from .j to .dimension
				@appendInterfaceName: .i, "Get eigenvector element: " + string$ (.j) + ", " + string$ (.k)
				.val[.j,.k] = Get eigenvector element: .j, .k
			endfor
		endfor
		for .j to .numberOfEigenvalues
			for .k to .dimension
				.val[.k] = Get eigenvector element: .j, .k
			endfor
			@appendInterfaceName: .i, "Invert eigenvector: " + string$ (.j)
			Invert eigenvector: .j
			for .k to .dimension
				.valk = Get eigenvector element: .j, .k
				assert .valk == - .val[.k]
			endfor	
		endfor
		removeObject: .tableofreal, .pca, .eigen
	endfor
	appendInfoLine: tab$, "interface test: OK"
endproc

procedure assertApproximatelyEqual: .val1, .val2, .eps, .comment$
	.diff = abs (.val1 -.val2)
	.tekst$ = .comment$ + " " + string$ (.val1) + ", " + string$ (.val2)
	if .val1 == 0
		assert .diff < .eps; '.tekst$'
	else
		.reldif =  .diff / abs(.val1)
		assert .reldif < .eps ; '.tekst$'
	endif
endproc

procedure test2by2
	.dim = 2
	.mat## = {{2, 1},
	...			  {1, 2}}
	.mat = Create simple Matrix: "2x2s", .dim, .dim, ~ .mat##[row,col]
	.eigenvalues# = {3, 1}
	.eigenvectors## = {{ 1/sqrt (2), 1/sqrt (2)},
		...						   {-1/sqrt (2), 1/sqrt (2)}}
	.eigen = To Eigen
	appendInfo: tab$, "2x2 symmetrical"
	@testeigen: .eigen, .dim, .eigenvalues#, .eigenvectors##, eps
	appendInfoLine: " OK"
	removeObject: .mat, .eigen
endproc

procedure testeigen: .eigen, .dim, .eigenvalues#, .eigenvectors##, .eps
	selectObject: .eigen
	.numberOfEigenvalues = Get number of eigenvalues
	assert .numberOfEigenvalues == .dim
	for .i to .dim
		.eval = Get eigenvalue: .i
		.comment$ = string$ (.dim) + " eigenvalue" + string$ (.i)
		@assertApproximatelyEqual: .eval, .eigenvalues# [.i], .eps, .comment$
		for .j to .dim
			.evecj = Get eigenvector element: .i, .j
			.comment$ = "eigenvector[" + string$ (.i) + "] [" +string$ (.j) + "]"
			.val = .eigenvectors## [.i,.j]
			@assertApproximatelyEqual: .evecj, .val, .eps, .comment$
		endfor
	endfor
endproc

procedure test3by3
	.dim = 3
	.mat## = {{2, 0, 0},
		...		  {0, 3, 4},
		...		  {0, 4, 9}}
	.mat = Create simple Matrix: "3x3s", .dim, .dim, ~ .mat##[row,col]
	.eigenvalues# = {11, 2 , 1}
	.eigenvectors## = {{0, 1/sqrt (5), 2/sqrt (5)},
		... 						 {1,          0,          0},
		...							 {0, -2/sqrt (5), 1/sqrt (5)}}
	.eigen = To Eigen
	appendInfo: tab$, "3x3 symmetrical: eigenvalues and eigenvectors:"
	@testeigen: .eigen, .dim, .eigenvalues#, .eigenvectors##, 1e-10
	appendInfoLine: " OK"
	removeObject: .mat, .eigen
endproc

procedure testDiagonals: .maxdim
	.text$ = tab$ + "test diagonals from 1 to " + string$ (.maxdim) + ":"
	appendInfoLine: .text$
	for .i to .maxdim
		@testDiagonal: .i
	endfor
	appendInfoLine: .text$ + " OK"
endproc

procedure diagonalData: .dim
	.name$ = string$(.dim) + "x" + string$ (.dim)
	.mat = Create simple Matrix: .name$, .dim, .dim, "0"
	.eigenvalues# = zero# (.dim)
	.eigenvectors## = zero## (.dim, .dim)
	for .i to .dim
		.val = .dim - .i + 1
		Set value: .i, .i, .val
		.eigenvalues# [.i] = .val
		.eigenvectors## [.i,.i] = 1
	endfor
endproc

procedure testDiagonal: .dim
	@diagonalData: .dim
	.matname$ = selected$ ("Matrix")
	.eigen = To Eigen
	appendInfo: tab$, tab$, .matname$ +" diagonal"
	@testeigen: .eigen, .dim, diagonalData.eigenvalues#, diagonalData.eigenvectors##, eps
	removeObject: .eigen, diagonalData.mat
	appendInfoLine: " OK"
endproc

procedure testgeneralSquare
	appendInfo: tab$, "3x3 general "
	.dim = 3
	.name$ = "3x3square"
	.mat## = {{0, 1, 0},
		...			{0, 0, 1},
		...			{1, 0, 0}}
	.mat = Create simple Matrix: .name$, .dim, .dim, ~ .mat## [row,col]
	# one of the eigenvalues is real (1) the other two are complex
	.given_re# = {1, -1/2, -1/2}
	.given_im# = {0, 0.5*sqrt(3), -0.5*sqrt(3)}
	Eigen (complex)
	.eigenvectors = selected ("Matrix", 1)
	.eigenvalues = selected ("Matrix", 2)
	selectObject: .eigenvalues
	.nrow = Get number of rows
	assert .nrow = 3
	# lite version of equality: check for occurrence
	# the eigenvalues of a real square matrix are not "sorted". We only know that complex conjugate 
	# eigenvalues have the one with positive imaginary part first.
	.eval_re# = {object [.eigenvalues, 1, 1], object [.eigenvalues, 2, 1],object [.eigenvalues, 3, 1]}
	.eval_im# = {object [.eigenvalues, 1, 2], object [.eigenvalues, 2, 2],object [.eigenvalues, 3, 2]}
	if .eval_re# [1] > 1-eps and .eval_re# [1] < 1+eps
		assert .eval_re# [2] / .given_re# [2] > 1-eps and .eval_re# [2] / .given_re# [2] < 1+eps
		assert .eval_re# [3] / .given_re# [3] > 1-eps and .eval_re# [3] / .given_re# [3] < 1+eps	
	else
		assert .eval_re# [1] / .given_re# [2] > 1-eps and .eval_re# [1] / .given_re# [2] < 1+eps
		assert .eval_re# [2] / .given_re# [3] > 1-eps and .eval_re# [2] / .given_re# [3] < 1+eps	
		assert .eval_re# [3] > 1-eps and .eval_re# [3] < 1+eps and .eval_im# [3] == 0		
	endif
	selectObject: .eigenvectors
	.ncol = Get number of columns
	assert .ncol = 6	
	removeObject: .mat, .eigenvectors, .eigenvalues
	appendInfoLine: " OK"
endproc

procedure testWNKMatrices
	.test$ = "test W__n_(k) matrices"
	appendInfoLine: tab$, .test$
	for .n from 6 to 20
			@testEigenvaluesOfOneWNKMatrix: .n
	endfor
	appendInfoLine: tab$, .test$, " OK"
endproc

procedure testEigenvaluesOfOneWNKMatrix: .n
	# The special symmetric tridiagonal nxn matrices W__n_[k] are defined as
	# W__n_[k]: diagonal [i]      = k,                   i = 1,...,n
	#           offDiagonal [i]   = sqrt(i*(2*n-1-i)/4), i = 1,...,n-2
	#           offDiagonal [n-1] = sqrt (n*(n-1)/2)
	# The eigenvalues of W_n_(n+1) are {2*n, ..., 4, 2}.
	# The eigenvalues of the principal (n-1)x(n-1) submatrix (without last column and row)
	# are {2*n-1, ..., 5, 3}.
	# .n = 6
	.k = .n + 1
	appendInfo: tab$, tab$, "test W_" + string$(.n) + "(" + string$(.k) + "):"
	.wn = Create simple Matrix: "Wn", .n, .n, ~ 0.0
	.bnm1 = sqrt(0.5*.n*(.n-1))
	Formula: ~ if row = col then .k else 
	... if row = col+1 then if row < .n then 0.5*sqrt(col*(2*.n-1-col)) else .bnm1 fi else
	... if col = row+1 then if col < .n then 0.5*sqrt(row*(2*.n-1-row)) else .bnm1 fi else 
	... 0.0 fi fi fi
	.wneigen = To Eigen (special): "symmetric tridiagonal", .n
	for .ival to 	.n
		.eval = Get eigenvalue: .ival
		assert abs (.eval / (2 * (.n + 1 -.ival)) - 1.0) < 1.0e-7
	endfor
	# Create the principal submatrix
	.wnps = Create simple Matrix: "Wns", .n-1, .n-1, ~ object[.wn, row, col]
	.wnpseigen = To Eigen (special): "symmetric tridiagonal", .n-1
	for .ival to 	.n-1
		.eval = Get eigenvalue: .ival
		assert abs (.eval / (2 * (.n + 1 -.ival) - 1) - 1.0) < 1.0e-7
	endfor
	removeObject: .wn, .wneigen, .wnps, .wnpseigen
	appendInfoLine: " OK"
endproc

procedure testKacSylvesterMatrices
	.test$ = "test Kac-Sylvester(n) matrices"
	appendInfoLine: tab$, .test$
	for .n from 6 to 20
			@testEigenvaluesOfOneKacSylvesterMatrix: .n
	endfor
	appendInfoLine: tab$, .test$, " OK"
endproc

procedure testEigenvaluesOfOneKacSylvesterMatrix: .n
	# The (n+1)x(n+1) Kac-Sylvester matrix KS has
	# KS(n): diagonal[i]       = 0,     for i =1...n+1
	#        upperDiagonal [i] = n+1-i, for i = 1,...,n
	#        lowerDiagonal [i] = i,     for i = 1,...,n
	# The eigenvalues are {2*k-n} for k = 1,...,n
	# .n = 5
	appendInfo: tab$, tab$, "test KacSylvester (" + string$(.n) + "):"
	.ks = Create simple Matrix: "Kn", .n+1, .n+1, ~ 
	... if row = col+1 then col else
	... if col = row+1 then .n+1-row else 0 fi fi
	.eigen = To Eigen (special): "symmetric", .n
	for .ival to 	.n
		.eval = Get eigenvalue: .ival
		assert abs (.eval / (2 * .ival - .n) - 1.0) < 1.0e-7
	endfor
	appendInfoLine: " OK"	
endproc

appendInfoLine: "test_Eigen.praat OK"	

