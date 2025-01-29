#!/usr/bin/env sh

compiler=clang
cflags='-DTARGET_OS_LINUX -fPIC -fno-strict-aliasing -Wall -Wextra -Wno-unused-label'

linker=clang
ldflags=''

Compile(){
	out="$(basename $1 | sed 's/\.c$/.o/')"
	echo "> Compile: $out"
	$compiler $cflags -o "$out" -c "$1"
}

Link(){
	out="$1"; shift
	echo ">    Link: $out"
	$linker -o $out "$@" $ldflags
}

Run(){
	echo ">    Run: $@"
	$@
}

BuildAll(){
	Compile 'src/base/base.c'
	Compile 'src/kiki.c'
	Compile 'src/driver.c'
	wait
	Link 'kiki' 'driver.o' 'kiki.o' 'base.o'
	Run ./kiki
}

mode="$1"
[ -z "$mode" ] &&
	mode=debug

set -eu


echo "Build mode: $mode"
case "$mode" in
	"debug")
		cflags="-O0 -g $cflags"
		BuildAll
	;;
	"release") ;;
esac

