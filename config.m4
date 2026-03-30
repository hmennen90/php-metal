dnl config.m4 for extension metal

PHP_ARG_ENABLE([metal],
  [whether to enable Metal GPU support],
  [AS_HELP_STRING([--enable-metal],
    [Enable Metal GPU compute support (macOS only)])])

if test "$PHP_METAL" != "no"; then
  case $host_os in
    darwin*)
      ;;
    *)
      AC_MSG_ERROR([php-metal requires macOS with Metal GPU support])
      ;;
  esac

  METAL_SHARED_LIBADD="-framework Metal -framework Foundation"
  PHP_SUBST(METAL_SHARED_LIBADD)

  PHP_NEW_EXTENSION(metal, [metal.c], $ext_shared,, [-fobjc-arc -xobjective-c])
fi
