#!/bin/bash

# prompt usage if the provided is not correct
if [ $# -lt 1 ];then
	echo "Usage: $0 <source-dir>"
	echo "			[int - number of slices in each volume default 30]"
	echo "			[int - number of volumes default 13]"
	echo "			[1/0 - use default arguments for tubegen with default 0]"
	echo "			[1/0 - need resize after coversion with default 1]"
	echo "			[int int int - resolution resized to in voxel default 160 220 60]"
	exit 1
fi

# compulsory source directory including volume images in the NIfTI format (.nii)
name=$1

if [ ! -d $name ];
then
	echo "FATAL: ${name} not found."
	exit 1	
fi

# other optional arguments with defaults
nslice=${2:-30}
nvol=${3:-13}
defaultgen=${4:-0}
bresz=${5:-0}
size="${6:-320} ${7:-440} ${8:-60}"

stime=`date +%s`

# /////////////////////////////////////////////////////////////////////////////
# Dispatch directory structure preferred
mkdir -p ${name}/{original,dwi,dwi/{original,resized},tensors,I0,tube} || exit 1
mv -f ${name}/*.nii ${name}/original || exit 1

> ${name}/${name}.nohup.out 
# /////////////////////////////////////////////////////////////////////////////
# Convert NII images to MriImages
nohup nifti2mriimage.batch.py ${name}/original b*.nii \
	 ${name}/dwi/original ${name}/bvals ${name}/bvecs \
	 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
wait

# /////////////////////////////////////////////////////////////////////////////
# resize volume in order to try making unit voxel size
if [ $bresz -ge 1 ];then
nohup dwiResize.py ${name}/dwi/original/ ${name}/dwi/resized/ ${size} ${size} \
	 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
# calculate diffusion tensor from multiple volumes - one volume per gradient dir 
if [ $bresz -ge 1 ];then
	nohup mridfit.auto.py ${name}/dwi/resized -D -oD ${name}/tensors \
		-oI0 ${name}/I0 -v -zn \
	 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
 else
	nohup mridfit.auto.py ${name}/dwi/original -D -oD ${name}/tensors \
		-oI0 ${name}/I0 -v -zn \
	 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
 fi
wait

# /////////////////////////////////////////////////////////////////////////////
# Generate a set of streamtubes.
if [ $defaultgen -ge 1 ];then
	nohup tubegen -out ${name}/tube/${name}.sm -dt ${name}/tensors -t2 ${name}/I0 \
		 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
else # with extremely dense sampling
	nohup tubegen -out ${name}/tube/${name}.sm -dt ${name}/tensors -t2 ${name}/I0 \
		-ODE 0 -cs 10 \
		-seed 1 1 1 -stepsize 1.0 -radius 0.1 -al 0.1 -dth .5 -t2thresh \
		-40 -Longshort 1 -cl .1 -cd 1.7 \
		 1>> ${name}/${name}.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
etime=`date +%s`
echo "all finished! time cost `expr $etime - $stime`"
exit 0




