target=shader.o
libraries=glew

CC=gcc
CFLAGS=-O0 -Wall -pedantic

CFLAGS:=$(shell pkg-config --cflags $(libraries)) $(CFLAGS)
LDLIBS:=$(shell pkg-config --libs $(libraries)) $(LDLIBS)

.PHONY: default clean
default: $(target)
clean::
	-rm $(target)

