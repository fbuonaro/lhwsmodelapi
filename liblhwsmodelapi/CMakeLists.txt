cmake_minimum_required( VERSION 3.10 )

project( liblhwsmodelapi 
         DESCRIPTION "lhwsmodelapi library"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhwsmodelapi/liblhwsmodelapi"
         LANGUAGES CXX
         VERSION 1.0.0 )

##############################################################
# general
##############################################################

include( GNUInstallDirs )

set( CMAKE_CXX_STANDARD 11 )

# TODO - how to propagate boost dyn link from lhwsutil?
add_compile_options( -Wall -Wextra -pedantic -Werror -DBOOST_LOG_DYN_LINK)

##############################################################
# library
##############################################################

set( LH_LIB_NAME lhwsmodelapi )

# install directories
set( LH_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( LH_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhwsmodelapi_${PROJECT_VERSION}" )
set( LH_INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhwsmodelapi_${PROJECT_VERSION}" )

# pull in liblhmodel
find_package( liblhmodel REQUIRED )
# pull in liblhmodelutil
find_package( liblhmodelutil REQUIRED )
# pull in liblhmiscutil
find_package( liblhmiscutil REQUIRED )
# pull in liblhwsutil
find_package( liblhwsutil REQUIRED )

find_package( Boost 1.69 COMPONENTS log REQUIRED )

# TODO find_package cppcms

# source files
set( LH_LIB_SRC_FILES 
    "src/apicontext.cxx"
    "src/apierror.cxx"
    "src/apiexception.cxx"
    "src/iapicontext.cxx" )

# library dependencies
set( LH_LIB_PUBLIC_LINKLIBS 
    "${Boost_LIBRARIES}"
     liblhmodel::lhmodel 
     liblhmodelutil::lhmodelutil
     liblhwsutil::lhwsutil 
     liblhmiscutil::lhmiscutil 
     cppcms )
set( LH_LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LH_LIB_PUBLIC_INCLUDES 
    "${Boost_INCLUDE_DIR}"
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhwsmodelapi>   # build
     $<INSTALL_INTERFACE:${LH_INSTALL_INCDIR}> )                               # export
set( LH_LIB_PRIVATE_INCLUDES 
     "${CMAKE_CURRENT_SOURCE_DIR}/include/lhwsmodelapi_impl"
     "${CMAKE_CURRENT_BINARY_DIR}" )

lh_add_library()

##############################################################
# unit tests
##############################################################

# pull in gtest
find_package( GTest REQUIRED )

add_executable( testlhwsmodelapi
                "test/testlhwsmodelapi.cxx" )

target_link_libraries( testlhwsmodelapi
                       PRIVATE
                           "${GTEST_BOTH_LIBRARIES}"
                           gmock
                           pthread
                           lhwsmodelapi )

target_include_directories( testlhwsmodelapi
                            PRIVATE
                                "${CMAKE_CURRENT_BINARY_DIR}"
                                "${Boost_INCLUDE_DIR}"
                                "${GTEST_INCLUDE_DIRS}"
                                "${CMAKE_CURRENT_SOURCE_DIR}/include/lhwsmodelapi_impl" )

include( GoogleTest )
gtest_add_tests( TARGET testlhwsmodelapi )

##############################################################
# installation
##############################################################

# name of component to install
set(LH_COMPONENT_NAME liblhwsmodelapi )
# directories to install
set(LH_LIB_PUBLIC_INCLUDE_DIRS "include/lhwsmodelapi/lhwsmodelapi")

lh_add_install_library()

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${LH_INSTALL_INCDIR}")
set( PC_LIBDIR "${LH_INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS "-DBOOST_LOG_DYN_LINK")
set( PC_PUBLIBS "-llhwsmodelapi -llhmodelutil -llhmodel -llhmiscutil -llhwsutil -lcppcms" )
set( PC_PRIVLIBS )
set( PC_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig" )
set( PC_INSTALL_FILENAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION_MAJOR}.pc" )
set( PC_INSTALL_FILE "${PC_INSTALL_DIR}/${PC_INSTALL_FILENAME}" )
set( PC_RELOCATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh" )

lh_add_pkgconfig()

##############################################################
# cmake packaging
##############################################################

lh_add_install_cmake_config()

##############################################################
# rpm packaging
##############################################################
 
# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_LIBLHMODELDBTEST_PACKAGE_REQUIRES_POST "sed" PARENT_SCOPE) 
set( CPACK_RPM_LIBLHMODELDBTEST_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" PARENT_SCOPE)

set(LH_PACKAGE_DESC "liblhwsmodelapi libraries package")

lh_add_cpack_component()
