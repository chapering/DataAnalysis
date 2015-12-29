#!/bin/bash

# prompt usage if the provided is not correct
if [ $# -lt 1 ];then
	echo "Usage: $0 <source-dir>"
	echo "			[int - number of slices in each volume default 30]"
	echo "			[int - number of volumes default 13]"
	echo "			[1/0 - need resize after coversion with default 1]"
	echo "			[1/0 - need DWI flipping or permuatation with default 0]"
	echo "			[int int int - resolution resized to in voxel default 160 220 60]"
	echo "			[\$@ - directly forwarded to mritransp default is -xy]"
	exit 1
fi

# compulsory parameter giving source directory including MRI volume images in DICOM
# format, should be the .dcm standard rather than RAW, DICOM files
name=$1

if [ ! -d $name ];
then
	echo "FATAL: ${name} not found."
	exit 1	
fi

# other optional arguments with defaults
nslice=${2:-30}
nvol=${3:-13}
defaultgen=0
bresz=${4:-1}
bdwitrans=${5:-0}
size="${6:-256} ${7:-256} ${8:-30}"
dwitransparas=${9:-"-xy"}

> ${name}/${name}.nohup.out 
stime=`date +%s`

# /////////////////////////////////////////////////////////////////////////////
# Dispatch directory structure preferred
echo -n "1. trimming up original dicom .. "
if ! test -d ${name}/original;then
	mkdir -p ${name}/original
	mv -f ${name}/* ${name}/original || exit 1
	echo "finished."
else
	echo "skipped."
fi

# /////////////////////////////////////////////////////////////////////////////
# Convert DICOM images to MriImages
echo -n "2. conversion from input into Mriimages .. "
if ! test -d ${name}/dwi/original;then
	mkdir -p ${name}/dwi/original
	nohup matlabLauncher.py $G/bin/dicomToMriImage_batch.m \
		 ${name}/original ${name}/dwi/original $nslice $nvol \
		 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
	wait
	echo "finished."
else
	echo "skipped."
fi

# /////////////////////////////////////////////////////////////////////////////
# resize volume in order to try making unit voxel size
echo -n "3. Resizing DWIs .. "
if ! test -d ${name}/dwi/resized;then
	if [ $bresz -ge 1 ];then
		mkdir -p ${name}/dwi/resized
		nohup dwiResize.py ${name}/dwi/original/ ${name}/dwi/resized/ $size $size \
			 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
	fi
	wait
	echo "finished."
else
	echo "skipped."
fi

# /////////////////////////////////////////////////////////////////////////////
# transpose or flip dwi before using it for tensor fitting
echo -n "4. DWI transformation .. "
if [ $bdwitrans -ge 1 -a ! -d ${name}/dwi/flipped ];then
	for ((j=0;j<$nvol;++j));
	do
		mkdir -p ${name}/dwi/flipped/$j
		if [ $bresz -ge 1 ];then
			mritransp -i ${name}/dwi/resized/$j -o ${name}/dwi/flipped/$j \
			"$dwitransparas" -v \
			 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
		else
			mritransp -i ${name}/dwi/original/$j -o ${name}/dwi/flipped/$j \
			"$dwitransparas" -v \
			 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
		fi
	done
	wait
	echo "finished."
else
	echo "skipped."
fi

# /////////////////////////////////////////////////////////////////////////////
# calculate diffusion tensor from multiple volumes - one volume per gradient dir 
echo -n "5. Tensor fitting .. "
if ! test -d ${name}/tensors || ! test -d ${name}/I0;then
	mkdir -p ${name}/{tensors,I0}
	if [ $bdwitrans -ge 1 ];then
		nohup mridfit.auto.py ${name}/dwi/flipped -D -oD ${name}/tensors \
			-oI0 ${name}/I0 -v -zn \
		 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
	elif [ $bresz -ge 1 ];then
		nohup mridfit.auto.py ${name}/dwi/resized -D -oD ${name}/tensors \
			-oI0 ${name}/I0 -v -zn \
		 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
	else
		nohup mridfit.auto.py ${name}/dwi/original -D -oD ${name}/tensors \
			-oI0 ${name}/I0 -v -zn \
		 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
	 fi
	wait
	echo "finished."
else
	echo "skipped."
fi

# /////////////////////////////////////////////////////////////////////////////
# Generate a set of streamtubes.
echo -n "6. Fiber tracking .. "
if ! test -d ${name}/tube;then
	mkdir -p ${name}/tube 
	if [ $defaultgen -ge 1 ];then
		nohup tubegen -out ${name}/tube/${name}.sm -dt ${name}/tensors -t2 ${name}/I0 \
			 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
	else # with dense sampling
		for ((s=2;s<=5;s++))
		do
		echo "*************************       Seeds : $s x $s x $s *********************** " \
			 1>> ${name}/${name}.nohup.out 2>&1 
		
		nohup tubegen -out ${name}/tube/${name}_s${s}_1.sm -dt ${name}/tensors -t2 ${name}/I0 \
			-ODE 0 -cs 10 \
			-seed $s $s $s -stepsize 1.0 -radius 0.3 -al 0.1 -dth .5 -t2thresh \
			100 -Longshort 1 -cl .1 -cd 2.0 -cr 0 \
			 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &

			 wait

		nohup tubegen -out ${name}/tube/${name}_s${s}_2.sm -dt ${name}/tensors -t2 ${name}/I0 \
			-ODE 0 -cs 10 \
			-seed $s $s $s -stepsize 1.0 -radius 0.3 -al 0.1 -dth .25 -t2thresh \
			60 -Longshort 1 -cl .1 -cd 1.5 -cr 0 \
			 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
		 done
	fi
	wait
	echo "finished."
else
	echo "skipped."
fi

# /////////////////////////////////////////////////////////////////////////////
etime=`date +%s`
echo -e "All pipeline finished!\n Time cost: `expr $etime - $stime` seconds.\n"
exit 0

:'
ts=4 sts=4 sw=4 tw=80
'

