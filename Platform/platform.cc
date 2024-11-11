#include "platform.h"
#include "platform.hpp"

#include <stdio.h>
#include "libs/base/check.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"

namespace {
    constexpr size_t kMaxChunkSize = 256;

    bool WriteChunked(VL53L8CX_Platform *p_platform,
                     uint16_t reg_addr,
                     const uint8_t* data,
                     size_t total_size) {
        auto config = static_cast<coralmicro::I2cConfig*>(p_platform->i2c_config);
        size_t bytes_written = 0;
        
        while (bytes_written < total_size) {
            size_t chunk_size = std::min(kMaxChunkSize, total_size - bytes_written);
            std::vector<uint8_t> chunk_buffer(chunk_size + 2);
            
            // Prepare register address
            chunk_buffer[0] = static_cast<uint8_t>((reg_addr + bytes_written) >> 8);
            chunk_buffer[1] = static_cast<uint8_t>((reg_addr + bytes_written) & 0xFF);
            memcpy(&chunk_buffer[2], data + bytes_written, chunk_size);
            
            if (!coralmicro::I2cControllerWrite(*config, p_platform->address, 
                                               chunk_buffer.data(), chunk_size + 2)) {
                return false;
            }
            
            bytes_written += chunk_size;
        }
        return true;
    }

    bool ReadChunked(VL53L8CX_Platform *p_platform,
                    uint16_t reg_addr,
                    uint8_t* data,
                    size_t total_size) {
        auto config = static_cast<coralmicro::I2cConfig*>(p_platform->i2c_config);
        size_t bytes_read = 0;
        
        while (bytes_read < total_size) {
            size_t chunk_size = std::min(kMaxChunkSize, total_size - bytes_read);
            uint16_t current_addr = reg_addr + bytes_read;
            
            uint8_t addr_bytes[2];
            addr_bytes[0] = static_cast<uint8_t>(current_addr >> 8);
            addr_bytes[1] = static_cast<uint8_t>(current_addr & 0xFF);
            
            if (!coralmicro::I2cControllerWrite(*config, p_platform->address,
                                               addr_bytes, sizeof(addr_bytes)) ||
                !coralmicro::I2cControllerRead(*config, p_platform->address,
                                             data + bytes_read, chunk_size)) {
                return false;
            }
            
            bytes_read += chunk_size;
        }
        return true;
    }
}

extern "C" {
    uint8_t VL53L8CX_RdByte(VL53L8CX_Platform *p_platform,
                            uint16_t RegisterAddress,
                            uint8_t *p_value) {
        if (!p_platform || !p_platform->i2c_config || !p_value) return 1;
        
        uint8_t addr_bytes[2];
        addr_bytes[0] = static_cast<uint8_t>(RegisterAddress >> 8);
        addr_bytes[1] = static_cast<uint8_t>(RegisterAddress & 0xFF);
        
        auto config = static_cast<coralmicro::I2cConfig*>(p_platform->i2c_config);
        return (!coralmicro::I2cControllerWrite(*config, p_platform->address, addr_bytes, sizeof(addr_bytes)) ||
                !coralmicro::I2cControllerRead(*config, p_platform->address, p_value, 1)) ? 1 : 0;
    }

    uint8_t VL53L8CX_WrByte(VL53L8CX_Platform *p_platform,
                            uint16_t RegisterAddress,
                            uint8_t value) {
        if (!p_platform || !p_platform->i2c_config) return 1;
        
        uint8_t buffer[3];
        buffer[0] = static_cast<uint8_t>(RegisterAddress >> 8);
        buffer[1] = static_cast<uint8_t>(RegisterAddress & 0xFF);
        buffer[2] = value;
        
        auto config = static_cast<coralmicro::I2cConfig*>(p_platform->i2c_config);
        return !coralmicro::I2cControllerWrite(*config, p_platform->address, buffer, sizeof(buffer)) ? 1 : 0;
    }

    uint8_t VL53L8CX_WrMulti(VL53L8CX_Platform *p_platform,
                             uint16_t RegisterAddress,
                             uint8_t *p_values,
                             uint32_t size) {
        if (!p_platform || !p_platform->i2c_config || !p_values || size == 0) return 1;
        return !WriteChunked(p_platform, RegisterAddress, p_values, size) ? 1 : 0;
    }

    uint8_t VL53L8CX_RdMulti(VL53L8CX_Platform *p_platform,
                             uint16_t RegisterAddress,
                             uint8_t *p_values,
                             uint32_t size) {
        if (!p_platform || !p_platform->i2c_config || !p_values || size == 0) return 1;
        return !ReadChunked(p_platform, RegisterAddress, p_values, size) ? 1 : 0;
    }

    void VL53L8CX_SwapBuffer(uint8_t *buffer, uint16_t size) {
        if (!buffer || size < 4) return;
        
        for (uint16_t i = 0; i < size - 3; i += 4) {
            std::swap(buffer[i], buffer[i + 3]);
            std::swap(buffer[i + 1], buffer[i + 2]);
        }
    }

    uint8_t VL53L8CX_WaitMs(VL53L8CX_Platform *p_platform,
                            uint32_t TimeMs) {
        (void)p_platform;
        vTaskDelay(pdMS_TO_TICKS(TimeMs));
        return 0;
    }
}

namespace vl53l8cx {
    bool PlatformInit(VL53L8CX_Platform* p_platform,
                     coralmicro::I2c bus,
                     uint16_t address) {
        if (!p_platform) return false;
        
        auto config = new coralmicro::I2cConfig(coralmicro::I2cGetDefaultConfig(bus));
        if (!config) return false;
        
        // Set to 400 kHz I2C speed
        config->controller_config.baudRate_Hz = 400'000;
        
        if (!coralmicro::I2cInitController(*config)) {
            delete config;
            return false;
        }
        
        p_platform->i2c_config = config;
        p_platform->address = address;
        
        return true;
    }
}