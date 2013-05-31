/*
 * FPGA Interface C API example for GCC for computers running Linux.
 *
 * NOTE: In order to run this example, you must compile a LabVIEW FPGA bitfile
 *       and generate a C API for it. For more information about using this
 *       example, refer to the Examples topic of the FPGA Interface C API Help,
 *       located under
 *       Start>>All Programs>>National Instruments>>FPGA Interface C API.
 */

#include "NiFpga_CTest.h"
#include <stdio.h>
#include <stdlib.h>
int main()
{
   /* must be called before any other calls */
   printf("Initializing...\n");
   NiFpga_Status status = NiFpga_Initialize();
   if (NiFpga_IsNotError(status))
   {
      NiFpga_Session session;
      /* opens a session, downloads the bitstream, and runs the FPGA */
      printf("Opening a session...\n");
      NiFpga_MergeStatus(&status, NiFpga_Open(NiFpga_CTest_Bitfile,
                                              NiFpga_CTest_Signature,
                                              "rio://146.6.84.143/RIO0",
                                              NiFpga_OpenAttribute_NoRun,
                                              &session));
      if (NiFpga_IsNotError(status))
      {
	 int		user_rate = 0;
         uint8_t	led = 0; 	// Digital Pin 0 
	 uint32_t 	rate = 0; 	// User Input
	 int 		i = 0; 

         /* run the FPGA application */
         printf("Running the FPGA...\n");
         NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));	


         /* Toggles led1 */
	 printf("Set toggle rate\n");
	 scanf("%d", &user_rate);
	 rate = user_rate;
	 NiFpga_MergeStatus(&status, NiFpga_WriteI16(session,NiFpga_CTest_ControlU32_CountmSec,rate)); 
   
for(i =0; i<50; i++) 
	{
	printf("Loop: %d Led: %d \n",  i, led); 
        led = !led;
	NiFpga_WriteBool(session, NiFpga_CTest_ControlBool_DIO0,led);
	NiFpga_ReadBool(session, NiFpga_CTest_IndicatorBool_DIO0, &led);
	printf("Loop: %d Led: %d \n",  i, led);  
	sleep(1);    	
        }


     	 printf("Press <Enter> to stop and quit...");
         getchar(); 
         /* stop the FPGA loops */
         printf("Stopping the FPGA...\n");
         NiFpga_MergeStatus(&status, NiFpga_WriteU32(session,
                                                     NiFpga_CTest_ControlBool_stop,
                                                     1));
	 /* close the session now that we're done */
         printf("Closing the session...\n");
         NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
      }
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
