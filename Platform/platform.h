#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#pragma once

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t address;
    void* i2c_config;  // Opaque pointer to store I2cConfig
} VL53L8CX_Platform;

#define VL53L8CX_NB_TARGET_PER_ZONE 1U

uint8_t VL53L8CX_RdByte(
    VL53L8CX_Platform *p_platform,
    uint16_t RegisterAdress,
    uint8_t *p_value);

uint8_t VL53L8CX_WrByte(
    VL53L8CX_Platform *p_platform,
    uint16_t RegisterAdress,
    uint8_t value);

uint8_t VL53L8CX_RdMulti(
    VL53L8CX_Platform *p_platform,
    uint16_t RegisterAdress,
    uint8_t *p_values,
    uint32_t size);

uint8_t VL53L8CX_WrMulti(
    VL53L8CX_Platform *p_platform,
    uint16_t RegisterAdress,
    uint8_t *p_values,
    uint32_t size);

void VL53L8CX_SwapBuffer(
    uint8_t *buffer,
    uint16_t size);

uint8_t VL53L8CX_WaitMs(
    VL53L8CX_Platform *p_platform,
    uint32_t TimeMs);

#ifdef __cplusplus
} // extern "C"

// Move this declaration outside of the extern "C" block but keep it in __cplusplus
#include "libs/base/i2c.h"

extern "C++" bool VL53L8CX_PlatformInit(
    VL53L8CX_Platform *p_platform,
    coralmicro::I2c bus = coralmicro::I2c::kI2c1,
    uint16_t address = 0x52);

#endif // __cplusplus

#endif // _PLATFORM_H_