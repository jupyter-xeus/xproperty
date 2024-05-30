############################################################################
# Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.1)
project(xproperty)

set(XPROPERTY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Versionning
# ===========

file(STRINGS "${XPROPERTY_INCLUDE_DIR}/xproperty/xproperty_config.hpp" xproperty_version_defines
     REGEX "#define XPROPERTY_VERSION_(MAJOR|MINOR|PATCH)")
foreach(ver ${xproperty_version_defines})
    if(ver MATCHES "#define XPROPERTY_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XPROPERTY_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(${PROJECT_NAME}_VERSION
    ${XPROPERTY_VERSION_MAJOR}.${XPROPERTY_VERSION_MINOR}.${XPROPERTY_VERSION_PATCH})
message(STATUS "xproperty v${${PROJECT_NAME}_VERSION}")

# Dependencies
# ============

# nlohmann_json requires libraries that exchange json objects to be linked
# with the same version of nlohmann_json. Therefore this version should be
# the same in all xeus components; to do so, downstream projects should not
# search # directly for nlohmann_json, but rely on find_dependency called by
# find_package(xeus) instead.
set(nlohmann_json_REQUIRED_VERSION 3.11)

if (NOT TARGET nlohmann_json)
    find_package(nlohmann_json ${nlohmann_json_REQUIRED_VERSION} REQUIRED)
    message(STATUS "Found nlohmann_json ${nlohmann_json_VERSION}")
endif ()

# Build
# =====

set(XPROPERTY_HEADERS
    ${XPROPERTY_INCLUDE_DIR}/xproperty/xproperty.hpp
    ${XPROPERTY_INCLUDE_DIR}/xproperty/xobserved.hpp
    ${XPROPERTY_INCLUDE_DIR}/xproperty/xjson.hpp
    ${XPROPERTY_INCLUDE_DIR}/xproperty/xproperty_config.hpp
)

add_library(xproperty INTERFACE)
target_include_directories(xproperty INTERFACE $<BUILD_INTERFACE:${XPROPERTY_INCLUDE_DIR}>
                                               $<INSTALL_INTERFACE:include>)
OPTION(BUILD_TESTS "xproperty test suite" OFF)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

# Installation
# ============

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS xproperty
        EXPORT ${PROJECT_NAME}-targets)

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

install(FILES ${XPROPERTY_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xproperty)

set(XPROPERTY_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for xpropertyConfig.cmake")

configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${XPROPERTY_CMAKECONFIG_INSTALL_DIR})

# xproperty is header-only and does not depend on the architecture.
# Remove CMAKE_SIZEOF_VOID_P from xpropertyConfigVersion.cmake so that an xpropertyConfig.cmake
# generated for a 64 bit target can be used for 32 bit targets and vice versa.
set(_XPROPERTY_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 VERSION ${${PROJECT_NAME}_VERSION}
                                 COMPATIBILITY AnyNewerVersion)
set(CMAKE_SIZEOF_VOID_P ${_XPROPERTY_CMAKE_SIZEOF_VOID_P})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${XPROPERTY_CMAKECONFIG_INSTALL_DIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${XPROPERTY_CMAKECONFIG_INSTALL_DIR})

