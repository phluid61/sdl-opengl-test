exe=main
#objects=main engine entity model shader text texture timing
objects=main engine entity model text texture timing
ifdef USE_GLEW
 libraries=sdl2 glew
 CFLAGS:=-DUSE_GLEW $(CFLAGS)
else
 libraries=sdl2 gl glu
endif

CC=gcc

CFLAGS:=$(shell pkg-config --cflags $(libraries)) $(CFLAGS)
LDLIBS:=$(shell pkg-config --libs $(libraries)) $(LDLIBS)

.PHONY: default run clean

default: $(exe)

run: $(exe)
	-./$<

$(exe): $(foreach obj,$(objects),$(obj).o)
	$(LINK.o) $^ $(LDLIBS) -o $@

define OBJ_template =
$(1).o::
	make -f Makefile.$(1)
clean::
	make -f Makefile.$(1) clean
endef
$(foreach obj,$(objects),$(eval $(call OBJ_template,$(obj))))

clean::
	-rm $(exe)

