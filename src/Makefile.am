## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(GRAPHCUT_CFLAGS) \
	-std=c++0x

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = graphcut

graphcut_SOURCES = \
	main.cpp \
	aux.h \
	graph.cpp \
	graph.h \
	imagenetwork.cpp \
	imagenetwork.h \
	network.cpp \
	network.h \
	partition.cpp \
	partition.h

graphcut_LDFLAGS = 

graphcut_LDADD = $(GRAPHCUT_LIBS)

