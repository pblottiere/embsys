#! /bin/sh

mkdir -p config m4
echo "Running aclocal..." ; aclocal -I m4 --install || exit 1
echo "Running libtoolize..." ; libtoolize || exit 1
echo "Running autoheader..." ; autoheader || exit 1
echo "Running autoconf..." ; autoconf || exit 1
echo "Running automake..." ; automake --add-missing || exit 1
