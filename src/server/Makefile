# Copyright 2011 Robrecht Dewaele
#
# This file is part of mpgc.
#
# mpgc is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# mpgc is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with mpgc.  If not, see <http://www.gnu.org/licenses/>.
#
DEVELOPMENT=1
ifeq (${DEVELOPMENT},1)
CFLAGS:=-O0 -g -Wall -Wextra -pedantic $(CFLAGS)
else
CFLAGS:=-O2 $(CFLAGS)
endif

PROGRAM=mpgcd

SOURCES=$(PROGRAM:%=%.c) options.c server.c
OBJECTS=$(SOURCES:%.c=%.o)

CFLAGS+=-std=c11 -D_POSIX_C_SOURCE=200809L

LIBS=popt libmicrohttpd
CFLAGS+=$(shell pkg-config --cflags $(LIBS))
LDLIBS+=$(shell pkg-config --libs $(LIBS) || echo "-lpopt -lmicrohttpd")

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)

clean:
	rm -rf $(PROGRAM) $(OBJECTS)

.PHONY: clean
