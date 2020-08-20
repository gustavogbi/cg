# Makefile do programa de mapeamento de texturas, criado a partir do Makefile
# do exemplo "cube" do wxWidgets.
# (C) Bruno de Oliveira Schneider - DCC - UFLA - Maio de 2017

# Este Makefile foi feito para Linux. Para compilar em Windows ou MacOS altere as flags do wxWidgets
# conforme exemplos de Makefile para esses sistemas operacionais.

# C++ compiler
CXX = g++

# Standard preprocessor flags (common for CC and CXX)
CPPFLAGS ?= -Wall

# Location and arguments of wx-config script
WX_CONFIG ?= wx-config

# Standard flags for C++
CXXFLAGS ?= -I. $(shell $(WX_CONFIG) --cxxflags $(WX_CONFIG_FLAGS)) $(CPPFLAGS)

# Standard linker flags
LDFLAGS ?= -lGL $(shell $(WX_CONFIG) $(WX_CONFIG_FLAGS) --libs gl,core,base)

# Port of the wx library to build against [gtk1,gtk2,msw,x11,motif,osx_cocoa,osx_carbon,dfb]
WX_PORT ?= $(shell $(WX_CONFIG) --query-toolkit)

# Use DLL build of wx library to use? [0,1]
WX_SHARED ?= $(shell if test -z `$(WX_CONFIG) --query-linkage`; then echo 1; else echo 0; fi)

# Compile Unicode build of wxWidgets? [0,1]
WX_UNICODE ?= $(shell $(WX_CONFIG) --query-chartype | sed 's/unicode/1/;s/ansi/0/')

# Version of the wx library to build against.
WX_VERSION ?= $(shell $(WX_CONFIG) --query-version | sed -e 's/\([0-9]*\)\.\([0-9]*\)/\1\2/')

# Nome do arquivo executável
EXECUTAVEL = map_textura

FONTES = main.cpp main.h

### Variables: ###
WX_FLAGS =
#CPPDEPS = -MT$@ -MF`echo $@ | sed -e 's,\.o$$,.d,'` -MD -MP
WX_VERSION_MAJOR = $(shell echo $(WX_VERSION) | cut -c1,1)
WX_VERSION_MINOR = $(shell echo $(WX_VERSION) | cut -c2,2)
WX_CONFIG_FLAGS = $(WX_CONFIG_UNICODE_FLAG) $(WX_CONFIG_SHARED_FLAG) \
	--toolkit=$(WX_PORT) --version=$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR)
OBJECTS = coordtex.o ponto.o vertice.o transformacao.o main.o

### Conditionally set variables: ###
ifeq ($(WX_UNICODE),0)
WX_CONFIG_UNICODE_FLAG = --unicode=no
endif
ifeq ($(WX_UNICODE),1)
WX_CONFIG_UNICODE_FLAG = --unicode=yes
endif
ifeq ($(WX_SHARED),0)
WX_CONFIG_SHARED_FLAG = --static=yes
endif
ifeq ($(WX_SHARED),1)
WX_CONFIG_SHARED_FLAG = --static=no
endif

### Targets: ###
all: $(EXECUTAVEL)

install:

uninstall:

clean:
	rm -f ./*.o
	rm -f ./*.d
	rm -f $(EXECUTAVEL)

map_textura: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) $(WX_FLAGS)

transformacao.o: transformacao.cpp transformacao.h
	$(CXX) $(CPPFLAGS) -c transformacao.cpp

coordtex.o: coordtex.cpp coordtex.h
	$(CXX) $(CPPFLAGS) -c coordtex.cpp

ponto.o: ponto.cpp ponto.h
	$(CXX) $(CPPFLAGS) -c ponto.cpp

vertice.o: vertice.cpp vertice.h
	$(CXX) $(CPPFLAGS) -c vertice.cpp

main.o: main.cpp main.h
	$(CXX) -c -o $@ $(CXXFLAGS) $(CPPDEPS) $<

.PHONY: all install uninstall clean


# Dependencies tracking:
-include ./*.d
                                                                                                                                                                                 # (C) Bruno de Oliveira Schneider - Não é permitido divulgar esta implementação - Publishing this code is forbidden
