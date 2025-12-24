# Makefile of the library "external/whispercpp"

include makefile.defs

CPPFLAGS += -D_GNU_SOURCE
CPPFLAGS += -DWHISPER_VERSION=\"1.8.2\"
CPPFLAGS += -DGGML_VERSION=\"0.9.4\"
CPPFLAGS += -DGGML_COMMIT
CPPFLAGS += -DGGML_USE_CPU
CPPFLAGS += -DGGML_CPU_GENERIC

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

all:  libwhisper.a

clean:
	$(RM) $(WHISPER_OBJECTS)
	$(RM) libwhisper.a

libwhisper.a: $(WHISPER_OBJECTS)
	touch libwhisper.a
	rm libwhisper.a
	$(AR) cq libwhisper.a $(WHISPER_OBJECTS)
	$(RANLIB) libwhisper.a

$(WHISPER_OBJECTS): *.h
