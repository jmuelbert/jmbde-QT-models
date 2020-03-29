#
#   jmbde a BDE Tool for companies
#   Copyright (C) 2013-2020  Jürgen Mülbert
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.


# minimal requirements
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

#
# the models library
#
set(JMBDEMODELS_VERSION 0.2.0)
set(JMBDEMODELS_SOVERSION 0)


#
# Building in-tree is not allowed (we take care of your craziness).
#

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmakex)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()


# Version setup
# set project version
# ecm_setup_version(${JMBDEMODELS_VERSION}
#    VARIABLE_PREFIX JMBDEMODELS
#    SOVERSION ${JMBDEMODELS_VERSION}
#    VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/jmbdemodels-version.h)


# JMBDEMODELS project
project(jmbdemodels VERSION ${JMBDEMODELS_SOVERSION}
      DESCRIPTION "${PROJECT_NAME} Datacontext Library")

message(STATUS "Building ${PROJECT_NAME} ${JMBDEMODELS_VERSION} ${JMBDEMODELS_SOVERSION} in ${CMAKE_BUILD_TYPE} mode")

# ecm_qt_declare_logging_category(jmbdemodels
#    HEADER loggingcategory.h
#    IDENTIFIER jmbdemodelsLog
#    CATEGORY_NAME jmuelbert.jmbde
#    DEFAULT_SEVERITY Warning
#)

include(FeatureSummary)
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

# load helper modules
include(CheckFunctionExists)
include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)
include(WriteBasicConfigVersionFile)
include(GenerateExportHeader)
include(GNUInstallDirs)


include(cmake/utils.cmake)
#
# Project configuration options
#

# Project options
option(BUILD_EXAMPLES "Build examples." OFF)
add_feature_info(EXAMPLES ${BUILD_EXAMPLES} "Build the the examples")
option(BUILD_MANUAL "Build manual." OFF)
add_feature_info(MANUAL ${BUILD_MANUAL}
                 "Build the Documentation and man pages")
option(BUILD_API_DOCS "Build the api-docs." OFF)
add_feature_info(API_DOCS ${BUILD_API_DOCS}
                 "Build the Documentation and man pages")
                 option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")
option(BUILD_TRANSLATIONS "Build translations" ON)
add_feature_info(TRANSL ${BUILD_TRANSLATIONS} "Build the translations")
option(BUILD_COVERAGE "Add coverage information." OFF)
add_feature_info(COVERAGE ${BUILD_COVERAGE} "Build with coverage informations.")
option(BUILD_SHARED_LIBS "Build shared libs" ON)
add_feature_info(SHARED_LIBS ${BUILD_SHARED_LIBS} "Build shared libs")
option(BUILD_TESTING "Build tests." ON)
add_feature_info(TESTS ${BUILD_TESTING} "Build and run the tests")
option(ENABLE_CLAZY "Enable clazy" ON)
add_feature_info(CLAZY ${ENABLE_CLAZY} "Build with clazy hints")
option(ENABLE_CLANG_TIDY "activate clang tidy messages" ON)
add_feature_info(CLANG_TIDY ${ENABLE_CLANG_TIDY} "Check the source with clang tidy")
option(ENABLE_CLANG_FORMAT "activate clang format" ON)
add_feature_info(CLANG_FORMAT ${ENABLE_CLANG_FORMAT} "Format the source with clang format")
option(ENABLE_CCACHE "prefix each compile command with ccache" OFF)
add_feature_info(CCACHE ${ENABLE_CCACHE} "Compile with ccache")
option(USE_GIT_VERSION "Append Git HEAD to jmbde version (if exists)" ON)

# forbid some old things
add_definitions(-DQT_NO_FOREACH)

# Allow adding Qt resource files with `add_executable` or `target_sources` instead of
# `qt5_add_resources`. See https://cmake.org/cmake/help/v3.0/manual/cmake-qt.7.html#autorcc.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Required here so that the version comparison below works
find_package(Qt5Widgets ${QT_MIN_VERSION} CONFIG REQUIRED)

set(LANGUAGES
    "All"
    CACHE
      STRING
      "A space-seperated list of translations to compile into jmbde or \"All\"")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)


