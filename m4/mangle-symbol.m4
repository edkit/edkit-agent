AC_DEFUN([MANGLE_SYMBOL],
[
    AC_REQUIRE([AC_PROG_CXX])
    AC_SUBST([$3], [$(echo -e "#include <$1>\n $2 {} " | $CXX -x c++ -S - -o- | grep "^_.*:$" | sed -e 's/:$//')])
])
