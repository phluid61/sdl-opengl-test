exe=main
objects=main engine entity
libraries=sdl2 gl glu

CC=gcc

CFLAGS:=$(shell pkg-config --cflags $(libraries)) $(CFLAGS)
LDLIBS:=$(shell pkg-config --libs $(libraries)) $(LDLIBS)

.PHONY: run clean
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

