# ---------------------------------------------------------------------------- #
# SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert <juergen.muelbert@outlook.de>
# SPDX-License-Identifier: EUPL-1.2
# ---------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.16...3.27)

# ---------------------------------------------------------------------------- #
# Project Configuration
# ---------------------------------------------------------------------------- #
set(LIB_NAME jmbdemodels)
set(LIB_NAMESPACE JMBDEModels)
set(LIB_VERSION_MAJOR 0)
set(LIB_VERSION_MINOR 7)
set(LIB_VERSION_PATCH 0)
set(LIB_VERSION "${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH}")
set(LIB_SOVERSION ${LIB_VERSION_MAJOR})

# ---------------------------------------------------------------------------- #
# CMake Includes
# ---------------------------------------------------------------------------- #
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

# ---------------------------------------------------------------------------- #
# Options
# ---------------------------------------------------------------------------- #
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(JMBDE_ENABLE_PCH "Enable precompiled headers" ON)
option(JMBDE_BUILD_TESTS "Build test suite" ON)
option(JMBDE_ENABLE_LTO "Enable Link Time Optimization" ON)

# ---------------------------------------------------------------------------- #
# Qt Configuration
# ---------------------------------------------------------------------------- #
find_package(Qt6 6.2 REQUIRED COMPONENTS
    Core Gui Sql OPTIONAL_COMPONENTS LinguistTools)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
qt_standard_project_setup()

# ---------------------------------------------------------------------------- #
# Source Files
# ---------------------------------------------------------------------------- #
# Include source and headers files list
include(CMakeLists_files.cmake)

# ---------------------------------------------------------------------------- #
# Library Target
# ---------------------------------------------------------------------------- #
qt_add_library(${LIB_NAME} MANUAL_FINALIZATION)
add_library(${LIB_NAMESPACE}::${LIB_NAME} ALIAS ${LIB_NAME})

target_sources(${LIB_NAME}
  PRIVATE
    ${SOURCES}
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES ${HEADERS}
)

# ---------------------------------------------------------------------------- #
# Target Properties
# ---------------------------------------------------------------------------- #
set_target_properties(${LIB_NAME}
    PROPERTIES
        VERSION ${LIB_VERSION}
        SOVERSION ${LIB_SOVERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON
        EXPORT_NAME ${LIB_NAME}
        DEBUG_POSTFIX "d"
        MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}"
)

# ---------------------------------------------------------------------------- #
# Include Directories and Compile Definitions
# ---------------------------------------------------------------------------- #
target_include_directories(${LIB_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${LIB_NAME}
  PRIVATE
    JMBDE_MODELS_LIBRARY
    QT_NO_CAST_TO_ASCII
    QT_NO_CAST_FROM_ASCII
    QT_NO_URL_CAST_FROM_STRING
    QT_NO_CAST_FROM_BYTEARRAY
    QT_USE_QSTRINGBUILDER
    QT_NO_SIGNALS_SLOTS_KEYWORDS
    QT_USE_FAST_CONCATENATION
    QT_USE_FAST_OPERATOR_PLUS
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    )

# ---------------------------------------------------------------------------- #
# Link Libraries
# ---------------------------------------------------------------------------- #
target_link_libraries(${LIB_NAME}
  PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Sql
    ${CMAKE_REQUIRED_LIBRARIES}
    )

# ---------------------------------------------------------------------------- #
# Precompiled Headers
# ---------------------------------------------------------------------------- #
if(JMBDE_ENABLE_PCH)
    target_precompile_headers(${LIB_NAME}
        PRIVATE
            <QtCore>
            <QtGui>
            <QtSql>
    )
endif()

# ---------------------------------------------------------------------------- #
# Export Header Generation
# ---------------------------------------------------------------------------- #
generate_export_header(${LIB_NAME}
  BASE_NAME ${LIB_NAME}
  EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/${LIB_NAME}_export.h
)

# ---------------------------------------------------------------------------- #
# Installation
# ---------------------------------------------------------------------------- #
install(TARGETS ${LIB_NAME}
  EXPORT ${LIB_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install PDB files for MSVC
if(MSVC AND BUILD_SHARED_LIBS)
    install(FILES $<TARGET_PDB_FILE:${LIB_NAME}>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        OPTIONAL
    )
endif()

# ---------------------------------------------------------------------------- #
# Package Configuration
# ---------------------------------------------------------------------------- #
configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
  VERSION ${LIB_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}
)

install(
  EXPORT ${LIB_NAME}Targets
  FILE ${LIB_NAME}Targets.cmake
  NAMESPACE ${LIB_NAMESPACE}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}
)

# ---------------------------------------------------------------------------- #
# Tests Configuration
# ---------------------------------------------------------------------------- #
# Add testing if enabled
# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR JMBDE_MODELS_ENABLE_TESTING)
#  enable_testing()
#  add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
# endif()

# ---------------------------------------------------------------------------- #
# Finalize Qt Setup
# ---------------------------------------------------------------------------- #
qt_finalize_target(${LIB_NAME})
