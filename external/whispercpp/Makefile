# Makefile of the library "external/whispercpp"

include ../../makefile.defs

#CPPFLAGS = -march=native

WHISPER_AND_GGML_OBJECTS = \
	whisper.o \
	ggml.o \
	ggml-cpp.o \
	ggml-backend.o \
	ggml-backend-dl.o \
	ggml-backend-reg.o \
	ggml-alloc.o \
	ggml-threading.o \
	ggml-quants.o \
	ggml-opt.o \
	gguf.o

GGML_CPU_OBJECTS = \
	ggml-cpu.o \
	ggml-cpu-cpp.o \
	traits.o \
	quants.o \
	ops.o \
	vec.o \
	unary-ops.o \
	binary-ops.o \
	repack.o \
	hbm.o

WHISPER_OBJECTS = $(WHISPER_AND_GGML_OBJECTS) $(GGML_CPU_OBJECTS)

.PHONY: all clean

all: libwhisper.a

clean:
	$(RM) $(WHISPER_OBJECTS)
	$(RM) libwhisper.a

libwhisper.a: $(WHISPER_OBJECTS)
	touch libwhisper.a
	rm libwhisper.a
	$(AR) cq libwhisper.a $(WHISPER_OBJECTS)
	$(RANLIB) libwhisper.a

$(WHISPER_OBJECTS): *.h
