AC_PREREQ(2.53)
AC_INIT([matchbox-desktop], 0.8, [mallum@handhelds.org])
AC_CONFIG_SRCDIR([src/mbdesktop.c])

AM_INIT_AUTOMAKE()
AM_MAINTAINER_MODE
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_VOLATILE

# Checks for library functions.
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_CHECK_FUNCS([alarm memset mkdir strchr strdup])

AC_ARG_ENABLE(debug,
  [  --enable-debug    enable debug ( verbose ) build],
     enable_debug=$enableval, enable_debug=no )

PKG_CHECK_MODULES(LIBMB, libmb >= 1.1,,
	         AC_MSG_ERROR([*** Required Matchbox Library (libmb) not installed ***]))

dnl ------ Debug Build ------------------------------------------------------

if test x$enable_debug = xyes; then
      LIBMB_CFLAGS="$LIBMB_CFLAGS -DDEBUG"
fi

dnl ----- Xsettings ---------------------------------------------------------

AC_MSG_CHECKING([for libmb xsettings support])

if $PKG_CONFIG --libs libmb | grep -i xsettings ; then
     mb_have_xsettings="yes"
     AC_DEFINE(USE_XSETTINGS, [1], [Use XSettings])
     AC_MSG_RESULT([yes])
else
     mb_have_xsettings="no"	
     AC_MSG_RESULT([no])
fi

dnl ------ GCC flags --------------------------------------------------------

if test "x$GCC" = "xyes"; then
        GCC_WARNINGS="-g -Wall -fno-strict-aliasing"
        LIBMB_CFLAGS="$GCC_WARNINGS $LIBMB_CFLAGS"
fi


dnl ------ Substitute in found libs, clags to Makefiles etc -----------------

AC_SUBST(LIBMB_CFLAGS)
AC_SUBST(LIBMB_LIBS)

AC_OUTPUT([
Makefile
matchbox-desktop.pc
src/Makefile
modules/Makefile
data/Makefile
data/mbdesktop_modules
])

