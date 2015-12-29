#!/bin/bash

:<< instruction
 simple a batch invocation of the underlying converter, currently always
 nifit2mriimage by default

 assumption:
 . $3 is the bvalue file containing lines in the number of volumes
   a single line per direction
 . $4 is the bvector file containing lines of the number of volumes,
   a single consisting of 3 whitespace sparated numbers per line
 . $5, an optional argument, is the number of gradient directions, i.e. the #volume - 1,
   the  default is 12
instruction

if [ $# -lt 4 ];
then
	echo "Fatal: too few arguments."
	echo -e "Usage: $0 <srcdir> <dstdir> <bval file> <bvec file> [#volume]\n"
    exit 1
fi

srcdir=$1
dstdir=$2
fbval=$3
fbvec=$4
nvol=${5:-12}

if ! test -d "$srcdir";
then
	echo "$srcdir" not found
	exit 1
fi

engine="nifti2mriimage"
fengine=`which $engine`

FDbval=7
FDbvec=8

exec 7<$fbval
exec 8<$fbvec

let nlines=nvol+1
echo "$nlines lines will be read both in $fbval and $fbvec."

for ((i=0;i<$nlines;++i))
do
	echo "processing on $srcdir/b$i ...."
	if ! test -s "$srcdir"/b$i.nii;
	then
		echo "$srcdir"/b$i.nii not found, bailed out now...
		exit 2
	fi

	: ' read a single line as a the No.$i b bvalue '
	if ! read curbval <&$FDbval; then
		echo "Fatal : reading line No.$i aborted."
		exit 3
	fi

	: ' read a single line as a the No.$i b bvector'
	if ! read curbvec <&$FDbvec; then
		echo "Fatal : reading line No.$i aborted."
		exit 4
	fi

	mkdir -p "$dstdir"/$i
	cmd="$fengine -i $srcdir/b$i.nii -vol 1 -o $dstdir/$i -b $curbval -bv $curbvec -v"
	echo $cmd
	$cmd

done


echo "finished."
exit 0


