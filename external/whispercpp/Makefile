# Makefile of the library "external/whispercpp"

include ../../makefile.defs

WHISPER_OBJECTS = \
	whisper.o \
	ggml.o \
	ggml-backend.o \
	ggml-backend-reg.o \
	ggml-alloc.o \
	ggml-threading.o \
	ggml-quants.o \
	ggml-cpu_.o \
	ggml-cpu.o \
	traits.o \
	quants.o \
	unary-ops.o \
	binary-ops.o \
	vec.o \
	ops.o

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
