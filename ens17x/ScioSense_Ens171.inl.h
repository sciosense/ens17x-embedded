#ifndef SCIOSENSE_ENS171_C_INL
#define SCIOSENSE_ENS171_C_INL

#include "ScioSense_Ens171.h"
#include "ScioSense_Ens171_defines.h"
#include <math.h>

#define write(address, data, len)   ens17x->io.write(ens17x->io.config, (address), (uint8_t*)(data), (len))
#define wait(ms)                    ens17x->io.wait(ms)

#define hasAnyFlag(a, b)    (((a) & (b)) != 0)
#define hasFlag(a, b)       (((a) & (b)) == (b))

static inline Result Ens17x_Invoke(ScioSense_Ens17x* ens17x, const Ens17x_Command command, uint32_t ms);

static inline Result Ens17x_Read(ScioSense_Ens17x* ens17x, const uint16_t address, uint8_t* data, const size_t size)
{
    Result result;

    result= ens17x->io.read(ens17x->io.config, address, data, size);

    return result;
}

static inline Result Ens17x_Init(ScioSense_Ens17x* ens17x)
{
    Result result;

    result = Ens17x_Reset(ens17x);
    if (result != RESULT_OK)
    {
        return result;
    }

    result = Ens17x_ReadPartId(ens17x);
    if (result != RESULT_OK)
    {
        return result;
    }

    result = Ens17x_ReadId(ens17x);
    if (result != RESULT_OK)
    {
        return result;
    }

    result = Ens17x_ReadDateCode(ens17x);
    if (result != RESULT_OK)
    {
        return result;
    }

    result = Ens17x_ReadFirmwareVersion(ens17x);
    if (result != RESULT_OK)
    {
        return result;
    }

    return result;
}

static inline Result Ens17x_Reset(ScioSense_Ens17x* ens17x)
{
    Result result;

    result = Ens17x_SetOperatingMode(ens17x, ENS17X_OPERATING_MODE_RESET);
    if (result != RESULT_OK)
    {
        return result;
    }

    wait(ENS17X_SYSTEM_TIMING_RESET_DELAY);

    result = Ens17x_SetOperatingMode(ens17x, ENS17X_OPERATING_MODE_IDLE);
    if (result != RESULT_OK)
    {
        return result;
    }

    wait(ENS17X_SYSTEM_TIMING_GENERAL_PURPOSE_DELAY);

    return result;
}

static inline Result Ens17x_Update(ScioSense_Ens17x* ens17x)
{
    Result result = Ens17x_Read(ens17x, ENS17X_REGISTER_ADDRESS_RS1, ens17x->resistanceBuffer, sizeof(ens17x->resistanceBuffer));
 
    return result;
}

static inline Result Ens17x_StartMeasureStandardMode(ScioSense_Ens17x* ens17x)
{
    return Ens17x_SetOperatingMode(ens17x, ENS17X_OPERATING_MODE_MEASURE);
}

static inline Result Ens17x_ReadFirmwareVersion(ScioSense_Ens17x* ens17x)
{
    Result result;

    result = Ens17x_Invoke(ens17x, ENS17X_COMMAND_READ_FW_VER, ENS17X_SYSTEM_TIMING_GENERAL_PURPOSE_DELAY);
    if (result == RESULT_OK)
    {
        result = Ens17x_Read(ens17x, ENS17X_REGISTER_ADDRESS_FW_VER, ens17x->firmwareVersion, sizeof(ens17x->firmwareVersion));
    }

    return result;
}

static inline Result Ens17x_ReadId(ScioSense_Ens17x* ens17x)
{
    Result result;

    result = Ens17x_Invoke(ens17x, ENS17X_COMMAND_READ_ID, ENS17X_SYSTEM_TIMING_GENERAL_PURPOSE_DELAY);
    if (result == RESULT_OK)
    {
        result = Ens17x_Read(ens17x, ENS17X_REGISTER_ADDRESS_ID, ens17x->id, sizeof(ens17x->id));
    }

    return result;
}

static inline Result Ens17x_ReadDateCode(ScioSense_Ens17x* ens17x)
{
    Result result;

    result = Ens17x_Invoke(ens17x, ENS17X_COMMAND_READ_DATE_CODE, ENS17X_SYSTEM_TIMING_GENERAL_PURPOSE_DELAY);
    if (result == RESULT_OK)
    {
        result = Ens17x_Read(ens17x, ENS17X_REGISTER_ADDRESS_DATE_CODE, ens17x->dateCode, sizeof(ens17x->dateCode));
    }

    return result;
}

