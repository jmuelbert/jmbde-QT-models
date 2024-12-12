# SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert
# SPDX-License-Identifier: EUPL-1.2

# Minimum CMake version required
cmake_minimum_required(VERSION 3.29)

# Set default policies
set(CMAKE_POLICY_DEFAULT_CMP0168 NEW)

# Project definition
project(
    jmbdemodels
    VERSION 0.7.0
    DESCRIPTION "JMBDE Database and Model module"
    HOMEPAGE_URL "https://github.com/jmuelbert/jmbde-QT-models"
    LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Enable IDE folder support
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Export compile commands for tooling
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Set default build options
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
    set(POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

    # Ensure compatibility with C++20
    if(NOT CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 20)
    endif()

    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    if(CMAKE_CXX_STANDARD LESS 20)
        message(FATAL_ERROR "This project requires at least C++20")
    endif()

    # Default build type for single-config generators
    if(NOT CMAKE_CONFIGURATION_TYPES)
        set(allowedBuildTypes Debug Release MinSizeRel RelWithDebInfo)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowedBuildTypes}")
        if(NOT CMAKE_BUILD_TYPE)
            set(CMAKE_BUILD_TYPE
                "RelWithDebInfo"
                CACHE STRING "Default build type" FORCE)
        elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
            message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
        endif()
    endif()

    # Set shared libraries as default
    option(BUILD_SHARED_LIBS "Build shared libraries" ON)

    # Treat warnings as errors in top-level projects
    option(CMAKE_COMPILE_WARNING_AS_ERROR "Treat compiler warnings as errors" ${PROJECT_IS_TOP_LEVEL})

    # MSVC runtime configuration
    if(MVSC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()

    # Directory for generated headers
    set(JMBDE_MODELS_GENERATED_INCLUDEDIR
        "${PROJECT_BINARY_DIR}/generated/include"
        CACHE PATH "Include directory for generated headers")

    # Python virtual environment handling
    set(Python_FIND_VIRTUALENV
        STANDARD
        CACHE STRING "Find Python in virtual environments")

    # Interprocedural optimization (IPO/LTO)
    option(CMAKE_INTERPROCEDURAL_OPTIMIZATION "Enable link-time optimization (IPO/LTO)" ON)

    # Split-dwarf debugging
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-gsplit-dwarf" HAVE_SPLIT_DWARF)
    option(JMBDE_MODELS_ENABLE_SPLIT_DWARF "Enable split-dwarf debugging" ${HAVE_SPLIT_DWARF})
    if(JMBDE_MODELS_ENABLE_SPLIT_DWARF)
        add_compile_options("$<$<CONFIG:Debug,RelWithDebInfo>:-gsplit-dwarf>")
    endif()

    # Static analysis with Clang-Tidy
    option(JMBDE_MODELS_ENABLE_CLANG_TIDY "Enable Clang-Tidy static analysis" ${PROJECT_IS_TOP_LEVEL})
    if(JMBDE_MODELS_ENABLE_CLANG_TIDY)
        find_program(CLANG_TIDY_EXECUTABLE clang-tidy REQUIRED)
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}" "-p" "${CMAKE_BINARY_DIR}")
    endif()

    # option(JMBDE_MODELS_ENABLE_IWYU "Enable include-what-you-use analysis" OFF)
    # if(JMBDE_MODELS_ENABLE_IWYU)
        # Locate IWYU executable
        # find_program(INCLUDE_WHAT_YOU_USE_EXECUTABLE NAMES include-what-you-use)
        # if(NOT INCLUDE_WHAT_YOU_USE_EXECUTABLE)
           # message(FATAL_ERROR "Include-what-you-use executable not found!")
        # endif()

        # Configure IWYU command
        # set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
        #        ${INCLUDE_WHAT_YOU_USE_EXECUTABLE}
        #            -w                  # Suppress warnings (remove during debugging)
        #            -Xiwyu --error      # Fail on IWYU errors
        #            -Xiwyu --experimental=clang_mappings
        #            -Xiwyu --comment_style=long
        #            -Xiwyu --mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/iwyu.imp
        #            -Xiwyu --max_line_length=1200
        #            # -Xiwyu --verbose=7
        #            CACHE STRING "Include-what-you-use command")
        # endif()

        # Precompiled headers
        option(JMBDE_MODELS_ENABLE_PCH "Enable precompiled headers" ${PROJECT_IS_TOP_LEVEL})

    # Enable testing
    option(JMBDE_MODELS_ENABLE_TESTING "Enable building of tests" ${PROJECT_IS_TOP_LEVEL})
    if(JMBDE_MODELS_ENABLE_TESTING)
        enable_testing()
    endif()

    # Compiler cache integration
    include(cmake/UseCompilerCache.cmake)
    usecompilercache()

    # Sanitizer support
    if(PROJECT_IS_TOP_LEVEL)
        include(cmake/Sanitizers.cmake)
    endif()

    # Documentation (Doxygen)
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, skipping documentation build")
    endif()

endif()

# FetchContent management
include(FetchContent)

# Subdirectories for source and packaging
add_subdirectory(src)

# Add testing if enabled
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR JMBDE_MODELS_ENABLE_TESTING)
   AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Add packaging for top-level projects
if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(packaging)
endif()
