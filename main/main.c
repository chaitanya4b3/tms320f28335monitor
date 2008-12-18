//###########################################################################
//
// FILE:   main.c
//
// TITLE:  SSM_ DSPMATE  28335 Monitor Main
//
// DESCRIPTION:
//	    				
// 		Jeon yu hun (dimbibara@gmail.com)
//
//  Copyright (C) 1993  Corey Minyard
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
// 
//
//###########################################################################
// $Release Date: 12-23, 2008 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//#define ReleaseVersion	0.1 // First make
//#define ReleaseVersion	0.2 // ram down complete
//#define ReleaseVersion	0.3 // flash down complete
//#define ReleaseVersion	0.4 // auto run complete
#define ReleaseVersion	0.5 // serial.c function modify

#define ONEWAITSEC		0.5  //auto run wait 1 time. full wait time = ONEWAITSEC * 3

interrupt void cpu_timer0_isr(void);

/*   MAIN   */
/*************************************************************************
*	@name    	main
*	@memo	       Monitor program Main
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
void main(void)
{
	char RcvData;
	
	// PLL, WatchDog, enable Peripheral Clocks
	InitSysCtrl();

	InitGpio();
	InitXintf();

	// Disable CPU interrupts
	DINT;
	
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Initialize PIE control registers to their default state.
	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	InitPieVectTable();
	
	// Interrupts that are used in this example are re-mapped to
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	InitFlash();
	InitSci();
	InitFlashAPI();
	InitCpuTimers();


#if (CPU_FRQ_150MHZ)
	// 150MHz CPU Freq, 1 second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, 150, ONEWAITSEC * 1000000);
#endif

#if (CPU_FRQ_100MHZ)
	// 100MHz CPU Freq, 1 second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, 100, ONEWAITSEC * 1000000);
#endif


	CpuTimer0Regs.TCR.bit.TSS = 0; // Use write-only instruction to set TSS bit = 0
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	EINT;   // Enable Global interrupt INTM



	// 자동 실행
	RcvData = SCIa_RxChar();
	DINT;   // Enable Global interrupt INTM				

	HelpMenu();
	SCIa_Printf("\nMonitor28335_ctml]#");

	for(;;)
	{
		
		RcvData = SCIa_RxChar();
		SCIx_TxChar(RcvData,&SciaRegs);
		switch(RcvData)
		{
			case 'h':
			case 'H':
				HelpMenu();
				break;

			case 'M':
			case 'm':
				PrintMenu();
				break;
				
			case 'A':
			case 'a':
				Erase_AllFlash();
				break;
				
			case 'S':
			case 's':
				Erase_SelectFlash();
				break;
				
			case 'R':
			case 'r':
				DownFromSCI();  // 씨리얼로 다운받아 바로 재배치한다. 
				break;
				
			case 'D':
			case 'd':
				SCItoFLASH();  // 씨리얼로 프로그램을 다운받아 FLASH에 저장한다.
				DownFromFlash(); // FLASH 에서 읽어와서 재배치 한다.

			case 'G':
			case 'g':
				DownFromFlash(); // FLASH 에서 읽어와서 재배치 한다.
				break;

			case CR:
				break;

			case ESC:
				break;
				
			default:
				SCIa_Printf("\n=======Wrong Command!!========\n");
				PrintMenu();
				break;
		} 
		
		SCIa_Printf("\nMonitor28335_ctml]#");
	}
}


/*   PRINTMENU   */
/*************************************************************************
*	@name    	PrintMenu
*	@memo	       모니터 프로그램 메뉴 출력
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
void PrintMenu(void)
{
	SCIa_Printf("\n\n");
	SCIa_Printf(" TMS320F28335 Monitor V%.1f ctml\n",ReleaseVersion);	
	SCIa_Printf("--------------------------------------------------------\n");		
	SCIa_Printf("  H  :	Display Help.\n");
	SCIa_Printf("  M  :	Display Menu.\n");
	SCIa_Printf("  A  :	Erase All Flash.( Sector B - H )\n");
	SCIa_Printf("  S  :	Select sector and Erase Flash sector. ( ex. S-B )\n");
	SCIa_Printf("  R  :	Download to RAM ( *.Hex )\n");
	SCIa_Printf("  D  :	Download to FLASH ( *.Hex )\n");
	SCIa_Printf("  G  :	Go User Program\n");
	SCIa_Printf("--------------------------------------------------------\n");		
	SCIa_Printf("  SSM 18th  Jeon yu hun   <dimbibara@gmail.com> \n");	
	SCIa_Printf("========================================================\n");	
}

/*   HELPMENU   */
/*************************************************************************
*	@name    	HelpMenu
*	@memo	       모니터 프로그램 메뉴 HELP
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
void HelpMenu(void)
{
	SCIa_Printf("\n\n\n");
	SCIa_Printf(" TMS320F28335 Monitor V%.1f ctml\n\n",ReleaseVersion);	
	SCIa_Printf("MENU :	one key button executable \n");		
	SCIa_Printf(" key :	Description \n");		
	SCIa_Printf("--------------------------------------------------------\n");		
	SCIa_Printf("  H  :	Help this page..\n");
	SCIa_Printf("  M  :	Display user available inputs, and executes.\n");
	SCIa_Printf("  A  :	Erase all sector of internal FLASH. sector B~H.\n\t(without sector A, Monitor sector)\n");
	SCIa_Printf("  S  :	Erase selected sector of internal FLASH. \n\t(It can be sector B~H)\n");
	SCIa_Printf("  R  :	Download HEX file to external RAM and relocation.  \n");
	SCIa_Printf("  D  :	Download HEX file to internal FLASH and relocation.\n\t(It will be stored from H to B)\n");
	SCIa_Printf("  G  :	JUMP program Counter to user start address.\n");											
	SCIa_Printf("--------------------------------------------------------\n");		
	SCIa_Printf("  SSM 18th  Jeon yu hun   <dimbibara@gmail.com> \n");	
	SCIa_Printf("========================================================\n");	

}

/*   CPU_TIMER0_ISR   */
/*************************************************************************
*	@name    	cpu_timer0_isr
*	@memo	       자동실행을 위한 Timer Interrupt 0
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
interrupt void cpu_timer0_isr(void)
{
	//CpuTimer0.InterruptCount++;
	static Uint16 WaitTime= 3;
	if (WaitTime>0)
	{
		if(WaitTime==3) SCIa_Printf("\nWelcome. This is TMS320F28335 Monitor program  by Jeon yu hun!!\n\nWait user input .. %d sec left\n\n",WaitTime);
		else SCIa_Printf("Wait user input .. %d sec left\n\n",WaitTime);
		WaitTime--;
	}
	else
	{
		SCIa_Printf("Jump to user program\n",WaitTime);		
		DownFromFlash(); // FLASH 에서 읽어와서 재배치 한다.
	}
   // Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

