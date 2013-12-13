## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

graphcutdocdir = ${prefix}/doc/graphcut
graphcutdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(graphcutdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(graphcutdocdir)
