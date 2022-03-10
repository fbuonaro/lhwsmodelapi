#!/bin/bash

DOCKERFILE_PATH="./Dockerfile"
TEST_STAGE="lhwsmodelapi-s3-test-env"
TEST_TAG="lhwsmodelapi:test-env"
MAIN_STAGE="lhwsmodelapi-s4-main-env"
MAIN_TAG="lhwsmodelapi:main"
TARGET_STAGE=${MAIN_STAGE}
TARGET_TAG=${MAIN_TAG}
BASE_STAGE="lhwsmodelapi-stage-base-env"

# Build the main image
docker build --target ${TARGET_STAGE} -t ${TARGET_TAG} -f ${DOCKERFILE_PATH} . || exit 1
