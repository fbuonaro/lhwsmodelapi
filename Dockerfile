##################################################################################
# STAGE 0 - base environment with first set of runtime dependencies
##################################################################################
FROM centos:centos7 as lhwsmodelapi-s0-base-env
LABEL lhwsmodelapi-stage-base-env="yes"
LABEL lhwsmodelapi-stage-build-env="no"
LABEL lhwsmodelapi-stage-build="no"
LABEL lhwsmodelapi-stage-test-env="no"
LABEL lhwsmodelapi-stage-main="no"

# okay repo is for gtest/gmock 1.8, 1.6 is permanently broken
RUN yum -y --enablerepo=extras install epel-release && \
    yum -y install https://repo.ius.io/ius-release-el7.rpm && \
    yum -y install http://repo.okay.com.mx/centos/7/x86_64/release/okay-release-1-5.el7.noarch.rpm && \
    yum clean all

##################################################################################
# STAGE 1 - build tools and libraries needed to build lhwsmodelapi
##################################################################################
FROM lhwsmodelapi-s0-base-env as lhwsmodelapi-s1-build-env
LABEL lhwsmodelapi-stage-base-env="no"
LABEL lhwsmodelapi-stage-build-env="yes"
LABEL lhwsmodelapi-stage-build="no"
LABEL lhwsmodelapi-stage-test-env="no"
LABEL lhwsmodelapi-stage-main="no"

# for compiling and unit testing
# x86_64 versions from okay repo for gmock/gtest 1.8
# zlib/pcre devel for cppcms 1.2.1
RUN yum -y install \
        cmake3 \
        gcc \
        gcc-c++ \
        gtest-devel.x86_64 \
        gmock-devel.x86_64 \
        wget \
        bzip2 \
        python36u \
        zlib-devel \ 
        pcre-devel \
        git \
        openssl-devel \
        curl-devel \
        rapidjson-devel \
        boost169-devel \
        make && \
    yum clean all

RUN yum -y update ca-certificates && \
    mkdir /cppcms && \
    cd /cppcms && \
    wget https://sourceforge.net/projects/cppcms/files/cppcms/1.2.1/cppcms-1.2.1.tar.bz2 && \
    tar -xf cppcms-1.2.1.tar.bz2 && \
    cd cppcms-1.2.1 && \
    mkdir build && \
    cd build && \
    cmake3 \
        -DCMAKE_INSTALL_PREFIX=/usr \
        ../ && \
    make && \
    make test || true && \
    make install

# it uses a different ssl1.1.1 by default, build manually to use 1.0.2
RUN yum install -y jansson-devel check-devel libtool automake autoconf perl-Thread-Queue.noarch && \
    git clone https://github.com/benmcollins/libjwt.git && \
    cd libjwt && \
    autoreconf -i && \
    ./configure --prefix=/usr && \
    make && \
    make check && \
    make install

RUN git clone https://github.com/fbuonaro/lhmiscutil.git && \
    cd lhmiscutil && \
    mkdir build && \
    cd build && \
    cmake3 \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        ../ && \
    make && \
    make test && \
    make install-lhmiscutil

RUN git clone https://github.com/fbuonaro/lhwsutil.git && \
    cd lhwsutil && \
    mkdir build && \
    cd build && \
    cmake3 \
        -DBOOST_INCLUDEDIR=/usr/include/boost169 \
        -DBOOST_LIBRARYDIR=/usr/lib64/boost169 \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        ../ && \
    make && \
    # make test && \
    make install-lhwsutil

RUN git clone https://github.com/fbuonaro/lhmodel.git /lhmodel && \
    cd /lhmodel && \
    mkdir ./build && \
    cd ./build && \
    cmake3 \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        ../ && \
    make && \
    make test && \
    make install-lhmodel && \
    make install-lhmodelutil

RUN ldconfig

ENTRYPOINT [ "bash" ]

##################################################################################
# STAGE 2 - the lhwsmodelapi source and compiled binaries
##################################################################################
FROM lhwsmodelapi-s1-build-env as lhwsmodelapi-s2-build
LABEL lhwsmodelapi-stage-base-env="no"
LABEL lhwsmodelapi-stage-build-env="no"
LABEL lhwsmodelapi-stage-build="yes"
LABEL lhwsmodelapi-stage-test-env="no"
LABEL lhwsmodelapi-stage-main="no"

ADD . /lhwsmodelapi
RUN cd /lhwsmodelapi && \
    mkdir ./build && \
    cd ./build && \
    cmake3 \
        -DBOOST_INCLUDEDIR=/usr/include/boost169 \
        -DBOOST_LIBRARYDIR=/usr/lib64/boost169 \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release \
        ../ && \
    make && \
    make test

##################################################################################
# STAGE 3 - the base image with additional built runtime dependencies, lhwsmodelapi 
#           binaries and test binaries needed for running integration tests
#           includes everything from build-env
##################################################################################
FROM lhwsmodelapi-s2-build as lhwsmodelapi-s3-test-env
LABEL lhwsmodelapi-stage-base-env="no"
LABEL lhwsmodelapi-stage-build-env="no"
LABEL lhwsmodelapi-stage-build="no"
LABEL lhwsmodelapi-stage-test-env="yes"
LABEL lhwsmodelapi-stage-main="no"

RUN cd /lhwsmodelapi/build && \
    make install
RUN ldconfig

##################################################################################
# STAGE 4 - the base image with additional built runtime dependencies and 
#           lhwsmodelapi binaries includes nothing from build-env
##################################################################################
FROM lhwsmodelapi-s0-base-env as lhwsmodelapi-s4-main-env
LABEL lhwsmodelapi-stage-base-env="no"
LABEL lhwsmodelapi-stage-build-env="no"
LABEL lhwsmodelapi-stage-build="no"
LABEL lhwsmodelapi-stage-test-env="no"
LABEL lhwsmodelapi-stage-main="yes"

COPY --from=lhwsmodelapi-s2-build /usr/ /usr/
COPY --from=lhwsmodelapi-s2-build /lhwsmodelapi/ /lhwsmodelapi/
RUN cd /lhwsmodelapi/build && \
    make install && \
    cd / && \
    rm -rf /lhwsmodelapi && \
    rm -rf /lhmiscutil && \
    rm -rf /lhwsutil && \
    rm -rf /lhmodel && \
    rm -rf /libjwt
