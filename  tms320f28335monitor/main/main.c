//###########################################################################
//
// FILE:   main.c
//
// TITLE:  SSM_ DSPMATE  28335 Monitor Main
//
// DESCRIPTION:
//	    				
//         Jeon yu hun  
//
//###########################################################################
// $Release Date: 11-25, 2008 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


void main(void)
{
	char RcvData;
	
	// Step 1. Initialize System Control:
	// PLL, WatchDog, enable Peripheral Clocks
	// This example function is found in the DSP2833x_SysCtrl.c file.
	InitSysCtrl();

	// Step 2. Initalize GPIO:
	// This example function is found in the DSP2833x_Gpio.c file and
	// illustrates how to set the GPIO to it's default state.
	InitGpio();
	InitXintf();
	// Step 3. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT;
	
	// Initialize PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	// This function is found in the DSP2833x_PieCtrl.c file.

	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	
	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
	// This function is found in DSP2833x_PieVect.c.
	InitPieVectTable();

	InitFlash();
	InitSystem();

	PrintMenu();
	TxPrintf("\nMonitor28335]#");


	for(;;)
	{
		
		RcvData = SCIa_RxChar();
		SCIa_TxChar(RcvData);
		switch(RcvData)
		{
			case 'M':
			case 'm':
				PrintMenu();
				break;
				
			case 'A':
			case 'a':
				Erase_AllFlash();
				break;
				
			case 'O':
			case 'o':
				Erase_SelectFlash();
				break;
				
			case 'D':
			case 'd':
				DownFromSCI();
				break;
				
			case 'b':
			case 'B':
				FlashBurnPrm();
				FlashtoRamDownloadPrm();

			case 'G':
			case 'g':
				FlashtoRamDownloadPrm();
				break;

			case CR:
				break;

			case ESC:
				TxPrintf("\n ESC !!\n");
				break;
				
			default:
				TxPrintf("\n=======Wrong Command!!========\n");
				PrintMenu();
				break;
		}
		
		TxPrintf("\nMonitor28335]#");


		
	}
}


void InitSystem(void)
{
	InitSci();
	InitFlashAPI();
//	InitVFDBright(VfdBright20);
}

void PrintMenu(void)
{
	TxPrintf("\n");
	TxPrintf("========   TMS320F28335 Monitor V0.1   ========\n");	
	TxPrintf("  M  :	Display Menu.\n");
	TxPrintf("  A  :	Erase All Flash.( Sector B - H )\n");
	TxPrintf("  O  :	Erase Select Sector Flash. ( ex. O B )\n");
	TxPrintf("  D  :	RAM down  User Program ( *.Hex )\n");
	TxPrintf("  B  :	Flash down  User Program ( *.Hex )\n");
	TxPrintf("  G  :	Go User Program\n");
	TxPrintf("========   SSM 18th     Team DSPMATE ========\n");	
}

