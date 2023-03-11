#!/bin/bash

TARGET_STAGE="$1"
if [[ "x${TARGET_STAGE}" == "x" ]];
then
    TARGET_STAGE="dist"
fi

./modules/lhscriptutil/scripts/buildLHDeps.sh

# libcppcms
./scripts/buildLibCppcmsLHDistImage.sh

./modules/lhscriptutil/scripts/buildImage.sh ./Dockerfiles/Dockerfile.lhwsmodelapi lhwsmodelapi "${TARGET_STAGE}"