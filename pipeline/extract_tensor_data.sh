#!/bin/bash

name=$1

if [ ! -d $name ];
then
	echo "FATAL: ${name} not found."
	exit 1	
fi

> ${name}/${name}.nohup.out 
stime=`date +%s`

# /////////////////////////////////////////////////////////////////////////////
# Generate a set of streamtubes.
if ! test -d ${name}/tube;then
	mkdir -p ${name}/tube 

	for ((s=5;s>=1;s--))
	do
		echo "*************************       Seeds : $s x $s x $s *********************** " \
			 1>> ${name}/${name}.nohup.out 2>&1 
		
		nohup ~/tubegen_tensors-gcc4 -out ${name}/tube/${name}_s${s}.sm -dt ${name}/tensors -t2 ${name}/I0 \
			-ODE 0 -cs 10 \
			-seed $s $s $s -stepsize 1.0 -radius 0.3 -al 0.1 -dth .25 -t2thresh \
			60 -Longshort 1 -cl .1 -cd 1.5 -cr 0 -tensorinfo ${name}/tube/${name}_s${s}.tensorinfo \
			 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
		wait
	done

	echo "finished."
else
	echo "skipped."
fi

# /////////////////////////////////////////////////////////////////////////////
etime=`date +%s`
echo -e "All finished!\n Time cost: `expr $etime - $stime` seconds.\n"
exit 0

:'
ts=4 sts=4 sw=4 tw=80
'

