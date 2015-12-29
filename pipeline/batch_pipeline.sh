#!/bin/bash

# prompt usage if it is not correct
if [ $# -lt 2 ];then
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
defaultgen=${5:-1}

# /////////////////////////////////////////////////////////////////////////////
# remove possible affine transformation
if [ $bnch -ge 1 ];then
	nifti_tool -mod_hdr -prefix dnew$name -infiles $name.nii -mod_field \
	srow_x "0.0 0.0 0.0 0.0" -mod_field srow_y "0.0 0.0 0.0 0.0" -mod_field \
	srow_z "0.0 0.0 0.0 0.0" -mod_field sform_code "0" -mod_field qform_code "0" \
	-mod_field qoffset_x 0 -mod_field qoffset_y 0 -mod_field qoffset_z 0 \
	-mod_field quatern_b 0 -mod_field quatern_c 0 -mod_field quatern_d 0

	mv $name.nii $name.nii.org && mv dnew$name.nii $name.nii
fi

# /////////////////////////////////////////////////////////////////////////////
# Convert a NIfTI-1 (.nii) file to MriImage.
if [ $single -ge 1 ];then
	nohup nifti2mriimage -i $name.nii -o $name.MRI -vol 4 -b 1000.0 -bv 0.0 0.0 0.0 \
	 1>$name.nohup.out 2>&1 < /dev/null &
# or Convert a group of NIfTI (.nii) files to MriImages.
else
	mkdir -p $name.MRI && nohup nifti2mriimage.batch.py $name b*.nii $name.MRI   \
	 $name/*.bval $name/*.bvec \
	 1>$name.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
# resize volume using hamming filter
if [ $bresz -ge 1 ];then
	mrifilt3 -i $name.MRI -o $name.MRI.filtered -x -32767 32767 -y -32767 32767 -z -32767 32767
	 1>> $name.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
# calculate diffusion tensor from multiple datasets
# fit volume image of a single gradient direction
if [ $single -ge 1 ];then
	nohup mridfit -i $name.MRI -D -oD $name.dt -T2 -oT2 $name.t2  \
	 1>> $name.nohup.out 2>&1 < /dev/null &
else
	nohup mridfit.auto.py $name.MRI -D -oD $name.dt -T2 -oT2 $name.t2 \
	 1>> $name.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
# Generate a set of streamtubes.
if [ $defaultgen -ge 1 ];then
	nohup tubegen -out $name.sm -dt $name.dt -t2 $name.t2 
		 1>> $name.nohup.out 2>&1 < /dev/null &
else # with extremely dense sampling
	nohup tubegen -out $name.sm -dt $name.dt -t2 $name.t2 \
		-ODE 0 -cs 10 \
		-seed 1 1 1 -stepsize 1.0 -radius 0.3 -al 0.1 -dth .5 -t2thresh \
		-40 -Longshort 1 -cl .1 -cd 1.7 \
		 1>> $name.nohup.out 2>&1 < /dev/null &
fi
wait

# /////////////////////////////////////////////////////////////////////////////
echo "all finished!"
exit 1




