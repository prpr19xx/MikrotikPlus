cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(MikrotikPlus VERSION 0.04.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE HEADERS_LIST CONFIGURE_DEPENDS include/MikrotikPlus/*.hpp)
file(GLOB_RECURSE SOURCES_LIST "src/*.cpp")

add_library(
    MikrotikPlus STATIC
    ${SOURCES_LIST}
)

target_sources(MikrotikPlus PUBLIC $<BUILD_INTERFACE:${HEADERS_LIST}>)

target_include_directories(
    MikrotikPlus PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(GNUInstallDirs)

set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/MikrotikPlus/cmake)

install(
    TARGETS MikrotikPlus
    EXPORT MikrotikPlusConfig
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION lib
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT MikrotikPlusConfig
    NAMESPACE MikrotikPlus::
    DESTINATION ${INSTALL_CMAKE_DIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MikrotikPlusConfig-version.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/MikrotikPlusConfig-version.cmake
    DESTINATION ${INSTALL_CMAKE_DIR}
)