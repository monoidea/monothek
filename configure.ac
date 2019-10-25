#                                               -*- Autoconf -*-
# Copyright (C) 2018 Jo\u00EBl Kr\u00E4hemann
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([monothek],[1.0.0-PRERELEASE],[jkraehemann@gmail.com])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([monothek/config.h.in])
AC_CONFIG_HEADERS([monothek/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_USE_SYSTEM_EXTENSIONS([_GNU_SOURCE])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])

# options
AC_ARG_ENABLE(gnulib, [AS_HELP_STRING([--enable-gnulib],
				      [gnulib builtin functions (default is no)])],
	      [],			       
	      [enable_gnulib=no])

AC_ARG_ENABLE(w32api, [AS_HELP_STRING([--enable-w32api],
		      		      [enable w32 API (default is no)])],
	      [],
	      [enable_w32api=no])

AC_ARG_ENABLE(osxapi, [AS_HELP_STRING([--enable-osxapi],
		      		      [enable OS X API (default is no)])],
	      [],
	      [enable_osxapi=no])

AC_ARG_ENABLE(x11, [AS_HELP_STRING([--enable-x11],
				   [enable X11 (default is yes)])],
	      [],
	      [enable_x11=yes])

AC_ARG_ENABLE(quartz, [AS_HELP_STRING([--enable-quartz],
				      [enable quartz (default is no)])],
	      [],
	      [enable_quartz=no])

# Checks for programs.
AC_PROG_CC
gl_EARLY
AC_PROG_CPP
AC_PROG_INSTALL

# libtool
LT_PREREQ([2.4.2])
LT_INIT
AC_SUBST([LIBTOOL_DEPS])
AC_ENABLE_SHARED

AM_CONDITIONAL([WITH_GNULIB], [test "x$enable_gnulib" == xyes])

AC_SUBST([MONOTHEK_CFLAGS])
AC_SUBST([MONOTHEK_LDFLAGS])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lX11':
AS_IF([test "x$enable_x11" != xno],
      [AC_DEFINE([MONOTHEK_WITH_X11], [1], [X11 enabled])
       AC_CHECK_LIB([X11], [main], , AC_MSG_ERROR("No X11 library present"))
       X11_LIBS="-lX11"],
      [AC_MSG_NOTICE([X11 disabled])])

AS_IF([test "x$enable_quartz" != xno],
      [AC_DEFINE([MONOTHEK_WITH_QUARTZ], [1], [quartz enabled])
       PKG_CHECK_MODULES([GTK_MAC_INTEGRATION], [gtk-mac-integration-gtk2 >= 2.0.7])
       AC_SUBST(GTK_MAC_INTEGRATION_CFLAGS)
       AC_SUBST(GTK_MAC_INTEGRATION_LIBS)
       AC_MSG_NOTICE([quartz enabled])],
      [AC_MSG_NOTICE([quartz disabled])])

# FIXME: Replace `main' with a function in `-ldl':
# AC_CHECK_LIB([dl], [main], , AC_MSG_ERROR("No dl library present"))

# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main], , AC_MSG_ERROR("No math library present"))

AS_IF([test "x$enable_w32api" != xno],
      [AC_DEFINE([MONOTHEK_W32API], [1], [w32api enabled])
      W32API_CFLAGS="-nostdinc -I./lib -I/usr/lib/gcc/x86_64-w64-mingw32/7.4.0/include -I/usr/x86_64-w64-mingw32/sys-root/mingw/include -I/usr/x86_64-w64-mingw32/sys-root/mingw/include/dssi -include monothek/config.h "
      W32API_LIBS="-L./lib -lgnu"
      AC_MSG_NOTICE([w32api enabled])],
      AC_MSG_NOTICE([w32api disabled]))
AM_CONDITIONAL([WITH_W32API], [test "x$enable_w32api" == xyes])

AS_IF([test "x$enable_osxapi" != xno],
      [AC_DEFINE([MONOTHEK_OSXAPI], [1], [OS X api enabled])
      OSXAPI_CFLAGS=""
      OSXAPI_LIBS=""
      AC_MSG_NOTICE([OS X api enabled])],
      AC_MSG_NOTICE([OS X api disabled]))
AM_CONDITIONAL([WITH_OSXAPI], [test "x$enable_osxapi" == xyes])

PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= 2.8.0])
AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)

PKG_CHECK_MODULES([GOBJECT], [gobject-2.0 >= 2.32.3 glib-2.0 >= 2.32.3 gmodule-2.0 >= 2.32.3 gthread-2.0 >= 2.32.3])
AC_SUBST(GOBJECT_CFLAGS)
AC_SUBST(GOBJECT_LIBS)

PKG_CHECK_MODULES([LIBQRENCODE], [libqrencode >= 4.0.0])
AC_SUBST(LIBQRENCODE_CFLAGS)
AC_SUBST(LIBQRENCODE_LIBS)

PKG_CHECK_MODULES([LIBAGS], [libags >= 2.2.0])
AC_SUBST(LIBAGS_CFLAGS)
AC_SUBST(LIBAGS_LIBS)

PKG_CHECK_MODULES([LIBAGS_AUDIO], [libags_audio >= 2.2.0])
AC_SUBST(LIBAGS_AUDIO_CFLAGS)
AC_SUBST(LIBAGS_AUDIO_LIBS)

PKG_CHECK_MODULES([FONTCONFIG], [fontconfig >= 2.9.0])
AC_SUBST(FONTCONFIG_CFLAGS)
AC_SUBST(FONTCONFIG_LIBS)

PKG_CHECK_MODULES([GDKPIXBUF], [gdk-pixbuf-2.0 >= 2.32.2])
AC_SUBST(GDKPIXBUF_CFLAGS)
AC_SUBST(GDKPIXBUF_LIBS)

PKG_CHECK_MODULES([CAIRO], [cairo >= 1.12.0])
AC_SUBST(CAIRO_CFLAGS)
AC_SUBST(CAIRO_LIBS)

PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.24.10])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

# check for gtk-doc
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.14],[--flavour no-tmpl])
],[
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])

gl_INIT

AM_EXTRA_RECURSIVE_TARGETS([monothek-docs])
AC_CONFIG_FILES([
Makefile
lib/Makefile
docs/reference/libmonothek/Makefile
m4/Makefile
po/Makefile.in])

AC_OUTPUT
