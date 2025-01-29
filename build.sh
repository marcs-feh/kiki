#!/usr/bin/env sh

compiler=clang
cflags='-DTARGET_OS_LINUX -fPIC -fno-strict-aliasing -Wall -Wextra -Wno-unused-label -Isrc'

linker=clang
ldflags=''

archiver=ar
arflags='rcs'

Compile(){
	out="$(basename $1 | sed 's/\.c$/.o/')"
	echo "* Compile: $compiler $cflags -o $out -c $1"
	$compiler $cflags -o "$out" -c "$1"
}

Link(){
	out="$1"; shift
	echo "*    Link: $linker -o $out $@ $ldflags"
	$linker -o $out "$@" $ldflags
}

Lib(){
	out="$(basename $1)"; shift
	echo "*     Lib: $archiver $arflags "$out" $@"
	$archiver $arflags "$out" $@
}

Run(){
	echo "*     Run: $@"
	$@
}

BuildAll(){
	Compile 'src/base/base.c'
	Compile 'src/kiki.c'
	Compile 'src/driver.c'
	wait
	Lib 'kiki.a' 'kiki.o' 'base.o'
	Link 'kiki' 'driver.o' 'kiki.a'
}

mode="$1"
[ -z "$mode" ] &&
	mode=debug

set -eu

echo "Build mode: $mode"
case "$mode" in
	"debug")
		cflags="-O1 -g $cflags"
		BuildAll
		Run ./kiki
	;;
	"release")
		cflags="-Os $cflags"
		BuildAll
		Run strip ./kiki
		Run ./kiki
	;;
esac

