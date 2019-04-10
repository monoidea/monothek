# Copyright (C) 2018 Jo\u00EBl Kr\u00E4hemann
# 
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

AUTOMAKE_OPTIONS = foreign
SUBDIRS = po

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -std=gnu99 -include errno.h -I$(top_srcdir) -DMONOTHEK_DEVEL_MODE=1 -DSRCDIR=\"$(srcdir)\" -DDESTDIR=\"$(DESTDIR)$(datadir)\" -DPACKAGE_VERSION=\"$(PACKAGE_VERSION)\" -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security

# what flags you want to pass to the C compiler & linker
CFLAGS =  @CFLAGS@
LDFLAGS = @LDFLAGS@ $(LTLIBINTL)
LIBTOOL_DEPS = @LIBTOOL_DEPS@

monothek_ui_CFLAGS = @MONOTHEK_CFLAGS@
monothek_ui_LDFLAGS = @MONOTHEK_LDFLAGS@

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

bin_PROGRAMS=monothek-ui

libmonothekdir = $(libdir)/monothek
libmonothek_LTLIBRARIES = libmonothek.la

EXTRA_DIST = config.rpath \
	monothek/config.h.in \
	gtk-doc.make \
	aclocal.m4 \
	README \
	m4/Makefile.am \
	m4/Makefile.in \
	monothek/i18n.h \
	monothek/gettext.h \
	monothek/object/monothek_marshallers.list \
	m4/README \
	po/README

CLEANFILES = \
	m4/Makefile

# pkgconfig_DATA += libmonothek.pc

# include
# nobase_include_HEADERS += $(libmonothek_la_HEADERS_0)

libmonothek_la_HEADERS_0 = $(libmonothek_h_sources) $(libmonothek_lib_h_sources) $(libmonothek_object_h_sources) $(libmonothek_session_h_sources) $(libmonothek_audio_h_sources) $(libmonothek_recall_h_sources) $(libmonothek_audio_file_h_sources) $(libmonothek_model_h_sources) $(libmonothek_controller_h_sources) $(libmonothek_view_h_sources)
libmonothek_la_SOURCES = $(libmonothek_la_HEADERS_0) $(libmonothek_c_sources) $(libmonothek_lib_c_sources) $(libmonothek_object_c_sources) $(libmonothek_session_c_sources) $(libmonothek_audio_c_sources) $(libmonothek_recall_c_sources) $(libmonothek_audio_file_c_sources) $(libmonothek_model_c_sources) $(libmonothek_controller_c_sources) $(libmonothek_view_c_sources)

