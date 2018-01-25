CC=gcc
CFLAGS=-O0 -Wall -pedantic

libraries=sdl2 gl glu

CFLAGS:=$(shell pkg-config --cflags $(libraries)) $(CFLAGS)
LDLIBS:=$(shell pkg-config --libs $(libraries)) $(LDLIBS)

.PHONY: run
run: main
	-./$<

