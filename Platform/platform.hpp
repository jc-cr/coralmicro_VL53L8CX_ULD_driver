#ifndef PLATFORM_CPP_H_
#define PLATFORM_CPP_H_
#pragma once

#include "platform.h"
#include "libs/base/i2c.h"

namespace vl53l8cx {

// C++ initialization function
bool PlatformInit(VL53L8CX_Platform* p_platform,
                 coralmicro::I2c bus = coralmicro::I2c::kI2c1,
                 uint16_t address = 0x29);

}  // namespace vl53l8cx


#endif // PLATFORM_CPP_H_