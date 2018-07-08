## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	$(WX_CXXFLAGS) \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall \
	 -g \
	 $(WX_CPPFLAGS)

bin_PROGRAMS = galapagos

galapagos_SOURCES = \
	frame.cpp \
	frame.hpp \
	app.cpp \
	app.hpp \
	commit.cpp \
	commit.hpp \
	history-panel.cpp \
	history-panel.hpp \
	history-detail-panel.cpp \
	history-detail-panel.hpp \
	create-repository-dialog.cpp \
	create-repository-dialog.hpp \
	clone-repository-dialog.cpp \
	clone-repository-dialog.hpp \
	remotes-panel.cpp \
	remotes-panel.hpp \
	repository-properties-dialog.cpp \
	repository-properties-dialog.hpp

galapagos_LDFLAGS = 

galapagos_LDADD = \
	$(LIBGIT2PP_LIBS) \
	$(WX_LIBS)

