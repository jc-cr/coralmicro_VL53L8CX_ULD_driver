# Create static library for VL53L8CX core driver
add_library(vl53l8cx_driver STATIC
    VL53L8CX_ULD_API/src/vl53l8cx_api.c
    VL53L8CX_ULD_API/src/vl53l8cx_buffers.c
    Platform/platform.cc
)

# Set source file properties
set_source_files_properties(
    VL53L8CX_ULD_API/src/vl53l8cx_api.c
    VL53L8CX_ULD_API/src/vl53l8cx_buffers.c
    PROPERTIES
    LANGUAGE C
)

set_source_files_properties(
    Platform/platform.cc
    PROPERTIES
    LANGUAGE CXX
)

# Include directories for the core driver
target_include_directories(vl53l8cx_driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Platform
        ${CMAKE_CURRENT_SOURCE_DIR}/VL53L8CX_ULD_API/inc
)

# Link against required libraries
target_link_libraries(vl53l8cx_driver
    PUBLIC
        libs_base-m7_freertos
)