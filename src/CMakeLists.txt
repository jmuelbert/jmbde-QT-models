# ---------------------------------------------------------------------------- #
# SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert <juergen.muelbert@outlook.de>
# SPDX-License-Identifier: EUPL-1.2
# ---------------------------------------------------------------------------- #

# CMake includes for package configuration, dependencies, and exports
include(CMakePackageConfigHelpers)
include(CMakeFindDependencyMacro)
include(GenerateExportHeader)

# ---------------------------------------------------------------------------- #
# Project Metadata
# ---------------------------------------------------------------------------- #
set(TARGET_ID jmbdemodels)
string(TOLOWER ${TARGET_ID} TARGET_ID_LOWER)
set(TARGET_NAMESPACE jmuelbert)
set(TARGET_NAME ${TARGET_ID_LOWER})
set(TARGET_VERSION_MAJOR 0)
set(TARGET_VERSION_MINOR 7)
set(TARGET_VERSION_PATCH 0)
set(TARGET_VERSION
    ${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${TARGET_VERSION_PATCH})
set(TARGET_SO_VERSION ${TARGET_VERSION_MAJOR})

message(STATUS "Building ${TARGET_NAMESPACE}::${TARGET_NAME}"
               " Version ${TARGET_VERSION} SO_Version ${TARGET_SO_VERSION}"
               " in ${CMAKE_BUILD_TYPE} mode")

# ---------------------------------------------------------------------------- #
# Find Qt6
# ---------------------------------------------------------------------------- #
find_package(
  Qt6 REQUIRED
  COMPONENTS Core Gui Sql
  OPTIONAL_COMPONENTS LinguistTools)

# ---------------------------------------------------------------------------- #
# Define Library Target
# ---------------------------------------------------------------------------- #
qt_standard_project_setup()

if(BUILD_SHARED_LIBS)
  qt_add_library(${TARGET_NAME} SHARED MANUAL_FINALIZATION)
else()
  qt_add_library(${TARGET_NAME} STATIC MANUAL_FINALIZATION)
endif()

add_library(${TARGET_NAMESPACE}::${TARGET_NAME} ALIAS ${TARGET_NAME})

# ---------------------------------------------------------------------------- #
# Source and Include Directories
# ---------------------------------------------------------------------------- #
set(TARGET_INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)
include(CMakeLists_files.cmake) # Define SOURCES and HEADERS

target_sources(${TARGET_NAME} PRIVATE ${SOURCES} ${HEADERS})

# ---------------------------------------------------------------------------- #
# Target Properties
# ---------------------------------------------------------------------------- #
set_target_properties(${TARGET_NAME}
  PROPERTIES
    VERSION ${TARGET_VERSION}
    SOVERSION ${TARGET_SO_VERSION}
    EXPORT_NAME ${TARGET_NAME}
    MSVC_RUNTIME_LIBRARY "${MSVC_RUNTIME_TYPE}")

# ---------------------------------------------------------------------------- #
# Include Directories
# ---------------------------------------------------------------------------- #
target_include_directories(${TARGET_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
    $<BUILD_INTERFACE:${TARGET_INCLUDE_FOLDER}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${TARGET_INCLUDE_FOLDER})


# ---------------------------------------------------------------------------- #
# Compile Definitions and Options
# ---------------------------------------------------------------------------- #
target_compile_definitions(${TARGET_NAME}
  PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    TRANSLATION_DOMAIN="${TARGET_NAME}"
    JMBDE_MODELS_VERSION="${TARGET_VERSION}")

# ensure right translations domain, we use one kate.po(t) for lib + apps
target_compile_definitions(${TARGET_NAME}
                           PRIVATE TRANSLATION_DOMAIN="${TARGET_NAME}")

# ---------------------------------------------------------------------------- #
# Link Libraries
# ---------------------------------------------------------------------------- #
target_link_libraries(${TARGET_NAME}
  PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Sql
    ${CMAKE_REQUIRED_LIBRARIES}
    )

# ---------------------------------------------------------------------------- #
# TODO: Find the Issue and enable
# Precompiled Headers (Optional)
# ---------------------------------------------------------------------------- #
# if(JMBDE_MODELS_ENABLE_PCH)
#    target_precompile_headers(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/pch.h)
# endif()

qt_finalize_target(${TARGET_NAME})

# ---------------------------------------------------------------------------- #
# Export Header Generation
# ---------------------------------------------------------------------------- #
generate_export_header(${TARGET_NAME}
  EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/${TARGET_NAME}_export.h)

# ---------------------------------------------------------------------------- #
# Installation Directories
# ---------------------------------------------------------------------------- #
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME})

install(TARGETS ${TARGET_NAME}
  EXPORT ${TARGET_NAME}Config
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY include/${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ---------------------------------------------------------------------------- #
# CMake Package Configuration
# ---------------------------------------------------------------------------- #
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ID}ConfigVersion.cmake
  VERSION ${TARGET_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ID}Config.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ID}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ID}ConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR})

# ---------------------------------------------------------------------------- #
# Export Build Tree Configuration
# ---------------------------------------------------------------------------- #
export(EXPORT ${TARGET_NAME}Config
  FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_ID}Config.cmake
  NAMESPACE ${TARGET_NAMESPACE}::)

if(__MSVC AND BUILD_SHARED_LIBS)
  install(FILES "$<TARGET_PDB_FILE:${TARGET_NAME}>" DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
