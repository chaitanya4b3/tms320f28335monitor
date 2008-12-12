//###########################################################################
//
// FILE:    VFD.c
//
// TITLE:   TMS320F28335MONITOR
//
// ASSUMPTIONS:
//
//###########################################################################
// $Release Date: 11, 8, 2008 $		BY  HWANG HA YUN
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define VFD_CNT_CS		(*(volatile Uint16 *)0x0200020)
#define VFD_DATA		(Uint32)0x0200030

#define VFDCLEAR		0x80

Uint16 VFDAddr;
Uint16 VFDBright;

void InitVFDBright(Uint16 i)
{
	VFD_CNT_CS = i;
	DSP28x_usDelay(200);
	VFDAddr = 0;
	VFDBright = i;

	
}

void VFDPutChar(char Data)
{
	if(VFDAddr < 0x0F)
	{
		(*(volatile Uint16 *)(VFD_DATA + VFDAddr)) = Data;
		DSP28x_usDelay(10);
	}
	else
		;
	VFDAddr+=2;
}

void VFDString(char *Str)
{	
	VFD_CNT_CS = VFDCLEAR | VFDBright;
	DSP28x_usDelay(200);
	VFDAddr = 0;

	while(*Str)
	{
		VFDPutChar(*Str++);
	}
		
}

void VFDPrintf(char *form, ... )
{
    static char buff[50];

	va_list argptr;
    va_start(argptr,form);
    vsprintf(buff, form, argptr);
	va_end(argptr);
	VFDString(buff);
}

void VFDMenu(char *str)
{
	char *backup_str;
	int16 count = 0;
	Uint16 i = 0;

	backup_str = str;

	VFD_CNT_CS = VFDCLEAR | VFDBright;
	DSP28x_usDelay(200);

	for(count = 7; count >= 0; count--)
	{
		VFDAddr = count*2;

		while(*str)
		{
			VFDPutChar(*str++);
			if(i++ == (7 - count))
				break;

		}

		i = 0;
		str = backup_str;
		DSP28x_usDelay(720000);
	}

}





