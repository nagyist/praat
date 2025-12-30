# meson.build of the external library "whispercpp"
# David Weenink, 27 December 2025

sources = '''
	whisper.cpp ggml.c ggml-backend.cpp	ggml-backend-reg.cpp ggml-alloc.c
	ggml-threading.cpp ggml-quants.c ggml-cpu_.cpp ggml-cpu.c traits.cpp 
	quants.c unary-ops.cpp binary-ops.cpp vec.cpp ops.cpp
	'''.split()

libwhisper = static_library (
	'whisper',
	sources : sources,
	include_directories : melder_inc
)

libwhisper_dep = declare_dependency (
	include_directories : include_directories('.'), link_with: libwhisper
)
