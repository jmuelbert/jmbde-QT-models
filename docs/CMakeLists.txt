# --------------------------------------------------------------------------------------------------------------------
# CMake configuration for ModernSharedLibTemplate documentation generation
# Copyright (c) 2021 C++ Modern Framework
# --------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21 FATAL_ERROR)  # Require CMake version 3.21 or higher

# Root project settings
set(ROOT_PROJECT ModernSharedLibTemplate)  # Name of the root project
set(ROOT_PROJECT_ID MLT)  # Identifier for the root project

# --------------------------------------------------------------------------------------------------------------------
# Early includes
# --------------------------------------------------------------------------------------------------------------------
# include(../cmake/CPM.cmake)  # Include the CPM module for package management

# --------------------------------------------------------------------------------------------------------------------
# Add project dependencies
# Some dependencies are added using CPM. See https://github.com/TheLartians/CPM.cmake for more info.
# --------------------------------------------------------------------------------------------------------------------
# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

FetchContent_Declare(
    awesome-css  # Add the awesome-css package for Doxygen styling
    GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
    GIT_TAG 1.6.1
    )

# --------------------------------------------------------------------------------------------------------------------
# Project configuration
# --------------------------------------------------------------------------------------------------------------------
project(${ROOT_PROJECT}_Doc)  # Define the project for documentation

# --------------------------------------------------------------------------------------------------------------------
# Find Doxygen and configure settings
# For more information, see: https://cmake.org/cmake/help/latest/module/FindDoxygen.html and https://www.doxygen.nl/manual/config.html
# --------------------------------------------------------------------------------------------------------------------
find_package(Doxygen REQUIRED)  # Find the Doxygen package

# Set Doxygen project information from CMake variables
set(DOXYGEN_PROJECT_NAME ${${ROOT_PROJECT_ID}_PROJECT_NAME})  # Project name
set(DOXYGEN_PROJECT_BRIEF ${${ROOT_PROJECT_ID}_PROJECT_DESCRIPTION})  # Brief description
set(DOXYGEN_PROJECT_COPYRIGHT ${${ROOT_PROJECT_ID}_PROJECT_COPYRIGHT})  # Copyright information
set(DOXYGEN_PROJECT_NUMBER ${${ROOT_PROJECT_ID}_VERSION})  # Project version
set(DOXYGEN_AUTHOR_ORGANIZATION ${${ROOT_PROJECT_ID}_AUTHOR_ORGANIZATION})  # Author organization
set(DOXYGEN_AUTHOR_DOMAIN ${${ROOT_PROJECT_ID}_AUTHOR_DOMAIN})  # Author domain
set(DOXYGEN_AUTHOR_MAINTAINER ${${ROOT_PROJECT_ID}_AUTHOR_MAINTAINER})  # Author maintainer

# Doxygen configuration settings
set(DOXYGEN_FULL_PATH_NAMES YES)  # Show full path names in documentation
set(DOXYGEN_STRIP_FROM_PATH ${${ROOT_PROJECT}_SOURCE_DIR})  # Strip source directory from paths
set(DOXYGEN_EXTRACT_ALL YES)  # Extract all entities
set(DOXYGEN_EXTRACT_PRIVATE YES)  # Extract private members
set(DOXYGEN_EXTRACT_STATIC YES)  # Extract static members
set(DOXYGEN_RECURSIVE YES)  # Recursively search for files
set(DOXYGEN_SOURCE_BROWSER YES)  # Enable source browser
set(DOXYGEN_INLINE_SOURCES YES)  # Include inline source code

# Custom theme settings for Doxygen
set(DOXYGEN_GENERATE_TREEVIEW YES)  # Generate a tree view in the documentation
set(DOXYGEN_HTML_EXTRA_STYLESHEET
    ${awesome-css_SOURCE_DIR}/doxygen-awesome.css
    ${awesome-css_SOURCE_DIR}/doxygen-awesome-sidebar-only.css
    ${awesome-css_SOURCE_DIR}/doxygen-awesome-sidebar-only-darkmode-toggle.css
    custom.css
)
set(DOXYGEN_HTML_EXTRA_FILES ${awesome-css_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js)  # Extra JavaScript for dark mode toggle
set(DOXYGEN_HTML_HEADER ${CMAKE_BINARY_DIR}/header.html)  # Custom HTML header
set(DOXYGEN_HTML_FOOTER ${CMAKE_BINARY_DIR}/footer.html)  # Custom HTML footer

# Set the main page for the documentation
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ../README.md)  # Use README.md as the main page

# Define output directory for Doxygen
set(DOXYGEN_OUTPUT_DIRECTORY .)  # Output directory for generated documentation

# Generate header and footer HTML files from templates
configure_file(${CMAKE_CURRENT_LIST_DIR}/header.html.in ${CMAKE_BINARY_DIR}/header.html)  # Configure header
configure_file(${CMAKE_CURRENT_LIST_DIR}/footer.html.in ${CMAKE_BINARY_DIR}/footer.html)  # Configure footer

# --------------------------------------------------------------------------------------------------------------------
# Add target for generating documentation with Doxygen
# --------------------------------------------------------------------------------------------------------------------
doxygen_add_docs(docs "${CMAKE_CURRENT_SOURCE_DIR}mainpage.md" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include")  # Add documentation generation target
