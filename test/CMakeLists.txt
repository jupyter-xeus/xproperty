############################################################################
# Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.1)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(xproperty-test)

    find_package(xproperty REQUIRED CONFIG)
    set(XPROPERTY_INCLUDE_DIR ${xproperty_INCLUDE_DIRS})
endif ()

message(STATUS "Forcing tests build type to Release")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

include(CheckCXXCompilerFlag)

string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MP /bigobj /wd4800")
    set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
endif()

find_package(Threads)
find_package(doctest REQUIRED)

set(XPROPERTY_TESTS
    main.cpp
    test_utils.hpp
    test_xobserved.cpp
    test_xproperty.cpp
    test_xjson.cpp
)

add_executable(test_xproperty ${XPROPERTY_TESTS} ${XPROPERTY_HEADERS})
target_compile_features(test_xproperty PRIVATE cxx_std_17)
target_include_directories(test_xproperty PRIVATE ${XPROPERTY_INCLUDE_DIR})
target_link_libraries(test_xproperty PRIVATE doctest::doctest Threads::Threads)

add_custom_target(xtest COMMAND test_xproperty DEPENDS test_xproperty)
