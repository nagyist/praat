# File: makefile.defs.linux.silent.BE

# System: Linux without sound
# Paul Boersma, 29 December 2025

CC ?= gcc
CXX ?= g++
LINK ?= g++

PKG_CONFIG ?= "pkg-config"

COMMONFLAGS = -D_FILE_OFFSET_BITS=64 -DUNIX -Dlinux `$(PKG_CONFIG) --cflags gtk+-3.0` \
	-DWORDS_BIGENDIAN=1 -DPA_BIG_ENDIAN \
	-Wreturn-type -Wunused -Wunused-parameter -Wuninitialized -O1 -g1 -pthread

CFLAGS = -std=gnu99 $(COMMONFLAGS) -Werror=missing-prototypes -Werror=implicit

CXXFLAGS = -std=c++17 $(COMMONFLAGS) -Wshadow

EXECUTABLE = praat

LIBS = `$(PKG_CONFIG) --libs gtk+-3.0` -no-pie -lm -static-libgcc -static-libstdc++ -lpthread -L /usr/lib/x86_64-linux-gnu

AR = ar
RANLIB = ls
ICON =
MAIN_ICON =
