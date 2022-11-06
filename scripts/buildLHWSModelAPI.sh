#!/bin/bash

TARGET_STAGE="$1"
if [[ "x${TARGET_STAGE}" == "x" ]];
then
    TARGET_STAGE="dist"
fi

# lhmodel
pushd ./modules/lhmodel
./scripts/buildLHModelImage.sh
popd

# lhmiscutil
pushd ./modules/lhmiscutil
./scripts/buildLHMiscUtilImage.sh
popd

# lhwsutil
pushd ./modules/lhwsutil
./scripts/buildLHWSUtil.sh
popd

# libcppcms
./scripts/buildLibCppcmsLHDistImage.sh

./modules/lhscriptutil/scripts/buildImage.sh ./Dockerfiles/Dockerfile.lhwsmodelapi lhwsmodelapi "${TARGET_STAGE}"