if (ENABLE_VCPKG)
    if (${PROJECT_NAME}_VERBOSE_OUTPUT)
        set(VCPKG_VERBOSE ON)
    endif ()
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif ()
