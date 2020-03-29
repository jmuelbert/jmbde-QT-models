# Turn on warning on the given target
function(enable_warnings target_name)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        list(APPEND MSVC_OPTIONS "/W3")
        if(MSVC_VERSION GREATER 1900)
            list(APPEND MSVC_OPTIONS "/WX")
        endif()
    endif()

    target_compile_options(${target_name} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Wextra -Wconversion -pedantic -Wfatal-errors>
        $<$<CXX_COMPILER_ID:MSVC>:${MSVC_OPTIONS}>)

endfunction()

# Enable address sanitizer (gcc/clang only)
function(enable_sanitizer target_name)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "Sanitizer supoorted only for gcc/clang")
    endif()
	message(STATUS "Address sanitizer enabled")
	target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined)
	target_compile_options(${target_name} PRIVATE -fno-sanitize=signed-integer-overflow)
	target_compile_options(${target_name} PRIVATE -fno-sanitize-recover=all)
	target_compile_options(${target_name} PRIVATE -fno-omit-frame-pointer)
	target_link_libraries(${target_name} PRIVATE -fsanitize=address,undefined -fuse-ld=gold)
endfunction()
