#ifndef PLATFORM_H_
#define PLATFORM_H_
#pragma once


#include <stdint.h>
#include <string.h>

// Forward declaration of I2cConfig to avoid including C++ headers in C code
struct I2cConfig;


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t address;
    void* i2c_config;  // Opaque pointer to store I2cConfig
} VL53L8CX_Platform;

#define VL53L8CX_NB_TARGET_PER_ZONE 1U

// Core C functions needed by the driver
uint8_t VL53L8CX_RdByte(VL53L8CX_Platform *p_platform,
                        uint16_t RegisterAddress,
                        uint8_t *p_value);

uint8_t VL53L8CX_WrByte(VL53L8CX_Platform *p_platform,
                        uint16_t RegisterAddress,
                        uint8_t value);

uint8_t VL53L8CX_RdMulti(VL53L8CX_Platform *p_platform,
                         uint16_t RegisterAddress,
                         uint8_t *p_values,
                         uint32_t size);

uint8_t VL53L8CX_WrMulti(VL53L8CX_Platform *p_platform,
                         uint16_t RegisterAddress,
                         uint8_t *p_values,
                         uint32_t size);

void VL53L8CX_SwapBuffer(uint8_t *buffer,
                        uint16_t size);

uint8_t VL53L8CX_WaitMs(VL53L8CX_Platform *p_platform,
                        uint32_t TimeMs);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_H_