# SPDX-FileCopyrightText: 2013-2024 Jürgen Mülbert <juergen.muelbert@gmail.com>
# SPDX-License-Identifier: MIT

include(CTest)

# Find the required Qt6Test package
find_package(Qt6 REQUIRED
  COMPONENTS Core Gui Sql Test
)

qt_standard_project_setup()

# Macro to add executable tests for offscreen rendering
macro(jmbde_executable_tests)
  foreach(_testname ${ARGN})  # Iterate over all provided test names

    # Create an executable for each test
    qt_add_executable(${_testname} ${_testname}.cpp tests.qrc)

    set_target_properties(${_testname} PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        )

    # Link the necessary libraries to the test executable
    target_link_libraries(
      ${_testname}
      PRIVATE Qt6::Core  # Core Qt6 library
              Qt6::Gui
              Qt6::Sql
              Qt6::Test  # Qt6 Test library
              jmuelbert::jmbdemodels  # Custom library for the project
    )

    # Register the test with CTest
    add_test(NAME ${_testname} COMMAND $<TARGET_FILE:${_testname}>)
  endforeach()
endmacro()

# Add tests based on the platform
# Currently, both Windows and non-Windows platforms use the same test
jmbde_executable_tests(test_account test_datacontext)  # Add all tests
