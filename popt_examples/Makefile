PROGRAMS=basicpopt

LIBS=popt
CFLAGS+=$(shell pkg-config --cflags $(LIBS))
LDLIBS+=$(shell pkg-config --libs $(LIBS))

CFLAGS+=-O0 -g

all: $(PROGRAMS)

clean:
	rm -rf $(PROGRAMS)

.PHONY: clean
