# Copyright (C) 2018 Jo\u00EBl Kr\u00E4hemann
# 
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

AUTOMAKE_OPTIONS = foreign
SUBDIRS = po

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -std=gnu99 -include errno.h -I$(top_srcdir) -DSRCDIR=\"$(srcdir)\" -DDESTDIR=\"$(DESTDIR)$(datadir)\" -DPACKAGE_VERSION=\"$(PACKAGE_VERSION)\" -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security

# what flags you want to pass to the C compiler & linker
CFLAGS =  @CFLAGS@
LDFLAGS = @LDFLAGS@ $(LTLIBINTL)
LIBTOOL_DEPS = @LIBTOOL_DEPS@

monothek_ui_CFLAGS = @MONOTHEK_CFLAGS@
monothek_ui_LDFLAGS = @MONOTHEK_LDFLAGS@

libmonothekdir = $(libdir)/monothek
libmonothek_LTLIBRARIES = libmonothek.la

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

bin_PROGRAMS=monothek-ui

CLEANFILES = \
	m4/Makefile

# pkgconfig_DATA += libmonothek.pc

# include
# nobase_include_HEADERS += $(libmonothek_la_HEADERS_0)

libmonothek_la_HEADERS_0 = $(libmonothek_h_sources) $(libmonothek_model_h_sources) $(libmonothek_controller_h_sources) $(libmonothek_view_h_sources)
libmonothek_la_SOURCES = $(libmonothek_la_HEADERS_0) $(libmonothek_c_sources) $(libmonothek_model_c_sources) $(libmonothek_controller_c_sources) $(libmonothek_view_c_sources)

# library libmonothek
libmonothek_la_CFLAGS = $(LIBAGS_CFLAGS) $(LIBAGS_AUDIO_CFLAGS) $(LIBAGS_GUI_CFLAGS) $(GOBJECT_CFLAGS) $(FONTCONFIG_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
libmonothek_la_LDFLAGS = $(LDFLAGS) -version-info 1:0:0 -shared -fPIC -Wl,-soname,libmonothek.so -pthread
libmonothek_la_LIBADD = -ldl -lm $(LIBAGS_CFLAGS) $(LIBAGS_AUDIO_CFLAGS) $(LIBAGS_GUI_CFLAGS) $(GOBJECT_CFLAGS) $(FONTCONFIG_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)

# monothek
monothek_ui_SOURCES = $(monothek_ui_c_sources) $(monothek_uiheaders)

monothek_ui_CFLAGS += $(LIBAGS_CFLAGS) $(LIBAGS_AUDIO_CFLAGS) $(LIBAGS_GUI_CFLAGS) $(GOBJECT_CFLAGS) $(FONTCONFIG_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
monothek_ui_LDFLAGS += -pthread
monothek_ui_LDADD = libmonothek.la -ldl -lm $(LIBAGS_LIBS) $(LIBAGS_AUDIO_LIBS) $(LIBAGS_GUI_LIBS) $(GOBJECT_LIBS) $(FONTCONFIG_LIBS) $(GDKPIXBUF_LIBS) $(CAIRO_LIBS) $(GTK_LIBS)

# libmonothek all files
deprecated_libmonothek_h_sources =

libmonothek_h_sources = \
	$(deprecated_libmonothek_h_sources) \
	monothek/ui/monothek_application_context.h \
	monothek/ui/monothek_window.h

deprecated_libmonothek_c_sources =

libmonothek_c_sources = \
	$(deprecated_libmonothek_c_sources) \
	monothek/ui/monothek_application_context.c \
	monothek/ui/monothek_window.c

deprecated_libmonothek_view_h_sources =

libmonothek_model_h_sources = \
	$(deprecated_libmonothek_model_h_sources) \
	monothek/ui/model/monothek_start_model.h

deprecated_libmonothek_model_c_sources =

libmonothek_model_c_sources = \
	$(deprecated_libmonothek_model_c_sources) \
	monothek/ui/model/monothek_start_model.c

deprecated_libmonothek_controller_h_sources =

libmonothek_controller_h_sources = \
	$(deprecated_libmonothek_controller_h_sources) \
	monothek/ui/controller/monothek_start_controller.h

deprecated_libmonothek_controller_c_sources =

libmonothek_controller_c_sources = \
	$(deprecated_libmonothek_controller_c_sources) \
	monothek/ui/controller/monothek_start_controller.c

deprecated_libmonothek_view_h_sources =

libmonothek_view_h_sources = \
	$(deprecated_libmonothek_view_h_sources) \
	monothek/ui/view/monothek_start_view.h \
	monothek/ui/view/monothek_closed_view.h \
	monothek/ui/view/monothek_outage_view.h \
	monothek/ui/view/monothek_view.h

deprecated_libmonothek_view_c_sources =

libmonothek_view_c_sources = \
	$(deprecated_libmonothek_view_c_sources) \
	monothek/ui/view/monothek_start_view.c \
	monothek/ui/view/monothek_closed_view.c \
	monothek/ui/view/monothek_outage_view.c \
	monothek/ui/view/monothek_view.c

# monothek_ui all files
monothek_uiheaders = \
	monothek/monothek_main.h

monothek_ui_c_sources = \
	monothek/monothek_main.c

check-gettext:
	@if test x$(USE_NLS) != "xyes" ; then echo "Missing gettext. Rerun configure and check for" \
	"'checking whether to use NLS... yes'!" ; exit 1 ; fi

update-po: check-gettext
	@find $(srcdir)/monothek/{ui,led}/ -name "*.c" -print | sort > $(srcdir)/po/POTFILES.in.2 ; \
	if diff $(srcdir)/po/POTFILES.in $(srcdir)/po/POTFILES.in.2 >/dev/null 2>&1 ; then \
		rm -f $(srcdir)/po/POTFILES.in.2 ; \
	else \
		mv $(srcdir)/po/POTFILES.in.2 $(srcdir)/po/POTFILES.in ; \
	fi
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-po

update-gmo: check-gettext
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

force-update-gmo: check-gettext
	touch po/*.po
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

force-update-gmo-%: check-gettext
	@language=`echo $@ | sed s/force-update-gmo-//` ; \
	if test ! -f po/$$language.po ; then echo "file po/$$language.po does not exist" ; exit 1 ; fi ; \
	touch po/$$language.po ; \
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

.PHONY: check-gettext update-po update-gmo force-update-gmo
