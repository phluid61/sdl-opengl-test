CC=gcc
CFLAGS=-O0 -Wall -pedantic

target=shader.o
ifdef USE_GLEW
 libraries=glew
 CFLAGS:=-DUSE_GLEW $(CFLAGS)
else
 libraries=gl glu
endif

CFLAGS:=$(shell pkg-config --cflags $(libraries)) $(CFLAGS)
LDLIBS:=$(shell pkg-config --libs $(libraries)) $(LDLIBS)

.PHONY: default clean
default: $(target)
clean::
	-rm $(target)

