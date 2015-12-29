#!/bin/bash

# prompt usage if it is not correct
if [ $# -lt 1 ];then
	echo "Usage: $0 <source-dir>"
	echo "			[1/0 need affine/trans correction with default 0]"
	echo "			[1/0 only convert single DWI with default 0]"
	echo "			[1/0 need resize after coversion with default 0]"
	echo "			[1/0 use default arguments for tubegen with default 1]"
	exit 0
fi

# compulsory source directory including NII volume images
name=$1

# other optional arguments with defaults
bnch=${2:-0}
single=${3:-0}
bresz=${4:-0}
defaultgen=${5:-0}

# /////////////////////////////////////////////////////////////////////////////
# calculate diffusion tensor from multiple datasets
# fit volume image of a single gradient direction
if [ $single -ge 1 ];then
	nohup mridfit -i $name.Resized -D -oD $name.tensors -oI0 $name.I0 -v -zn \
	 1>> $name.nohup.out 2>&1 < /dev/null &
else
	nohup mridfit.auto.py $name.Resized -D -oD $name.tensors -oI0 $name.I0 -v -zn \
	 1>> $name.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
# Generate a set of streamtubes.
if [ $defaultgen -ge 1 ];then
	nohup tubegen -out $name.sm -dt $name.tensors -t2 $name.I0 
		 1>> $name.nohup.out 2>&1 < /dev/null &
else # with extremely dense sampling
	nohup tubegen -out $name.sm -dt $name.tensors -t2 $name.I0 \
		-ODE 0 -cs 10 \
		-seed 1 1 1 -stepsize 1.0 -radius 0.3 -al 0.1 -dth .5 -t2thresh \
		-40 -Longshort 1 -cl .1 -cd 1.7 \
		 1>> $name.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
echo "all finished!"
exit 1




