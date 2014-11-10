# gds Makefile
# ============
# Copyright 2014 Paul Griffiths
# Email: mail@paulgriffiths.net
#
# Distributed under the terms of the GNU General Public License.
# http://www.gnu.org/licenses/

MKFLPATH    := $(realpath $(lastword $(MAKEFILE_LIST)))
MKFLDIR     := $(dir $(MKFLPATH))
LIBDIR      := $(MKFLDIR)lib
BINDIR      := $(MKFLDIR)bin
INCLDIRPUB  := $(MKFLDIR)include/public
INCLDIRPRI  := $(MKFLDIR)include/private

SOURCES     := $(wildcard *.c)
OBJECTS	     = $(subst .c,.o,$(SOURCES))
DEPENDS	     = $(subst .c,.d,$(SOURCES))
LIBRARIES   := 
SAMPLES     :=
TESTS		:=

CC          := gcc
CFLAGS      := -std=c99 -pedantic -Wall -Wextra -ggdb -DDEBUG -DDEBUG_ALL
CFLAGS		+= -pthread
CPPFLAGS	:= -I$(INCLDIRPUB) -I$(INCLDIRPRI)
LDFLAGS     := -pthread
RM          := rm -f
SED         := sed
CTAGS       := ctags

CLNGLOB      = $(LIBRARIES) $(OBJECTS) $(DEPENDS) $(SAMPLES) $(TESTS) tags

default: all

include src/module.mk
include samples/module.mk
include tests/module.mk

all: libraries samples tests

libraries: $(LIBRARIES)

samples: $(LIBRARIES) $(SAMPLES)

tests: $(LIBRARIES) $(TESTS)

# clean - removes ancilliary files from working directory

.PHONY: clean
clean:
	-@$(RM) $(CLNGLOB) 2>/dev/null

# tags - makes tags file

.PHONY: tags
tags:
	@$(CTAGS) $(SOURCES)

# docs - makes Doxygen documentation:
.PHONY: docs
docs:
	@doxygen
	-@cd latex; make; cd ..
	@if [ ! -d docs ]; then \
		mkdir docs; fi
	@echo "Copying reference manual to docs..."
	@cp latex/refman.pdf docs 
	@echo "Done."

# Dependencies

-include $(DEPENDS)

%.d: %.c
	@$(CC) -M $(CFLAGS) $(CPPFLAGS) $< > $@.tmp; \
		$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@; \
		$(RM) $@.tmp
