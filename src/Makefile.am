PREFIXDIR  = $(prefix)
PKGDATADIR = $(pkgdatadir)
DATADIR    = $(datadir)

INCLUDES = -DDATADIR=\"$(DATADIR)\" -DPKGDATADIR=\"$(PKGDATADIR)\" -DPREFIX=\"$(PREFIXDIR)\" $(LIBMB_CFLAGS)

bin_PROGRAMS = matchbox-desktop

matchbox_desktop_LDADD = $(LIBMB_LIBS)
matchbox_desktop_LDFLAGS = -export-dynamic

matchbox_desktop_SOURCES = \
 mbdesktop.c mbdesktop_item.c mbdesktop_view.c mbdesktop_module.c md5.c md5.h \
 mbdesktop.h mbdesktop_item.h mbdesktop_view.h mbdesktop_module.h


mbdesktopheadersdir = $(includedir)/matchbox-desktop
mbdesktopheaders_DATA = \
  mbdesktop.h mbdesktop_item.h mbdesktop_view.h mbdesktop_module.h
