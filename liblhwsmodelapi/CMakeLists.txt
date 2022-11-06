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

# install directories
set( INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhwsmodelapi_${PROJECT_VERSION}" )
set( INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhwsmodelapi_${PROJECT_VERSION}" )

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
set( LIB_SRC_FILES 
    "src/apicontext.cxx"
    "src/apierror.cxx"
    "src/apiexception.cxx"
    "src/iapicontext.cxx" )

# library dependencies
set( LIB_PUBLIC_LINKLIBS 
    "${Boost_LIBRARIES}"
     liblhmodel::lhmodel 
     liblhmodelutil::lhmodelutil
     liblhwsutil::lhwsutil 
     liblhmiscutil::lhmiscutil 
     cppcms )
set( LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LIB_PUBLIC_INCLUDES 
    "${Boost_INCLUDE_DIR}"
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhwsmodelapi>   # build
     $<INSTALL_INTERFACE:${INSTALL_INCDIR}> )                               # export
set( LIB_PRIVATE_INCLUDES 
     "${CMAKE_CURRENT_SOURCE_DIR}/include/lhwsmodelapi_impl"
     "${CMAKE_CURRENT_BINARY_DIR}" )

# boilerplate ..
# libs
add_library( lhwsmodelapi SHARED ${LIB_SRC_FILES} )
add_library( lhwsmodelapi_static STATIC ${LIB_SRC_FILES} )
# lib deps
target_link_libraries( lhwsmodelapi
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
target_link_libraries( lhwsmodelapi_static
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
# header deps
target_include_directories( lhwsmodelapi
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
target_include_directories( lhwsmodelapi_static
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
# properties
set_target_properties( lhwsmodelapi
                       PROPERTIES 
                           LIBRARY_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           SOVERSION ${PROJECT_VERSION}
                           VERSION ${PROJECT_VERSION}
                           CLEAN_DIRECT_OUTPUT 1)
set_target_properties( lhwsmodelapi_static 
                       PROPERTIES
                           ARCHIVE_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           OUTPUT_NAME lhwsmodelapi )
# .. boilerplate

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

# boilerplate ..
# install lib/bin
install( TARGETS lhwsmodelapi lhwsmodelapi_static
         EXPORT liblhwsmodelapi-targets 
         COMPONENT liblhwsmodelapi
         RUNTIME DESTINATION "${INSTALL_BINDIR}"
         COMPONENT liblhwsmodelapi
         LIBRARY DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhwsmodelapi
         ARCHIVE DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhwsmodelapi
         INCLUDES DESTINATION "${LIB_PUBLIC_INCLUDES}" )
# install header dir
install( DIRECTORY "include/lhwsmodelapi/lhwsmodelapi"
         DESTINATION "${INSTALL_INCDIR}"
         COMPONENT liblhwsmodelapi
         FILES_MATCHING PATTERN "*.h" )
# install export for things outside of build
install( EXPORT liblhwsmodelapi-targets 
         FILE liblhwsmodelapiTargets.cmake 
         NAMESPACE liblhwsmodelapi:: 
         DESTINATION "${INSTALL_LIBDIR}/cmake" 
         COMPONENT liblhwsmodelapi )
# export export for things in build
export( EXPORT liblhwsmodelapi-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/liblhwsmodelapiTargets.cmake 
    NAMESPACE liblhwsmodelapi:: )
ADD_CUSTOM_TARGET( install-lhwsmodelapi
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=liblhwsmodelapi"
                   -P "cmake_install.cmake" )
# .. boilerplate

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${INSTALL_INCDIR}")
set( PC_LIBDIR "${INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS "-DBOOST_LOG_DYN_LINK")
set( PC_PUBLIBS "-llhwsmodelapi -llhmodelutil -llhmodel -llhmiscutil -llhwsutil -lcppcms" )
set( PC_PRIVLIBS )
set( PC_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig" )
set( PC_INSTALL_FILENAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION_MAJOR}.pc" )
set( PC_INSTALL_FILE "${PC_INSTALL_DIR}/${PC_INSTALL_FILENAME}" )
set( PC_RELOCATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh" )

# boilerplate ..
# configure pc file
configure_file( "${CMAKE_SOURCE_DIR}/modules/lhscriptutil/cmake/pkgconfig/component.pc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/${PC_INSTALL_FILENAME}"
                @ONLY )
# install pc file
install( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PC_INSTALL_FILENAME}"
         DESTINATION "${PC_INSTALL_DIR}"
         COMPONENT liblhwsmodelapi )
# configure pc relocation script
configure_file( "${CMAKE_SOURCE_DIR}/modules/lhscriptutil/cmake/pkgconfig/rpm_relocate_pc.sh.in"
                "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh"
                @ONLY )
# .. boilerplate

##############################################################
# cmake packaging
##############################################################

# boilerplate ..
include(CMakePackageConfigHelpers)

configure_package_config_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                               INSTALL_DESTINATION "${INSTALL_LIBDIR}/cmake" )

write_basic_package_version_file( "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                                  VERSION "${PROJECT_VERSION}"
                                  COMPATIBILITY SameMajorVersion )

install( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
         DESTINATION "${INSTALL_LIBDIR}/cmake"
         COMPONENT liblhwsmodelapi )
# .. boilerplate

##############################################################
# rpm packaging
##############################################################
 
# boilerplate ..
set( CPACK_PROJECT_NAME ${PROJECT_NAME} )
set( CPACK_PROJECT_VERSION ${PROJECT_VERSION} )
set( CPACK_COMPONENTS_GROUPING ONE_PER_GROUP )
set( CPACK_GENERATOR "RPM" )

# set( CPACK_GENERATOR "RPM" "TGZ" )
# CPACK_RPM_BUILD_REQUIRES
# CPACK_RPM_liblhmodeldbutil_REQUIRES # component requires

set( CPACK_RPM_PACKAGE_RELOCATABLE ON)
set( CPACK_RPM_COMPONENT_INSTALL ON )

# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_PACKAGE_REQUIRES_POST "sed" ) 
set( CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" )

include( CPack )

# component, group can be different; group will appended to package
cpack_add_component(liblhwsmodelapi
    DISPLAY_NAME  "lhwsmodelapi library"
    DESCRIPTION   "lhwsmodelapi library"
    GROUP liblhwsmodelapi)
cpack_add_component_group(liblhwsmodelapi)
# .. boilerplate