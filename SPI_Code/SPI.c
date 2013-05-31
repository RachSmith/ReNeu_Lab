#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "SPI.h"

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
#define ENABLE 	1
#define BURN 	8
#define VERIFY	64

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

uint16_t spiTransfer(uint16_t command,NiFpga_Session session)
{
   uint8_t 	w_bit; 	// Bit to be sent to MOSI
   uint8_t 	*r_ptr; // Bit received from MISO
   uint16_t     test_w;
   uint16_t     test_r; 
   uint8_t	r_bit = 0; 
   uint16_t 	data = 0x0000;  /// data receive
   int16_t	i;

   NiFpga_WriteBool(session, NiFpga_SPI_Test_ControlBool_SS, 0); /// Activates AS5408
   nanosleep(20); 
   r_ptr = &r_bit;

   for(i = 15 ; i >= 0; i--) 
   { 
      test_w = (1<<i)&command; /// MSB sent first4004
      w_bit = (uint8_t)test_w && 0x01; /// make into boolean value   
      
      NiFpga_WriteBool(session, NiFpga_SPI_Test_ControlBool_MOSI, w_bit);
      NiFpga_WriteBool(session, NiFpga_SPI_Test_ControlBool_SCK, 1);/// Clock high to receive data
      nanosleep(20);
      NiFpga_ReadBool(session, NiFpga_SPI_Test_IndicatorBool_MISO, r_ptr);  
      nanosleep(30); 
      NiFpga_WriteBool(session, NiFpga_SPI_Test_ControlBool_SCK, 0); /// Set clock to low
      nanosleep(50);
      test_r = r_bit << i; 
      data |= test_r; 
  } 

   
   NiFpga_WriteBool(session, NiFpga_SPI_Test_ControlBool_SS, 1); /// Resets parts of interface core;
   return data; 
}



/* Send Command to read data, checks for error, send NOP.  
   Angle value returned as doube in degree form. 
   Only outputs value from 0-360, which is the current angle */
/* rewritten to only read angle */
    
double readAngle(NiFpga_Session session) 
{ 
   double angle = 0;   
   uint16_t data = 0xFFFF;
   uint16_t error = 0; 
 
   data = spiTransfer(ANG_R, session);
   error = data & 0x4000; 

   /* checking data for error flag*/
   if( error != 0) 
	{
	  printf("Error in read\n");
          spiTransfer(NOP, session);
          return 0xFFFF;  
	}
   
   data = data & 0x3FFF;  // removing parity bit and error bit 
   angle = data*(360.0/16383.0); // Converting to degree
   return angle; 

   
   ///////
   /*data = spiTransfer(ANG_R, session);

   data &= 0x3FFF;
   /*check for last NOP command 
   if (data != 4) 
   { printf("Error, data =%x \n", data); }
  


   /* data is now from angle read 
   data = spiTransfer(ERR_R, session);
   error = data & 0x4000; 

   /* checking data for error flag
   if( error != 0) 
	{
	  printf("Error in read\n");
          spiTransfer(NOP, session);
          return 0xFFFF;  
	}
   
   data = data & 0x3FFF;  // removing parity bit and error bit 
   angle = data*(360.0/16383.0); // Converting to degree

   data = spiTransfer(NOP, session); 
   data &= 0x0007;
   switch (data) 
   {
      case 0:
	 return angle; 
      case 1: 
         printf("Framing Error\n");
	 break;  
      case 2:
 	 printf("Command Error\n");  
	 break;
      case 4:
	 printf("Parity Error\n"); 
      default:
	 printf("Unknown Error\n"); 
   }
    
   return 0xFFFF; 
  */
}




/* Send Command to read data, checks for error, send NOP.  */ 
double readMag(NiFpga_Session session) 
{ 
   float mag = 0xFFFF; 
   uint16_t data = 0xFFFF; 
   uint16_t error =0;
   int16_t sign; 

   data = spiTransfer(MAG_R, session);

   data &= 0x3FFF;
   /* check for last NOP command */
   if(data != 4)
   { printf("Error, data = %x \n",data); }
  

   /* Data is from magnitude read */
   data = spiTransfer(ERR_R,session);
   error = data & 0x4000; 
   /* checking data for error flag*/
   if(error != 0) 
	{
	  printf("Error in read\n");
          ///spiTransfer(NOP, session);
          return 0xFFFF;  
	}
  
   data = data & 0x3FFF;  // removing parity bit and error bit 
   mag = data*(360.0/16383.0); 
   data = spiTransfer(NOP, session);

   switch (data) 
   {
      case 0:
	 return mag; 
      case 1: 
         printf("Framing Error\n");
	 break;  
      case 2:
 	 printf("Command Error\n");  
	 break;
      case 4:
	 printf("Parity Error\n"); 
      default:
	 printf("Unknown Error\n");
   }
   
   return 0xFFFF; 
}

uint16_t setZero(uint16_t current,NiFpga_Session session)
{  
   uint16_t data; 
   uint16_t hi_byte;
   uint16_t low_byte;
   uint16_t new;

   hi_byte = current & 0x3FC0;  /// 0011 1111 1100 0000
   hi_byte = hi_byte >> 6;
   low_byte = current & 0x003F; /// 0000 0000 0011 1111 
   

   data = spiTransfer(PRO_W,session);
   if(data !=0)
   { printf("Error in last command"); } 

   /*Enable programming */
   data = spiTransfer(ENABLE,session);
   data = spiTransfer(BURN,session);
   data = spiTransfer(VERIFY,session); 
   
   /*Set current to zero*/
   data = spiTransfer(ZHI_W,session);
   data = spiTransfer(hi_byte,session);
   data = spiTransfer(ZLOW_W,session);
   data = spiTransfer(low_byte,session); 

   /*Disable programming */
   data = spiTransfer(PRO_W,session); 
   data = spiTransfer(ENABLE,session);
   data = spiTransfer(0,session); 
   
   /*check*/
   data = spiTransfer(ZHI_R,session);
   hi_byte = spiTransfer(ZLOW_R,session);
   low_byte = spiTransfer(NOP,session);
   hi_byte = hi_byte << 6;
   new = hi_byte | low_byte; 
   if(new != current)
   { printf("error in setting zero"); }

}





