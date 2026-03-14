# meson.build of the external library "whisper"
# David Weenink, 14 March 202

whisper_and_ggml_objects = '''
	whisper.cpp	ggml.c ggml-backend.cpp ggml-backend-dl.cpp
	ggml-backend-reg.cpp ggml-alloc.c ggml-threading.cpp
	ggml-quants.c'''.split()
	
ggml_cpu_objects = '''
	ggml-cpu.c ggml-cpu-cpp.cpp traits.cpp quants.c
	unary-ops.cpp binary-ops.cpp ops.cpp vec.cpp'''.split()
	
sources = whisper_and_ggml_objects + ggml_cpu_objects

libwhisper = static_library (
	'whisper',
	sources : sources,
	include_directories : melder_inc
)

libwhisper_dep = declare_dependency (
	include_directories : include_directories('.'), link_with: libwhisper
)
