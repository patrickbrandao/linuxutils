#!/bin/sh


	USRDIR="$1"
	if [ "x$USRDIR" = "x" ]; then USRDIR="/usr"; fi

	TMPDST="$2"
	if [ "x$TMPDST" = "x" ]; then TMPDST="/tmp/linuxutils"; fi

	echo "> Compilando"
	mkdir -p $TMPDST
	mkdir -p $USRDIR
	mkdir -p $TMPDST$USRDIR/bin

	# cada .c gera um binario com o proprio nome
	for i in *.c; do
		bin=$(basename $i .c)
		tmpbin="/tmp/_$bin"
		gcc $i -o $tmpbin
		strip --strip-unneeded $tmpbin
		cp -rav $tmpbin $TMPDST$USRDIR/bin/$bin
		cp -rav $tmpbin $USRDIR/bin/$bin
	done

