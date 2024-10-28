cmake_minimum_required(VERSION 3.16)

# Create static library for VL53L8CX core driver
add_library(vl53l8cx_driver STATIC
    VL53L8CX_ULD_API/src/vl53l8cx_api.c
#    VL53L8CX_ULD_API/src/vl53l8cx_plugin_detection_thresholds.c
#    VL53L8CX_ULD_API/src/vl53l8cx_plugin_motion_indicator.c
#    VL53L8CX_ULD_API/src/vl53l8cx_plugin_xtalk.c
)

# Include directories for the core driver
target_include_directories(vl53l8cx_driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Platform
        ${CMAKE_CURRENT_SOURCE_DIR}/VL53L8CX_ULD_API/inc
)

# Compiler flags for the driver
target_compile_options(vl53l8cx_driver
    PRIVATE
        -Os
        -ffunction-sections
        -fdata-sections
        -Wall
        -Wextra
        -mcpu=cortex-m7
        -mthumb
        -mfloat-abi=hard
        -mfpu=fpv5-sp-d16
        -fno-exceptions
        -fno-rtti
        -fdata-sections
)
