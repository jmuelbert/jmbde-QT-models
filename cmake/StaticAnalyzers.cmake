# Options to enable static analysis tools
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)

# Enable cppcheck if requested
if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck) # Search for cppcheck executable
    if(CPPCHECK)
        # Set cppcheck options
        set(CMAKE_CXX_CPPCHECK
            "${CPPCHECK}"
            --suppress=missingInclude
            --enable=all
            --inline-suppr
            --inconclusive
            -i
            "${CMAKE_SOURCE_DIR}/imgui/lib") # Ignore imgui/lib directory
    else()
        message(SEND_ERROR "cppcheck requested but executable not found") # Error if not found
    endif()
endif()

# Enable clang-tidy if requested
if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy) # Search for clang-tidy executable
    if(CLANGTIDY)
        # Set clang-tidy options
        set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY}" -extra-arg=-Wno-unknown-warning-option)
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found") # Error if not found
    endif()
endif()

# Enable include-what-you-use if requested
if(ENABLE_INCLUDE_WHAT_YOU_USE)
    find_program(INCLUDE_WHAT_YOU_USE include-what-you-use) # Search for include-what-you-use executable
    if(INCLUDE_WHAT_YOU_USE)
        # Set include-what-you-use options
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${INCLUDE_WHAT_YOU_USE}")
    else()
        message(SEND_ERROR "include-what-you-use requested but executable not found") # Error if not found
    endif()
endif()
