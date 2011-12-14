PROGRAMS=hellobrowser

LIBS=libmicrohttpd
CFLAGS=$(shell pkg-config --cflags $(LIBS))
LDLIBS=$(shell pkg-config --libs $(LIBS))

$(PROGRAMS):

clean:
	rm -rf $(PROGRAMS)

.PHONY: clean
