# Prelude
AC_PREREQ(2.4)
AC_INIT([autotools], [0.1], [blottiere.paul@gmail.com])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([foreign])
AM_PROG_AR
AM_CONFIG_HEADER([config.h])

# libtool init
LT_INIT
AC_ENABLE_SHARED

# check headers
AC_CHECK_HEADER(sys/socket.h, [],
                [AC_MSG_ERROR(["Couldn't find sys/socket.h"])])


# output files
AC_CONFIG_FILES([Makefile])

AC_OUTPUT

AC_MSG_NOTICE([

------------------------------------------------------------------------
  $PACKAGE $VERSION - Configuration Report
------------------------------------------------------------------------

 prefix:            $prefix
 exec-prefix:       $exec_prefix
 bin path:          $bindir
 lib path:          $libdir
 include path:      $includedir
 etc path:          $sysconfdir
 data path:         $datarootdir
])

