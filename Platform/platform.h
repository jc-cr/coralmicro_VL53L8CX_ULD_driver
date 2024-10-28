#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#pragma once

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure VL53L8CX_Platform needs to be filled by the customer,
 * depending on his platform. At least, it contains the VL53L8CX I2C address.
 * Some additional fields can be added, as descriptors, or platform dependencies.
 */
typedef struct {
    uint16_t address;
    void* platform_handle;  // Generic handle for platform-specific data
} VL53L8CX_Platform;

/*
 * @brief The macro below is used to define the number of target per zone sent
 * through I2C. This value can be changed by user, in order to tune I2C
 * transaction, and also the total memory size (a lower number of target per
 * zone means a lower RAM). The value must be between 1 and 4.
 */
#define VL53L8CX_NB_TARGET_PER_ZONE 1U

/* Function declarations that need to be implemented */
uint8_t VL53L8CX_RdByte(VL53L8CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t *p_value);
uint8_t VL53L8CX_WrByte(VL53L8CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t value);
uint8_t VL53L8CX_RdMulti(VL53L8CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t *p_values, uint32_t size);
uint8_t VL53L8CX_WrMulti(VL53L8CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t *p_values, uint32_t size);
uint8_t VL53L8CX_WaitMs(VL53L8CX_Platform *p_platform, uint32_t TimeMs);
void VL53L8CX_SwapBuffer(uint8_t *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif  // _PLATFORM_H_