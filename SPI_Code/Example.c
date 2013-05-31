/*
 * FPGA Interface C API example for GCC for computers running Linux.
 *
 * NOTE: In order to run this example, you must compile a LabVIEW FPGA bitfile
 *       and generate a C API for it. For more information about using this
 *       example, refer to the Examples topic of the FPGA Interface C API Help,
 *       located under
 *       Start>>All Programs>>National Instruments>>FPGA Interface C API.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NiFpga.h"
#include "NiFpga_SPI_Test.h"
#include "SPI.h"  

int main()
{
   
   double Angle = 0;
   double ZeroAngle = 0;
   double ZeroMag = 0;   
   double Mag =0xFFFF;
   int i =0; 

   /* must be called before any other calls */
   printf("Initializing...\n");
   NiFpga_Status status = NiFpga_Initialize();

   if (NiFpga_IsNotError(status))
   {
      NiFpga_Session session;
      /* opens a session, downloads the bitstream, and runs the FPGA */
      printf("Opening a session...\n");
      NiFpga_MergeStatus(&status, NiFpga_Open(NiFpga_SPI_Test_Bitfile,
                                              NiFpga_SPI_Test_Signature,
                                              "rio://146.6.84.207/RIO0",
                                              NiFpga_OpenAttribute_NoRun,
                                              &session));
                                              
      if (NiFpga_IsNotError(status))
      {

         /* run the FPGA application */
         printf("Running the FPGA...\n");
 	 NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));	
         printf("Starting Program\n"); 
	 ///spiTransfer(NOP, session);	 // Sends NOP command to clear any leftover data
         spiTransfer(ANG_R,session);     // Sends first command, when reading angle only 
         ZeroAngle = readAngle(session); // Setting variable to first position to get change in position, must 
         ZeroMag = readMag(session);     // set before reading angle values

for(i=0; i<15; i++)
{
	
        Angle = readAngle(session) - ZeroAngle;  // Gets current position and offsets due to inital position
        if(Angle < 0) 				 // If case of negitive value, makes positive
	{Angle += 360;} 			 
        printf("Angle=%lf \n", Angle);        
       // Mag = readMag(session) - ZeroMag;
       // printf("Magnitude=%lf \n", Mag); 
       
} 
         ///spiTransfer(NOP, session);
     	 printf("Press <Enter> to stop and quit...");
         getchar(); 
         /* stop the FPGA loops */
         printf("Stopping the FPGA...\n");
         NiFpga_MergeStatus(&status, NiFpga_WriteU32(session,
                                                     NiFpga_SPI_Test_ControlBool_stop,
                                                     1));
	 /* close the session now that we're done */
         printf("Closing the session...\n");
         NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
      }
else printf("Opps error"); 
      /* must be called after all other calls */
      printf("Finalizing...\n");
      NiFpga_MergeStatus(&status, NiFpga_Finalize());
   }
   /* check if anything went wrong */
   if (NiFpga_IsError(status))
   {
      printf("Error %d!\n", status);
      printf("Press <Enter> to quit...\n");
      getchar();
   }
   return status;
}