if(USE_GIT_VERSION)
  if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    find_package(Git)
    set_package_properties(Git PROPERTIES TYPE OPTIONAL PURPOSE
                           "Determine exact build version.")
    if(GIT_FOUND)
      execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE _git_revision)
      string(REGEX REPLACE "\n" "" _git_revision "${_git_revision}")
      set(JMBDEMODELS_REVISION
          "${JMBDEMODELS_REVISION} (revision: ${_git_revision})")
    endif()
  endif()
endif()

##################################################
# clang tidy static analysis
if(ENABLE_CLANG_TIDY)
  find_program (CLANG_TIDY_EXE NAMES "clang-tidy" PATHS /usr/bin)
  if (CLANG_TIDY_EXE)
    message(STATUS "Clang-tidy supported, found and enabled: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
        CACHE STRING "" FORCE)
  else()
    message(AUTHOR_WARNING "clang-tidy not found!")
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # delete it
  endif()
else()
  message(STATUS "Clang-tidy supported but disabled")
endif()

##################################################
# clang format cpp and h files.
if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_EXE NAMES "clang-format" PATHS /usr/bin)
    if (CLANG_FORMAT_EXE)
        message(STATUS "Clang-format supported, found and enabled: ${CLANG_FORMAT_EXE}")
        file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
        # "${CLANG_FORMAT_EXE}" -i (${ALL_CLANG_FORMAT_SOURCE_FILES})
    else()
        message(AUTHOR_WARNING "clang-format not found!")
    endif()
else()
  message(STATUS "Clang-format supported but disabled")
endif()

##################################################
# ccache setup
# distros can also do this but they don't use this mechanism
if(ENABLE_CCACHE)
        find_program(CCACHE_FOUND "ccache")

        if(CCACHE_FOUND)
                set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
                set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
        else()
                message(FATAL_ERROR "ccache not found, but you requested it")
        endif(CCACHE_FOUND)
endif()

