dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(crack-attack,1.1.14,lorien420@myrealbox.com)
AC_CONFIG_SRCDIR(src/Attack.cxx)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(src/config.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_CANONICAL_HOST 
AC_CANONICAL_TARGET

dnl Checks for libraries.
# FIXME: Replace `main' with a function in `-lGL':
AC_CHECK_LIB([GL], [glBegin])
# FIXME: Replace `main' with a function in `-lGLU':
AC_CHECK_LIB([GLU], [gluLookAt])
# I don't know how to check these.
# FIXME: Replace `main' with a function in `-lX11':
#AC_CHECK_LIB([X11], [main])
# FIXME: Replace `main' with a function in `-lXi':
#AC_CHECK_LIB([Xi], [main])
# FIXME: Replace `main' with a function in `-lXmu':
#AC_CHECK_LIB([Xmu], [main])
# FIXME: Replace `main' with a function in `-lglut':
AC_CHECK_LIB([glut], [glutInit])

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h string.h sys/socket.h unistd.h])

dnl BinReloc source additions
AM_BINRELOC
AM_CONDITIONAL([WANT_BINRELOC], [test "$br_cv_binreloc" = "yes"])

dnl Check for gtk
AC_ARG_ENABLE(gtk,
  AC_HELP_STRING([--enable-gtk=[yes/no]],[Use gtk front-end [default=yes]]),
  enable_gtk="$enableval",
  enable_gtk=yes)
if test "$enable_gtk" = "yes"; then
  pkg_modules="gtk+-2.0 >= 2.0.0"
  PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
  AC_DEFINE([WANT_GTK], [], [Wants to compile gtk front-end])
else
  PACKAGE_CFLAGS=""
  PACKAGE_LIBS=""
fi

AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)
AM_CONDITIONAL([WANT_GTK], [test "$enable_gtk" = yes])

AC_ARG_ENABLE(gamepad,
  AC_HELP_STRING([--enable-gamepad=[yes/no]],
    [Enables gamepad support [default=yes]]),
  enable_gamepad="$enableval",
  enable_gamepad=yes)
if test "$enable_gamepad" = "yes"; then
  AC_DEFINE([WANT_GAMEPAD], [], [Wants to compile gamepad support])
fi
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)
AM_CONDITIONAL([WANT_GAMEPAD], [test "$enable_gamepad" = yes])


dnl Check for sound support
AC_ARG_ENABLE(sound,
  AC_HELP_STRING([--enable-sound=[yes/no]],
    [Play sounds and music during game [default=yes]]),
  enable_sound="$enableval",
  enable_sound="no")
AUDIO_CFLAGS=""
AUDIO_LIBS=""
if test "$enable_sound" != "no"; then
  found_sound="no"
  AM_PATH_SDL(1.2.0,
    [
      AC_CHECK_LIB([SDL_mixer],
        [Mix_OpenAudio],
        [found_sound=yes],
        [found_sound=no]
      )
    ],[found_sound=no]
  )

  if test "$enable_sound" == "yes" -a "$found_sound" == "no"; then
    AC_MSG_ERROR(libSDL and SDL_mixer are required for sound)
  fi
  if test "$found_sound" == "yes"; then
    AUDIO_CFLAGS="$SDL_CFLAGS"
    AUDIO_LIBS="$SDL_LIBS -lSDL_mixer"
    AC_DEFINE(AUDIO_ENABLED, 1, [Has audio support])
  fi
fi
AC_SUBST(AUDIO_CFLAGS)
AC_SUBST(AUDIO_LIBS)
AM_CONDITIONAL([AUDIO_ENABLED], [test "$found_sound" = "yes"])


dnl Check for debugging
AC_ARG_ENABLE(debug,
  AC_HELP_STRING([--enable-debug=[no/yes]],[Compile extra debugging info [default=no]]),
  enable_debug="$enableval",
  enable_debug=no)
if test "$enable_debug" = "yes"; then
  DEBUG_INCLUDES=-DDEVELOPMENT
  DEBUG_CFLAGS="-g -Wall -pedantic"
else
  DEBUG_INCLUDES=-DNDEBUG
  DEBUG_CFLAGS=
fi
AC_SUBST(DEBUG_INCLUDES)
AC_SUBST(DEBUG_CFLAGS)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T

AC_PATH_XTRA

dnl Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MEMCMP
AC_FUNC_STAT
AC_CHECK_FUNCS([atexit gethostbyname inet_ntoa mkdir pow socket sqrt strchr strcspn strstr poll])

AC_CONFIG_FILES(
  Makefile 
  src/Makefile 
  src/gtk-gui/Makefile
  data/Makefile 
  doc/Makefile
  autopackage/default.apspec)

AC_OUTPUT
