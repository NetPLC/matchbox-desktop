EXTRA_DIST = simplefilebrowser-example.desktop 

PREFIXDIR  = $(prefix)
PKGDATADIR = $(pkgdatadir)
DATADIR    = $(datadir)

INCLUDES = -DDATADIR=\"$(DATADIR)\" -DPKGDATADIR=\"$(PKGDATADIR)\" -DPREFIX=\"$(PREFIXDIR)\" $(LIBMB_CFLAGS)

libdir = $(pkgdatadir)/desktop/modules

lib_LTLIBRARIES = tasks.la dotdesktop.la simplefilebrowser.la

tasks_la_SOURCES = tasks.c
tasks_la_LIBADD  = $(LIBMB_LIBS)
tasks_la_LDFLAGS = -avoid-version -module -export-dynamic
# -no-undefined

dotdesktop_la_SOURCES = dotdesktop.c
dotdesktop_la_LIBADD  = $(LIBMB_LIBS)
dotdesktop_la_LDFLAGS = -avoid-version -module -export-dynamic

simplefilebrowser_la_SOURCES = simplefilebrowser.c
simplefilebrowser_la_LIBADD  = $(LIBMB_LIBS)
simplefilebrowser_la_LDFLAGS = -avoid-version -module -export-dynamic

