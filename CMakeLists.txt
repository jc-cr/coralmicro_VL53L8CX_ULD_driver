add_library(vl53l8cx_driver STATIC
    VL53L8CX_ULD_API/src/vl53l8cx_api.c
    VL53L8CX_ULD_API/src/vl53l8cx_buffers.c
    Platform/platform.cc
)

# Set C standard for the C files
set_source_files_properties(
    VL53L8CX_ULD_API/src/vl53l8cx_api.c
    VL53L8CX_ULD_API/src/vl53l8cx_buffers.c
    PROPERTIES
    LANGUAGE C
    C_STANDARD 11
)

# Set C++ standard for the platform file
set_source_files_properties(
    Platform/platform.cc
    PROPERTIES
    LANGUAGE CXX
    CXX_STANDARD 17
)

# Make sure the library is compiled with position-independent code
set_target_properties(vl53l8cx_driver PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(vl53l8cx_driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Platform
        ${CMAKE_CURRENT_SOURCE_DIR}/VL53L8CX_ULD_API/inc
)

target_link_libraries(vl53l8cx_driver
    PUBLIC
        libs_base-m7_freertos
)