#
# cppcheck
#
find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)
  set(_cppcheck_flags "-I${CMAKE_CURRENT_BINARY_DIR}")
  get_directory_property(_inc_dirs INCLUDE_DIRECTORIES)
  foreach(_current ${_inc_dirs})
    set(_cppcheck_flags ${_cppcheck_flags} "-I${_current}")
  endforeach()
  get_directory_property(_defs COMPILE_DEFINITIONS)
  foreach(_current ${_defs})
    set(_cppcheck_flags ${_cppcheck_flags} "-D${_current}")
  endforeach()

  add_custom_target(cppcheck
    COMMAND ${CPPCHECK_EXECUTABLE} --enable=all --inconclusive -f --suppress=*:${QT_INCLUDE_DIR}* ${_cppcheck_flags}
      -i${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
      -i${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running the cppcheck static code checker"
  )
endif()


##################################################
# Build the API Documentation
if(BUILD_API_DOCS AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wdocumentation)
endif()

remove_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)


# debug suffixes for qmake compatibility
if(WIN32)
  set(CMAKE_DEBUG_POSTFIX "d")
elseif(APPLE)
  set(CMAKE_DEBUG_POSTFIX "_debug")
endif()

if(UPPERCASE_CMAKE_BUILD_TYPE MATCHES "^RELEASE$")
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# if translation/doc host tools are missing, the Qt5 cmake config files throw
# errors...
if(BUILD_API_DOCS OR BUILD_MANUAL)
  find_package(Qt5 NO_MODULE QUIET OPTIONAL_COMPONENTS Help LinguistTools)
  add_subdirectory(docs)
endif()

set(LICENSE_FILE "LICENSE")
set(README_FILE "README.md")
list(APPEND DOCS ${LICENSE_FILE} ${README_FILE})

add_library(jmbdemodels)
generate_export_header(jmbdemodels)
add_library(jmbde::models ALIAS jmbdemodels)

enable_warnings(jmbdemodels)
enable_sanitizer(jmbdemodels)

set_target_properties(jmbdemodels PROPERTIES
      VERSION   ${JMBDEMODELS_SOVERSION}
      SOVERSION ${JMBDEMODELS_SOVERSION}
      EXPORT_NAME "jmbdemodels"
      )

target_compile_features(jmbdemodels
    PUBLIC cxx_std_17)

target_include_directories(jmbdemodels PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

target_compile_definitions(jmbdemodels PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries( jmbdemodels
    LINK_PUBLIC
    Qt5::Core
    LINK_PRIVATE
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Sql
    )

#
# Finding Qt
#
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
    COMPONENTS
    Core
    Widgets
    Sql
    PrintSupport
)


QT5_ADD_RESOURCES(
  Resources_SRCS
  resources/res.qrc
)

target_sources(jmbdemodels
    PUBLIC
    # jmbdemodels-version.h
    # models
    include/models/account.h
    include/models/chipcard.h
    include/models/chipcarddoor.h
    include/models/chipcardprofile.h
    include/models/chipcardprofiledoor.h
    include/models/cityname.h
    include/models/company.h
    include/models/computer.h
    include/models/computersoftware.h
    include/models/datacontext.h
    include/models/department.h
    include/models/devicename.h
    include/models/devicetype.h
    include/models/document.h
    include/models/employeeaccount.h
    include/models/employee.h
    include/models/employeedocument.h
    include/models/fax.h
    include/models/function.h
    include/models/inventory.h
    include/models/manufacturer.h
    include/models/mobile.h
    include/models/os.h
    include/models/phone.h
    include/models/place.h
    include/models/printer.h
    include/models/processor.h
    include/models/software.h
    include/models/system.h
    include/models/title.h
    include/models/zipcity.h
    include/models/zipcode.h

    PRIVATE
    # Models
    models/loggingcategory.cpp
    models/account.cpp
    models/chipcard.cpp
    models/chipcarddoor.cpp
    models/chipcardprofile.cpp
    models/chipcardprofiledoor.cpp
    models/cityname.cpp
    models/commondata.cpp
    models/company.cpp
    models/computer.cpp
    models/computersoftware.cpp
    models/datacontext.cpp
    models/department.cpp
    models/devicename.cpp
    models/devicetype.cpp
    models/document.cpp
    models/employee.cpp
    models/employeeaccount.cpp
    models/employeedocument.cpp

    models/fax.cpp
    models/function.cpp
    # models/idatamodel.cpp
    models/inventory.cpp
    models/manufacturer.cpp
    models/mobile.cpp
    models/os.cpp

    models/phone.cpp
    models/place.cpp
    models/printer.cpp
    models/processor.cpp

    models/software.cpp
    models/system.cpp
    models/title.cpp
    models/zipcity.cpp
    models/zipcode.cpp

    ${Resources_SRCS}

    include/models/commondata.h
    include/models/idatamodel.h

    include/loggingcategory.h

    )

#
# Install 
#

install(TARGETS jmbdemodels 
    EXPORT jmbdeModelsTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )



configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/config.cmake.in
    JMBDEModelsConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/jmbdemodels/cmake
)

write_basic_package_version_file(
     JMBDEModelsConfigVersion.cmake
    VERSION ${JMBDEMODELS_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(
    EXPORT jmbdeModelsTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/jmbdeModelsTargets.cmake
    NAMESPACE jmbde::
)

install(EXPORT jmbdeModelsTargets
    FILE jmbdemodelsTargets.cmake
    NAMESPACE jmbde::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jmbdemodels
)

install(
    FILES
        ${PROJECT_BINARY_DIR}/JMBDEModelsConfig.cmake
        ${PROJECT_BINARY_DIR}/JMBDEModelsConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/jmbdemodels/cmake
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# export(PACAKGE jmbdemodels)

if(MSVC AND BUILD_SHARED_LIBS)
  install(FILES "$<TARGET_PDB_FILE_DIR:jmbdemodels>/$<TARGET_PDB_FILE_NAME:jmbdemodels>" DESTINATION ${BIN_INSTALL_DIR} CONFIGURATIONS Debug RelWithDebInfo)
endif()

# - Tests
if(BUILD_TESTING)
    if(GOOGLE_TEST)
        enable_testing()
        # Unittests.
        add_subdirectory(tests)
        add_test(
            NAME jmbdeModelsTestLib
            COMMAND testJmbdeModels
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()


    enable_testing()
    add_subdirectory(tests)
endif()

#
# AOB
#

add_custom_target(
    aob
    SOURCES
        .github/workflows/build.yml
        .github/workflows/coverage.yml
        .github/workflows/deploy.yml
        .github/FUNDING.yml
        AUTHORS
        CONTRIBUTING.md
        LICENSE
        README.md
        TODO
)

# tell about our features (and what is missing)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
