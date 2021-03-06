/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_SPI_Test_h__
#define __NiFpga_SPI_Test_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_SPI_Test_Bitfile;
 */
#define NiFpga_SPI_Test_Bitfile "NiFpga_SPI_Test.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_SPI_Test_Signature = "E45F71F5DEACAC0307B0105CB7988035";

typedef enum
{
   NiFpga_SPI_Test_IndicatorBool_MISO = 0x8112,
} NiFpga_SPI_Test_IndicatorBool;

typedef enum
{
   NiFpga_SPI_Test_ControlBool_MOSI = 0x8116,
   NiFpga_SPI_Test_ControlBool_Power = 0x810E,
   NiFpga_SPI_Test_ControlBool_SCK = 0x811A,
   NiFpga_SPI_Test_ControlBool_SS = 0x8122,
   NiFpga_SPI_Test_ControlBool_stop = 0x811E,
} NiFpga_SPI_Test_ControlBool;

#endif
