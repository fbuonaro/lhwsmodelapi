cmake_minimum_required( VERSION 3.10 )

project( lhwsmodelapi
         DESCRIPTION "lhwsmodelapi libraries"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhwsmodelapi"
         LANGUAGES CXX
         VERSION 1.0.0  )

enable_testing()

add_subdirectory( liblhwsmodelapi )
