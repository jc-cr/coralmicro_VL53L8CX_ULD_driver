#include "platform.h"
#include "libs/base/i2c.h"

using namespace coralmicro;

// Helper to get I2cConfig from opaque pointer
static inline I2cConfig& GetI2cConfig(VL53L8CX_Platform* p_platform) {
    return *static_cast<I2cConfig*>(p_platform->i2c_config);
}

// Helper function to write register address before read/write operations
static uint8_t WriteRegAddr(VL53L8CX_Platform *p_platform, uint16_t reg_addr) {
    uint8_t addr_bytes[2] = {
        static_cast<uint8_t>((reg_addr >> 8) & 0xFF),  // MSB
        static_cast<uint8_t>(reg_addr & 0xFF)          // LSB
    };
    
    return I2cControllerWrite(GetI2cConfig(p_platform), 
                            p_platform->address,
                            addr_bytes, 
                            sizeof(addr_bytes)) ? 0 : 1;
}

extern "C" 
{

    uint8_t VL53L8CX_RdByte(
        VL53L8CX_Platform *p_platform,
        uint16_t RegisterAddress,
        uint8_t *p_value) 
    {
        if (!p_platform || !p_value) return 1;

        if (WriteRegAddr(p_platform, RegisterAddress) != 0) {
            return 1;
        }

        if (!I2cControllerRead(GetI2cConfig(p_platform),
                            p_platform->address,
                            p_value, 
                            1)) {
            return 1;
        }

        return 0;
    }

    uint8_t VL53L8CX_WrByte(
        VL53L8CX_Platform *p_platform,
        uint16_t RegisterAddress,
        uint8_t value)
    {
        if (!p_platform) return 1;

        uint8_t data[3] = {
            static_cast<uint8_t>((RegisterAddress >> 8) & 0xFF),
            static_cast<uint8_t>(RegisterAddress & 0xFF),
            value
        };

        if (!I2cControllerWrite(GetI2cConfig(p_platform),
                            p_platform->address,
                            data,
                            sizeof(data))) {
            return 1;
        }

        return 0;
    }

    uint8_t VL53L8CX_WrMulti(
        VL53L8CX_Platform *p_platform,
        uint16_t RegisterAddress,
        uint8_t *p_values,
        uint32_t size)
    {
        if (!p_platform || !p_values || size == 0) return 1;

        uint8_t* buffer = new uint8_t[size + 2];
        if (!buffer) return 1;

        buffer[0] = static_cast<uint8_t>((RegisterAddress >> 8) & 0xFF);
        buffer[1] = static_cast<uint8_t>(RegisterAddress & 0xFF);
        memcpy(&buffer[2], p_values, size);

        bool success = I2cControllerWrite(GetI2cConfig(p_platform),
                                        p_platform->address,
                                        buffer,
                                        size + 2);
        
        delete[] buffer;
        return success ? 0 : 1;
    }

    uint8_t VL53L8CX_RdMulti(
        VL53L8CX_Platform *p_platform,
        uint16_t RegisterAddress,
        uint8_t *p_values,
        uint32_t size)
    {
        if (!p_platform || !p_values || size == 0) return 1;

        if (WriteRegAddr(p_platform, RegisterAddress) != 0) {
            return 1;
        }

        if (!I2cControllerRead(GetI2cConfig(p_platform),
                            p_platform->address,
                            p_values,
                            size)) {
            return 1;
        }

        return 0;
    }

    void VL53L8CX_SwapBuffer(
        uint8_t *buffer,
        uint16_t size)
    {
        if (!buffer || size < 4) return;

        uint32_t i, tmp;
        
        for(i = 0; i < size; i += 4) {
            if(i + 4 > size) break;
            
            tmp = ((uint32_t)buffer[i] << 24) | 
                ((uint32_t)buffer[i + 1] << 16) | 
                ((uint32_t)buffer[i + 2] << 8) | 
                (uint32_t)buffer[i + 3];
            
            buffer[i] = (tmp >> 24) & 0xFF;
            buffer[i + 1] = (tmp >> 16) & 0xFF;
            buffer[i + 2] = (tmp >> 8) & 0xFF;
            buffer[i + 3] = tmp & 0xFF;
        }
    }

    uint8_t VL53L8CX_WaitMs(
        VL53L8CX_Platform *p_platform,
        uint32_t TimeMs)
    {
        vTaskDelay(pdMS_TO_TICKS(TimeMs));
        return 0;
    }

} // extern "C"


extern "C++" bool VL53L8CX_PlatformInit(
    VL53L8CX_Platform *p_platform,
    I2c bus,
    uint16_t address)
{
    if (!p_platform) return false;

    p_platform->address = address;
    
    auto* config = new I2cConfig(I2cGetDefaultConfig(bus));
    if (!config) return false;
    
    p_platform->i2c_config = config;
    
    return I2cInitController(*config);
}
