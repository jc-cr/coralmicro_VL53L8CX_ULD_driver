#ifndef VL53L8CX_BUFFERS_H_
#define VL53L8CX_BUFFERS_H_

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define section for initialization data
#define VL53L8CX_BUFFER_SECTION __attribute__((section(".vl53l8_buffers"), used, aligned(32)))

// Buffer sizes
#define VL53L8CX_FIRMWARE_SIZE 86017
#define VL53L8CX_DEFAULT_CONFIGURATION_SIZE 972
#define VL53L8CX_DEFAULT_XTALK_SIZE 776
#define VL53L8CX_GET_NVM_CMD_SIZE 40

/**
 * @brief Inner internal number of targets.
 */
#if VL53L8CX_NB_TARGET_PER_ZONE == 1
#define VL53L8CX_FW_NBTAR_RANGING	2
#else
#define VL53L8CX_FW_NBTAR_RANGING	VL53L8CX_NB_TARGET_PER_ZONE
#endif

// External declarations for the buffers
extern const uint8_t VL53L8CX_FIRMWARE[VL53L8CX_FIRMWARE_SIZE];
extern const uint8_t VL53L8CX_DEFAULT_CONFIGURATION[VL53L8CX_DEFAULT_CONFIGURATION_SIZE];
extern const uint8_t VL53L8CX_DEFAULT_XTALK[VL53L8CX_DEFAULT_XTALK_SIZE];
extern const uint8_t VL53L8CX_GET_NVM_CMD[VL53L8CX_GET_NVM_CMD_SIZE];



#ifdef __cplusplus
}
#endif

#endif /* VL53L8CX_BUFFERS_H_ */