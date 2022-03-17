cmake_minimum_required( VERSION 3.10 )

project( liblhwsmodelapi )

# general flags for building successfully
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK" )

# for debugging
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -Wall -Wextra -W" )
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -DDEBUG" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${LLHC_CXX_FLAGS_DBG}" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${LLHC_CXX_FLAGS_DBG}" )

##############################################################
# library
##############################################################

set( LIB_SRC_DIR "src" )
set( LIB_INC_DIR "include" )

# pull in gtest
find_package( GTest REQUIRED )
if( NOT GTEST_FOUND )
    message( FATAL_ERROR "gtest could not be found" )
endif( NOT GTEST_FOUND )

# pull in boost log
find_package( Boost 1.69 COMPONENTS log REQUIRED )
if( NOT Boost_FOUND )
    message( FATAL_ERROR "boost log could not be found" )
endif( NOT Boost_FOUND )

add_library( lhwsmodelapi SHARED
    "${LIB_SRC_DIR}/apicontext.cxx"
    "${LIB_SRC_DIR}/apierror.cxx"
    "${LIB_SRC_DIR}/apiexception.cxx"
    "${LIB_SRC_DIR}/iapicontext.cxx" )

target_link_libraries( lhwsmodelapi
    PUBLIC
        "${Boost_LIBRARIES}"
        lhmiscutil
        lhmodel
        lhmodelutil
        lhwsutil
        cppcms )

target_include_directories( lhwsmodelapi
    PUBLIC
        "${LIB_INC_DIR}/lhwsmodelapi"
        "${Boost_INCLUDE_DIR}"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}" 
        "${LIB_INC_DIR}/lhwsmodelapi_impl" )

##############################################################
# unit tests
##############################################################

set( TST_SRC_DIR "./test" )

enable_testing()

add_executable( testlhwsmodelapi
    "${TST_SRC_DIR}/testlhwsmodelapi.cxx" )

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
        "${LIB_INC_DIR}/lhwsmodelapi_impl" )

include( GoogleTest )
gtest_add_tests( TARGET testlhwsmodelapi )

##############################################################
# installation
##############################################################

# library
install( TARGETS lhwsmodelapi
         LIBRARY
            DESTINATION lib
            COMPONENT lhwsmodelapi )

# include directory
install( DIRECTORY "${LIB_INC_DIR}/lhwsmodelapi/lhwsmodelapi"
         DESTINATION include
         COMPONENT lhwsmodelapi
         FILES_MATCHING PATTERN "*.h" )

ADD_CUSTOM_TARGET( install-lhwsmodelapi
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=lhwsmodelapi"
                   -P "cmake_install.cmake" )
