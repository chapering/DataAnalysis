#!/bin/bash

pushd . >/dev/null
cd $G && cvs co common/build common/utility common/gg common/mri common/libcurvecollection
cd $G/common/utility/nag && make install
cd $G/common/utility/port && make all && make install
cd $G/common/gg && make all && make install
cd $G/common/mri && make all && make install
cd $G/common/libcurvecollection && make all && make install

cd $G && cvs co project/brain/pipeline
cd $G/project/brain/pipeline && make all && make install

cd $G
mkdir -p bin
common/build/kit/linkbins.py $G/install_linux/bin bin

popd > /dev/null

