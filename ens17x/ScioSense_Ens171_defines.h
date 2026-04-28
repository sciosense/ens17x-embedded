#ifndef SCIOSENSE_ENS170_DEFINES_C_H
#define SCIOSENSE_ENS170_DEFINES_C_H

#include <inttypes.h>

//// Register Address (checked)
typedef uint8_t Ens17x_RegisterAddress;
#define ENS17X_REGISTER_ADDRESS_PART_ID        (0x00)                                   // This 2-byte register contains the part number in little endian of the ENS17x.

#define ENS17X_REGISTER_ADDRESS_ID             (0x4D)                                   // This 3-byte register contains the ID: WN/XLOC/YLOC.
#define ENS17X_REGISTER_ADDRESS_DATE_CODE      (0x48)                                   // This 8-byte register contains the date code Datecode Byte[0:7].
#define ENS17X_REGISTER_ADDRESS_FW_VER         (0x4C)                                   // This 3-byte register contains the firmware version in little endian. The first byte is the major version, the second byte is the minor version, and the third byte is the patch version.

#define ENS17X_REGISTER_ADDRESS_OPMODE         (0x10)                                   // This 1-byte register sets the Operating Mode of the ENS17x. The Host System can write a new OPMODE at any time.
#define ENS17X_REGISTER_ADDRESS_COMMAND        (0x12)                                   // This 1-byte register allows some additional commands to be executed on the ENS17x

#define ENS17X_REGISTER_ADDRESS_RS1            (0x4A)                                   // This 2-byte register contains the raw value of sensor element 1 in little endian. and can only be read in OPMODE MEASUREMENT.
#define ENS17X_REGISTER_ADDRESS_RS2            (0x4C)                                   // This 2-byte register contains the raw value of sensor element 2 in little endian. and can only be read in OPMODE MEASUREMENT.
#define ENS17X_REGISTER_ADDRESS_RS3            (0x4E)                                   // This 2-byte register contains the raw value of sensor element 3 in little endian. and can only be read in OPMODE MEASUREMENT.

//// Operating Mode
typedef uint8_t Ens17x_OperatingMode;
#define ENS17X_OPERATING_MODE_IDLE             (0x01)                                   // IDLE mode
#define ENS17X_OPERATING_MODE_MEASURE          (0x02)                                   // MEASURE mode
#define ENS17X_OPERATING_MODE_RESET            (0xF0)                                   // RESET (software reset)

//// Command
typedef uint8_t Ens17x_Command;
#define ENS17X_COMMAND_NOP                              (0x00)                         // Clears the Command Register (ENS171_COMMAND_NOP)
#define ENS17X_COMMAND_READ_ID                          (0x0C)                         // get ID
#define ENS17X_COMMAND_READ_FW_VER                      (0x0E)                         // get FW Version 
#define ENS17X_COMMAND_READ_DATE_CODE                   (0xDC)                         // get Datecode
#define ENS17X_COMMAND_CLEAR_GPR_REGISTERS              (0xCC)                         // Clears GPR Read Registers

//// SystemTiming in ms 
#define ENS17X_SYSTEM_TIMING_GENERAL_PURPOSE_DELAY                   (10)             
#define ENS17X_SYSTEM_TIMING_RESET_DELAY                             (50)             
#define ENS17X_SYSTEM_TIMING_STANDARD_MEASURE_DELAY                  (1000)           

//// BufferInfo defines internally used buffer and data indices and sizes (in bytes)
#define ENS17X_BUFFER_INFO_PART_ID_SIZE            (2)
#define ENS17X_BUFFER_INFO_ID_SIZE                 (3)
#define ENS17X_BUFFER_INFO_DATE_CODE_SIZE          (8)
#define ENS17X_BUFFER_INFO_FW_VER_SIZE             (3)
#define ENS17X_BUFFER_INFO_RESISTANCE_DATA_SIZE    (6) // each element is 2 bytes, and there are 3 elements (RS1, RS2, RS3)

#define ENS17X_BUFFER_INFO_RS1_INDEX               0
#define ENS17X_BUFFER_INFO_RS2_INDEX               2
#define ENS17X_BUFFER_INFO_RS3_INDEX               4

//// Result and Errors
#ifndef SCIOSENSE_RESULT_CODES
#define SCIOSENSE_RESULT_CODES
typedef int8_t Result;
#define RESULT_NOT_ALLOWED      (4)     // The requested command is not allowed.
#define RESULT_CHECKSUM_ERROR   (3)     // The value was read, but the checksum over the payload (valid and data) does not match.
#define RESULT_INVALID          (2)     // The value was read, but the data is invalid.
#define RESULT_IO_ERROR         (1)     // There was an IO communication error, read/write the stream failed.
#define RESULT_OK               (0)     // All OK; The value was read, the checksum matches, and data is valid.
#endif

#endif // SCIOSENSE_ENS170_DEFINES_C_H

