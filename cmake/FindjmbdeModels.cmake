# FindJMBDEModels.cmake
#
# Finds the JMBDE models library.
#
# This module defines the following variables:
# - JMBDEModels_FOUND
# - JMBDEModels_VERSION
# - JMBDEModels_INCLUDE_DIRS
#
# It also creates the following import target:
# - jmuelbert::jmbdemodels
#
# Author: Jürgen Mülbert - juergen.muelbert@gmail.com
#

# Find the PkgConfig package manager
find_package(PkgConfig REQUIRED)

# Check for the JMBDEModels package
pkg_check_modules(PC_JMBDEModels QUIET JMBDEModels)

# Find the include directory for the JMBDEModels library
find_path(
  JMBDEModels_INCLUDE_DIR
  NAMES datacontext.h
  PATHS ${PC_JMBDEModels_INCLUDE_DIRS}
  PATH_SUFFIXES jmbdemodels
)

# Set the version variable from PkgConfig
set(JMBDEModels_VERSION ${PC_JMBDEModels_VERSION})

# Mark variables as advanced to hide them from the user interface
mark_as_advanced(JMBDEModels_FOUND JMBDEModels_INCLUDE_DIR JMBDEModels_VERSION)

# Include standard argument handling for find_package
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  JMBDEModels
  REQUIRED_VARS JMBDEModels_INCLUDE_DIR
  VERSION_VAR JMBDEModels_VERSION
)

# If the JMBDEModels library is found, set up the include directories
if(JMBDEModels_FOUND)
    # Set include directories to the parent directory to enable includes like:
    # #include <rapidjson/document.h>
    get_filename_component(JMBDEModels_INCLUDE_DIRS ${JMBDEModels_INCLUDE_DIR} DIRECTORY)
endif()

# Create an imported target for the JMBDEModels library if found
if(JMBDEModels_FOUND AND NOT TARGET jmuelbert::jmbdemodels)
    add_library(jmuelbert::jmbdemodels INTERFACE IMPORTED)
    set_target_properties(
    jmuelbert::jmbdemodels PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${JMBDEModels_INCLUDE_DIRS}"
  )
endif()