# library libmonothek
libmonothek_la_CFLAGS = $(LIBAGS_CFLAGS) $(LIBAGS_AUDIO_CFLAGS) $(LIBAGS_GUI_CFLAGS) $(GOBJECT_CFLAGS) $(LIBQRENCODE_CFLAGS) $(FONTCONFIG_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
libmonothek_la_LDFLAGS = $(LDFLAGS) -version-info 1:0:0 -shared -fPIC -Wl,-soname,libmonothek.so -pthread
libmonothek_la_LIBADD = -ldl -lm $(LIBAGS_LIBS) $(LIBAGS_AUDIO_LIBS) $(LIBAGS_GUI_LIBS) $(GOBJECT_LIBS) $(LIBQRENCODE_LIBS) $(FONTCONFIG_LIBS) $(GDKPIXBUF_LIBS) $(CAIRO_LIBS) $(GTK_LIBS)

# monothek
monothek_ui_SOURCES = $(monothek_ui_c_sources) $(monothek_uiheaders)

monothek_ui_CFLAGS += $(LIBAGS_CFLAGS) $(LIBAGS_AUDIO_CFLAGS) $(LIBAGS_GUI_CFLAGS) $(GOBJECT_CFLAGS) $(LIBQRENCODE_CFLAGS) $(FONTCONFIG_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
monothek_ui_LDFLAGS += -pthread
monothek_ui_LDADD = libmonothek.la -ldl -lm $(LIBAGS_LIBS) $(LIBAGS_AUDIO_LIBS) $(LIBAGS_GUI_LIBS) $(GOBJECT_LIBS) $(LIBQRENCODE_LIBS) $(FONTCONFIG_LIBS) $(GDKPIXBUF_LIBS) $(CAIRO_LIBS) $(GTK_LIBS)

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

deprecated_libmonothek_lib_h_sources =

libmonothek_lib_h_sources = \
	$(deprecated_libmonothek_lib_h_sources) \
	monothek/lib/monothek_qrcode.h

deprecated_libmonothek_lib_c_sources =

libmonothek_lib_c_sources = \
	$(deprecated_libmonothek_lib_c_sources) \
	monothek/lib/monothek_qrcode.c

deprecated_libmonothek_object_h_sources =

libmonothek_object_h_sources = \
	$(deprecated_libmonothek_object_h_sources) \
	monothek/object/monothek_marshal.h

deprecated_libmonothek_object_c_sources =

libmonothek_object_c_sources = \
	$(deprecated_libmonothek_object_c_sources) \
	monothek/object/monothek_marshal.c

deprecated_libmonothek_session_h_sources =

libmonothek_session_h_sources = \
	$(deprecated_libmonothek_session_h_sources) \
	monothek/session/monothek_session.h \
	monothek/session/monothek_session_manager.h

deprecated_libmonothek_session_c_sources =

libmonothek_session_c_sources = \
	$(deprecated_libmonothek_session_c_sources) \
	monothek/session/monothek_session.c \
	monothek/session/monothek_session_manager.c

deprecated_libmonothek_audio_h_sources =

libmonothek_audio_h_sources = \
	$(deprecated_libmonothek_audio_h_sources) \
	monothek/audio/monothek_rack.h \
	monothek/audio/monothek_recall_factory.h

deprecated_libmonothek_audio_c_sources =

libmonothek_audio_c_sources = \
	$(deprecated_libmonothek_audio_c_sources) \
	monothek/audio/monothek_rack.c \
	monothek/audio/monothek_recall_factory.c

deprecated_libmonothek_recall_h_sources =

libmonothek_recall_h_sources = \
	$(deprecated_libmonothek_recall_h_sources) \
	monothek/audio/recall/monothek_copy_pattern_audio.h \
	monothek/audio/recall/monothek_copy_pattern_audio_run.h \
	monothek/audio/recall/monothek_copy_pattern_channel.h \
	monothek/audio/recall/monothek_copy_pattern_channel_run.h \
	monothek/audio/recall/monothek_play_wave_audio.h \
	monothek/audio/recall/monothek_play_wave_audio_run.h \
	monothek/audio/recall/monothek_play_wave_channel.h \
	monothek/audio/recall/monothek_play_wave_channel_run.h

deprecated_libmonothek_recall_c_sources =

libmonothek_recall_c_sources = \
	$(deprecated_libmonothek_recall_c_sources) \
	monothek/audio/recall/monothek_copy_pattern_audio.c \
	monothek/audio/recall/monothek_copy_pattern_audio_run.c \
	monothek/audio/recall/monothek_copy_pattern_channel.c \
	monothek/audio/recall/monothek_copy_pattern_channel_run.c \
	monothek/audio/recall/monothek_play_wave_audio.c \
	monothek/audio/recall/monothek_play_wave_audio_run.c \
	monothek/audio/recall/monothek_play_wave_channel.c \
	monothek/audio/recall/monothek_play_wave_channel_run.c

deprecated_libmonothek_audio_file_h_sources =

libmonothek_audio_file_h_sources = \
	$(deprecated_libmonothek_audio_file_h_sources) \
	monothek/audio/file/monothek_audio_file_manager.h

deprecated_libmonothek_audio_file_c_sources =

libmonothek_audio_file_c_sources = \
	$(deprecated_libmonothek_audio_file_c_sources) \
	monothek/audio/file/monothek_audio_file_manager.c

deprecated_libmonothek_model_h_sources =

libmonothek_model_h_sources = \
	$(deprecated_libmonothek_model_h_sources) \
	monothek/ui/model/monothek_start_model.h \
	monothek/ui/model/monothek_closed_model.h \
	monothek/ui/model/monothek_outage_model.h \
	monothek/ui/model/monothek_jukebox_payment_model.h \
	monothek/ui/model/monothek_diskjokey_payment_model.h \
	monothek/ui/model/monothek_jukebox_mode_model.h \
	monothek/ui/model/monothek_jukebox_no_test_model.h \
	monothek/ui/model/monothek_jukebox_playlist_model.h \
	monothek/ui/model/monothek_jukebox_track_model.h \
	monothek/ui/model/monothek_jukebox_end_model.h \
	monothek/ui/model/monothek_jukebox_qrcode_model.h \
	monothek/ui/model/monothek_diskjokey_sequencer_model.h \
	monothek/ui/model/monothek_diskjokey_end_model.h \
	monothek/ui/model/monothek_diskjokey_qrcode_model.h

deprecated_libmonothek_model_c_sources =

libmonothek_model_c_sources = \
	$(deprecated_libmonothek_model_c_sources) \
	monothek/ui/model/monothek_start_model.c \
	monothek/ui/model/monothek_closed_model.c \
	monothek/ui/model/monothek_outage_model.c \
	monothek/ui/model/monothek_jukebox_payment_model.c \
	monothek/ui/model/monothek_diskjokey_payment_model.c \
	monothek/ui/model/monothek_jukebox_mode_model.c \
	monothek/ui/model/monothek_jukebox_no_test_model.c \
	monothek/ui/model/monothek_jukebox_playlist_model.c \
	monothek/ui/model/monothek_jukebox_track_model.c \
	monothek/ui/model/monothek_jukebox_end_model.c \
	monothek/ui/model/monothek_jukebox_qrcode_model.c \
	monothek/ui/model/monothek_diskjokey_sequencer_model.c \
	monothek/ui/model/monothek_diskjokey_end_model.c \
	monothek/ui/model/monothek_diskjokey_qrcode_model.c

deprecated_libmonothek_controller_h_sources =

libmonothek_controller_h_sources = \
	$(deprecated_libmonothek_controller_h_sources) \
	monothek/ui/controller/monothek_action_box.h \
	monothek/ui/controller/monothek_action_slider.h \
	monothek/ui/controller/monothek_start_controller.h \
	monothek/ui/controller/monothek_jukebox_payment_controller.h \
	monothek/ui/controller/monothek_diskjokey_payment_controller.h \
	monothek/ui/controller/monothek_jukebox_mode_controller.h \
	monothek/ui/controller/monothek_jukebox_no_test_controller.h \
	monothek/ui/controller/monothek_jukebox_playlist_controller.h \
	monothek/ui/controller/monothek_jukebox_track_controller.h \
	monothek/ui/controller/monothek_jukebox_end_controller.h \
	monothek/ui/controller/monothek_jukebox_qrcode_controller.h \
	monothek/ui/controller/monothek_diskjokey_sequencer_controller.h \
	monothek/ui/controller/monothek_diskjokey_end_controller.h \
	monothek/ui/controller/monothek_diskjokey_qrcode_controller.h \
	monothek/ui/controller/monothek_controller.h

deprecated_libmonothek_controller_c_sources =

libmonothek_controller_c_sources = \
	$(deprecated_libmonothek_controller_c_sources) \
	monothek/ui/controller/monothek_action_box.c \
	monothek/ui/controller/monothek_action_slider.c \
	monothek/ui/controller/monothek_start_controller.c \
	monothek/ui/controller/monothek_jukebox_payment_controller.c \
	monothek/ui/controller/monothek_diskjokey_payment_controller.c \
	monothek/ui/controller/monothek_jukebox_mode_controller.c \
	monothek/ui/controller/monothek_jukebox_no_test_controller.c \
	monothek/ui/controller/monothek_jukebox_playlist_controller.c \
	monothek/ui/controller/monothek_jukebox_track_controller.c \
	monothek/ui/controller/monothek_jukebox_end_controller.c \
	monothek/ui/controller/monothek_jukebox_qrcode_controller.c \
	monothek/ui/controller/monothek_diskjokey_sequencer_controller.c \
	monothek/ui/controller/monothek_diskjokey_end_controller.c \
	monothek/ui/controller/monothek_diskjokey_qrcode_controller.c \
	monothek/ui/controller/monothek_controller.c

deprecated_libmonothek_view_h_sources =

libmonothek_view_h_sources = \
	$(deprecated_libmonothek_view_h_sources) \
	monothek/ui/view/monothek_start_view.h \
	monothek/ui/view/monothek_closed_view.h \
	monothek/ui/view/monothek_outage_view.h \
	monothek/ui/view/monothek_jukebox_payment_view.h \
	monothek/ui/view/monothek_diskjokey_payment_view.h \
	monothek/ui/view/monothek_jukebox_mode_view.h \
	monothek/ui/view/monothek_jukebox_no_test_view.h \
	monothek/ui/view/monothek_jukebox_playlist_view.h \
	monothek/ui/view/monothek_jukebox_track_view.h \
	monothek/ui/view/monothek_jukebox_end_view.h \
	monothek/ui/view/monothek_jukebox_qrcode_view.h \
	monothek/ui/view/monothek_diskjokey_sequencer_view.h \
	monothek/ui/view/monothek_diskjokey_end_view.h \
	monothek/ui/view/monothek_diskjokey_qrcode_view.h \
	monothek/ui/view/monothek_view.h

deprecated_libmonothek_view_c_sources =

libmonothek_view_c_sources = \
	$(deprecated_libmonothek_view_c_sources) \
	monothek/ui/view/monothek_start_view.c \
	monothek/ui/view/monothek_closed_view.c \
	monothek/ui/view/monothek_outage_view.c \
	monothek/ui/view/monothek_jukebox_payment_view.c \
	monothek/ui/view/monothek_diskjokey_payment_view.c \
	monothek/ui/view/monothek_jukebox_mode_view.c \
	monothek/ui/view/monothek_jukebox_no_test_view.c \
	monothek/ui/view/monothek_jukebox_playlist_view.c \
	monothek/ui/view/monothek_jukebox_track_view.c \
	monothek/ui/view/monothek_jukebox_end_view.c \
	monothek/ui/view/monothek_jukebox_qrcode_view.c \
	monothek/ui/view/monothek_diskjokey_sequencer_view.c \
	monothek/ui/view/monothek_diskjokey_end_view.c \
	monothek/ui/view/monothek_diskjokey_qrcode_view.c \
	monothek/ui/view/monothek_view.c

# monothek_ui all files
monothek_uiheaders = \
	monothek/monothek_main.h

monothek_ui_c_sources = \
	monothek/monothek_main.c

gen-marshallers:
	glib-genmarshal --prefix=monothek_cclosure_marshal --header monothek/object/monothek_marshallers.list > monothek/object/monothek_marshal.h
	glib-genmarshal --prefix=monothek_cclosure_marshal --body monothek/object/monothek_marshallers.list > monothek/object/monothek_marshal.c
	echo "generated marshallers in monothek/object/"

check-gettext:
	@if test x$(USE_NLS) != "xyes" ; then echo "Missing gettext. Rerun configure and check for" \
	"'checking whether to use NLS... yes'!" ; exit 1 ; fi

update-po: check-gettext
	@find $(srcdir)/monothek/{object,session,audio,ui,led}/ -name "*.c" -print | sort > $(srcdir)/po/POTFILES.in.2 ; \
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