static inline Result Ens17x_ReadPartId(ScioSense_Ens17x* ens17x)
{
    Result result;
    uint8_t partIdData[2];

    result= Ens17x_Read(ens17x, ENS17X_REGISTER_ADDRESS_PART_ID, partIdData, 2);
    if (result == RESULT_OK)
    {
        ens17x->partId = ((uint16_t)partIdData[1] << 8) + partIdData[0];
    }

    return result;
}

static inline Result Ens17x_SetOperatingMode(ScioSense_Ens17x* ens17x, const Ens17x_OperatingMode mode)
{
    Result result;

    // Before switching to a measurement mode, the idle mode must be activated.
    if (    (mode != ENS17X_OPERATING_MODE_IDLE)
        &&  (mode != ENS17X_OPERATING_MODE_RESET)
        &&  (ens17x->operatingMode != ENS17X_OPERATING_MODE_IDLE)
    )
    {
        result = Ens17x_SetOperatingMode(ens17x, ENS17X_OPERATING_MODE_IDLE);
        if (result != RESULT_OK)
        {
            return result;
        }
    }

    result = write(ENS17X_REGISTER_ADDRESS_OPMODE, &mode, sizeof(mode));
    if (result == RESULT_OK)
    {
        ens17x->operatingMode = mode;
    }
    
    return result;
}

static inline Result Ens17x_Wait(ScioSense_Ens17x* ens17x)
{
    switch (ens17x->operatingMode)
    {
        case ENS17X_OPERATING_MODE_MEASURE : wait(ENS17X_SYSTEM_TIMING_STANDARD_MEASURE_DELAY); break;
        default                             : return RESULT_NOT_ALLOWED;
    }

    return RESULT_OK;
}

static inline Result Ens17x_Invoke(ScioSense_Ens17x* ens17x, const Ens17x_Command command, uint32_t ms)
{
    Result result = RESULT_OK;

    if(ens17x->operatingMode != ENS17X_OPERATING_MODE_IDLE)
    {
        result = Ens17x_SetOperatingMode(ens17x, ENS17X_OPERATING_MODE_IDLE);
        if (result != RESULT_OK)
        {
            return result;
        }
    }

    if (result == RESULT_OK)
    {
        uint8_t cmd = ENS17X_COMMAND_CLEAR_GPR_REGISTERS;
        write(ENS17X_REGISTER_ADDRESS_COMMAND, &cmd, sizeof(cmd)); // Clear GPR registers before invoking the command to make sure that the data is up to date and not from a previous command.

        cmd = ENS17X_COMMAND_NOP;
        write(ENS17X_REGISTER_ADDRESS_COMMAND, &cmd, sizeof(cmd));

        write(ENS17X_REGISTER_ADDRESS_COMMAND, &command, sizeof(command));
        wait(ms);
    }

    return result;
}

static inline bool Ens17x_IsConnected(ScioSense_Ens17x* ens17x, uint16_t expectedPartId)
{
    return ens17x->partId == expectedPartId;
}

static inline uint32_t calculateResistance(const uint16_t r)
{
    return (uint32_t)pow(2, (float)(r) / 2048);
}

static inline uint32_t Ens17x_GetRs1(ScioSense_Ens17x* ens17x)
{
    uint16_t r = (ens17x->resistanceBuffer[ENS17X_BUFFER_INFO_RS1_INDEX + 1] << 8) + ens17x->resistanceBuffer[ENS17X_BUFFER_INFO_RS1_INDEX];
    return calculateResistance(r);
}

static inline uint32_t Ens17x_GetRs2(ScioSense_Ens17x* ens17x)
{
    uint16_t r = (ens17x->resistanceBuffer[ENS17X_BUFFER_INFO_RS2_INDEX + 1] << 8) + ens17x->resistanceBuffer[ENS17X_BUFFER_INFO_RS2_INDEX];
    return calculateResistance(r);
}

static inline uint32_t Ens17x_GetRs3(ScioSense_Ens17x* ens17x)
{
    uint16_t r = (ens17x->resistanceBuffer[ENS17X_BUFFER_INFO_RS3_INDEX + 1] << 8) + ens17x->resistanceBuffer[ENS17X_BUFFER_INFO_RS3_INDEX];
    return calculateResistance(r);
}

static inline uint8_t* Ens17x_GetFirmwareVersion(ScioSense_Ens17x* ens17x)
{
    return ens17x->firmwareVersion;
}

static inline uint16_t Ens17x_GetPartId(ScioSense_Ens17x* ens17x)
{
    return ens17x->partId;
}

static inline uint8_t* Ens17x_GetId(ScioSense_Ens17x* ens17x)
{
    return ens17x->id;
}


#undef write
#undef wait
#undef hasAnyFlag
#undef hasFlag

#endif // SCIOSENSE_ENS171_C_INL