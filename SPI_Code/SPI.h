
#ifndef SPI_h
#define SPI_h

#include "NiFpga.h"
#include "NiFpga_SPI_Test.h"

/* Command Package: SPI NOP-read only
No operation dummy information */
#define	NOP 16384   	/// 0x4000		

/* Command Package: Clear Error Flag-read only 
All errors are cleared by access	
Error Registers:
-Parity Error		BIT2 
-Command Invaild	BIT1
-Framing Error		BIT0			*/
#define	ERR_R 16385	/// 0x4001

/* Command Package: Programming Control-Write
Programming must be enabled be enable before burning. 
After programming verification is mandatory.
Programming Registers:
-Verify			BIT6
-Burn			BIT3
-Reserved		BIT2 & BIT1
-Programming Enable	BIT0			*/
#define PRO_W 32771	/// 0x8003
			
/* Command Package: Programming Control-Read
Programming must be enabled be enable before burning. 
After programming verification is mandatory.
Programming Registers:
-Verify			BIT6
-Burn			BIT3
-Reserved		BIT2 & BIT1
-Programming Enable	BIT0			*/
#define PRO_R 49155	/// 0xC003

/* Command Package: Zero Position Hi-Write
Zero Position value high byte
OPT Registers:
-Zero Position <13>	BIT7
- ...			...
-Zero Position <6>	BIT0			*/
#define ZHI_W 22  	/// 0x0016

/* Command Package: Zero Position Hi-Read
Zero Position value high byte
OPT Registers:
-Zero Position <13>	BIT7
- ...			...
-Zero Position <6>	BIT0			*/	
#define ZHI_R 16406	/// 0x4016

/* Command Package: Zero Position Low-Write
Zero Position value low six values
OPT Registers:
-Zero Position <5>	BIT5
- ...			...
-Zero Postion <0>	BIT0  			*/
#define ZLOW_W 32791	/// 0x8017

/* Command Package: Zero Position Low-Read
Zero Position value low six values
OPT Registers:
-Zero Position <5>	BIT5
- ...			...
-Zero Postion <0>	BIT0  			*/
#define ZLOW_R 49175	/// 0xC017

/* Command Package: Diagnostics and Automatic Gain Control-Read only
Diagnostics flags:
-Comp High		BIT11 
-Comp Low		BIT10 
-COF			BIT9 
-OCF			BIT8 (default 1)

Automatic Gain Control Value:
-AGC Vaule <7>		BIT7 (default 1)
-...			...
-AGC Vaule <0>		BIT0 
(0 represents high magnetic field, 255 represents low magnetic field) */ 
#define GAIN_R 32765	/// 0x7FFD

/* Command Package: Magnitude-read only 
Registers: 
-Magnitude <13> 	BIT13
-...			...
-Magnitude <0>		BIT0
(Magnitude information after ATAN calculation)	*/
#define MAG_R 32766	/// 0x7FFE

/* Command Package: Angle-read only
Registers:
-Angle <13>		BIT13
-...			...
-Angle <0>		BIT0
(Angle information after ATAN calculation and zero position adder) */
#define ANG_R 65535	/// 0xFFFF

uint16_t spiTransfer(uint16_t, NiFpga_Session);
double readAngle(NiFpga_Session); 
double readMag(NiFpga_Session);


#endif

