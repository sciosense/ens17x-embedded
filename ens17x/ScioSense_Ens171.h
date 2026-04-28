#ifndef SCIOSENSE_ENS171_C_H
#define SCIOSENSE_ENS171_C_H

#include "ScioSense_Ens171_defines.h"

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct ScioSense_Ens17x_IO
{
    Result  (*read)     (void* config, const uint16_t address, uint8_t* data, const size_t size);
    Result  (*write)    (void* config, const uint16_t address, uint8_t* data, const size_t size);
    void    (*wait)     (const uint32_t ms);
    void* config;
} ScioSense_Ens17x_IO;

typedef struct ScioSense_Ens17x
{
    ScioSense_Ens17x_IO io;
    Ens17x_OperatingMode    operatingMode;
    uint16_t                partId;
    uint8_t                 id[ENS17X_BUFFER_INFO_ID_SIZE];
    uint8_t                 dateCode[ENS17X_BUFFER_INFO_DATE_CODE_SIZE];
    uint8_t                 firmwareVersion [ENS17X_BUFFER_INFO_FW_VER_SIZE];
    uint8_t                 resistanceBuffer[ENS17X_BUFFER_INFO_RESISTANCE_DATA_SIZE];
} ScioSense_Ens17x;

static inline Result Ens17x_Init                                  (ScioSense_Ens17x* ens17x);     // Resets the device to IDLE and reads PartID and FirmwareVersion
static inline Result Ens17x_Reset                                 (ScioSense_Ens17x* ens17x);     // Device reset to the power-on configuration.
static inline Result Ens17x_StartMeasureStandardMode                      (ScioSense_Ens17x* ens17x);     // Starts the measurement operating mode(gas sensing).
static inline Result Ens17x_ReadPartId                            (ScioSense_Ens17x* ens17x);     // Reads the PartID of the device
static inline Result Ens17x_ReadFirmwareVersion                   (ScioSense_Ens17x* ens17x);     // Reads the FirmwareVersion; only available in IDLE OpMode
static inline Result Ens17x_ReadId                                (ScioSense_Ens17x* ens17x);     // Reads the ID: WN/XLOC/YLOC; only available in IDLE OpMode
static inline Result Ens17x_ReadDateCode                          (ScioSense_Ens17x* ens17x);     // Reads the DateCode; only available in IDLE OpMode
static inline Result Ens17x_SetOperatingMode                      (ScioSense_Ens17x* ens17x, const Ens17x_OperatingMode mode);
static inline Result Ens17x_Wait                                  (ScioSense_Ens17x* ens17x);     // Waits for a measurement to finish; Only allowed in MeasurementMode

static inline bool                Ens17x_IsConnected              (ScioSense_Ens17x* ens17x, uint16_t expectedPartId);     // Tests whether a device is connected; returns true if ReadPartId() returned 0x171A
static inline uint16_t            Ens17x_GetPartId                (ScioSense_Ens17x* ens17x);     // Returns the PartID of the device
static inline uint32_t            Ens17x_GetRs1                   (ScioSense_Ens17x* ens17x);     // Returns resistor 1 value;
static inline uint32_t            Ens17x_GetRs2                   (ScioSense_Ens17x* ens17x);     // Returns resistor 2 value;
static inline uint32_t            Ens17x_GetRs3                   (ScioSense_Ens17x* ens17x);     // Returns resistor 3 value;
static inline uint8_t*            Ens17x_GetId                    (ScioSense_Ens17x* ens17x);     // Returns the ID: WN/XLOC/YLOC.
static inline uint8_t*            Ens17x_GetFirmwareVersion       (ScioSense_Ens17x* ens17x);     // Returns the firmware version number as uint8_t[3] {major, minor, release}

#include "ScioSense_Ens171.inl.h"
#endif // SCIOSENSE_ENS171_C_H