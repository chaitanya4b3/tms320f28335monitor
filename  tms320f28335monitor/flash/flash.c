//###########################################################################
//
// FILE:    flash.c
//
// TITLE:   TMS320F28335MONITOR
//
// ASSUMPTIONS:
//
//###########################################################################
// $Release Date: 11, 8, 2008 $		BY  HWANG HA YUN
//###########################################################################

#define __FLASH__

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_SUCCESS 0
#define SCI		0
#define FLASH	1

#define USER_FLASH	(Uint32)0x300000
#define USER_RAM	(Uint32)0x100000

#define FLASH_DEBUG		0

const Uint16 FlashSector[8] = {0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};

Uint16 *pFlashAdd;
Uint16 *pRamAdd;

#define USER_PRG_ADD	(Uint32)0x008300


//#pragma CODE_SECTION(HEXDOWN_AsciiToHex, "ramfuncs");
//#pragma CODE_SECTION(HEXDOWN_AsciiConvert, "ramfuncs");
//#pragma CODE_SECTION(UserPrmHexFileDownLoading, "ramfuncs");
//#pragma CODE_SECTION(FlashBurnPrm, "ramfuncs");


void InitFlashAPI28235(void)
{
	Uint16 VersionHex;
	float  Version;
	
	Flash_CPUScaleFactor = SCALE_FACTOR;

	VersionHex = Flash_APIVersionHex();
	if(VersionHex != 0x0210)
	{
		// Unexpected API version
		// Make a decision based on this info. 
		TxPrintf("\n  Flash API Error!!\n");
		asm("    ESTOP0");
	}   


	Version = Flash_APIVersion();
	if(Version != (float32)2.10)
	{
		// Unexpected API version
		// Make a decision based on this info. 
		TxPrintf("\n  Flash API Error!!\n");
		asm("    ESTOP0");
	}
   
	
}

void DeleteSecletFlash(void)
{
	Uint16 Status;
	char RcvData;
	Uint16 DeleteSector;

	FLASH_ST FlashStatus;

	RcvData = SCIa_RxChar();
	SCIa_TxChar(RcvData);
	if(RcvData != ' ')
	{
		TxPrintf("\nIllegal Command!!\n");
		return;
	}
	RcvData = SCIa_RxChar();
	SCIa_TxChar(RcvData);
	switch(RcvData)
	{
		case 'b':
		case 'B':
			DeleteSector = SECTORB;
			TxPrintf("\n  Delete SectorB.\n");
			break;
			
		case 'c':
		case 'C':
			DeleteSector = SECTORC;
			TxPrintf("\n  Delete SectorC.\n");
			break;
			
		case 'd':
		case 'D':
			DeleteSector = SECTORD;
			TxPrintf("\n  Delete SectorD.\n");
			break;

		case 'e':
		case 'E':
			DeleteSector = SECTORE;
			TxPrintf("\n  Delete SectorE.\n");
			break;

		case 'f':
		case 'F':
			DeleteSector = SECTORF;
			TxPrintf("\n  Delete SectorF.\n");
			break;

		case 'g':
		case 'G':
			DeleteSector = SECTORG;
			TxPrintf("\n  Delete SectorG.\n");
			break;

		case 'h':
		case 'H':
			DeleteSector = SECTORH;
			TxPrintf("\n  Delete SectorH.\n");
			break;
			
		default:
			TxPrintf("\nWrong Sector Select!!\n");
			return;
	}

	Status = Flash_Erase(DeleteSector, &FlashStatus);

	if(Status != STATUS_SUCCESS)
	{
		TxPrintf("\n  Flash Error!!\n");
		return;
	}

	TxPrintf("\n  Delete Sector OK!!\n");

	
}

void DeleteAllFlash(void)
{
	Uint16 Status;
	
	FLASH_ST FlashStatus;

	TxPrintf("\n  Delete All Flash Sector.\n");
				
	Status = Flash_Erase(SECTORB|SECTORC|SECTORD|SECTORE|SECTORF|SECTORG|SECTORH, &FlashStatus);
	
	if(Status != STATUS_SUCCESS)
	{
		TxPrintf("\n  Flash Error!!\n");
		return;
	}
	
	TxPrintf("\n  Delete All Flash Sector OK!!(SECTOR B ~ SECTOR H)\n");
	
}

void SCItoRamDownloadPrm(void)

{
	InitUserHexDownVariable();
	TxPrintf("\n  Send User Program *.Hex\n");

	SCIa_TxChar(BELL);
	SCIa_TxChar(BELL);

	if(UserPrmHexFileDownLoading(0, SCI)) 
		TxPrintf("\n  DownLoading Success !!");
    else 
	{
		TxPrintf("\n  DownLoading Failure !!"); 
		return;
    }
	TxPrintf("\n  Go To User Program !!\n");

	UserProgramStart();
	
}

void FlashtoRamDownloadPrm(void)
{
	InitUserProgramData();

	InitUserHexDownVariable();		
	if(UserPrmHexFileDownLoading(0, FLASH)) 
		TxPrintf("\n  DownLoading Success !!");
	else 
	{
		TxPrintf("\n  DownLoading Failure !!"); 
		//return;//
	}

	TxPrintf("\n  Go To User Program !!\n");

	UserProgramStart();
		
}

void FlashBurnPrm(void)
{
	char RcvData[2];
	Uint16 BurnData;
	Uint16 Status;
	Uint16 Buf[15] = {0,};		
	Uint16 EndCnt = 0;

	Uint32 BurnCnt;
	Uint32 i;
	
	FLASH_ST FlashStatus;

	TxPrintf("\n  Send User Program *.Hex\n");
	
	SCIa_TxChar(BELL);
	SCIa_TxChar(BELL);

	
	pFlashAdd = (Uint16 *)USER_FLASH;
	pRamAdd = (Uint16 *)USER_RAM;
	BurnCnt = 0;

	while(TRUE)
	{
		RcvData[0] = SCIa_RxChar();
		RcvData[1] = SCIa_RxChar();

		BurnData = ((RcvData[0] << 8) + RcvData[1]);

		*(pRamAdd + BurnCnt++) = BurnData;

		if(RcvData[0] == ':')
		{
			SCIa_TxChar('.');
			EndCnt = 0;
			Buf[EndCnt++] = RcvData[0];
			Buf[EndCnt++] = RcvData[1];
		}
		else if(RcvData[1] == ':')
		{
			SCIa_TxChar('.');
			EndCnt = 0;
			Buf[EndCnt++] = RcvData[1];
		}
		else
		{
			if(EndCnt > 13)
				EndCnt = 0;
			
			Buf[EndCnt++] = RcvData[0];
			Buf[EndCnt++] = RcvData[1];

			// :00000001FF
			if(Buf[0] == ':')
			if(Buf[1] == '0')
			if(Buf[2] == '0')
			if(Buf[3] == '0')
			if(Buf[4] == '0')
			if(Buf[5] == '0')
			if(Buf[6] == '0')
			if(Buf[7] == '0')			
			if(Buf[8] == '1')
			if(Buf[9] == 'F')
			if(Buf[10] == 'F')
				
				break;
		}

	
	}

	TxPrintf("\n  %ld word downloded!!\n", BurnCnt);
	

	for(i = 1; i < 8; i++)
	{
		if(BurnCnt < ((Uint32)0x8000*i - 0x100))
		{
			TxPrintf("\n  Delete Flash Sector !!\n");
			Status = Flash_Erase(FlashSector[i-1], &FlashStatus);
			if(Status != STATUS_SUCCESS)
			{
				TxPrintf("\n  Flash Error!!\n");
				return;
			}
			else
			{
				TxPrintf("\n  Flash Sector Cnt : %ld Deleted !!\n", i);
				i = 0;
				break;
			}
		}
	}
	if(i != 0)
	{
		TxPrintf("\n  User Program Size too Big !!\n");
		return;
	}


	for(i = 0; i < BurnCnt; i++)
	{
		Status = Flash_Program(pFlashAdd++,pRamAdd++,1,&FlashStatus);
		if(Status != STATUS_SUCCESS)
		{
			TxPrintf("\n  Flash Error!!\n");
			return;
		}	
	}

	TxPrintf("\n  Burn User Program End!!\n");
	
	SCIa_TxChar(BELL);
	SCIa_TxChar(BELL);

#if FLASH_DEBUG

	pFlashAdd = (Uint16 *)USER_FLASH;

	for(i = 0; i < BurnCnt; i++)
	{
		RcvData[0] = *(pFlashAdd++);

		Buf[0] = RcvData[0] >> 8;

		if((char)Buf[0] == CR)
		{
			SCIa_TxChar('\r');
			SCIa_TxChar('\n');
		}
		else
			SCIa_TxChar(Buf[0]);
		
		Buf[1] = RcvData[0] & 0x00ff;
		
		if((char)Buf[1] == CR)
		{
			SCIa_TxChar('\r');
			SCIa_TxChar('\n');
		}
		else
			SCIa_TxChar(Buf[1]);
		
	}
#endif
	

}

void UserProgramStart(void)
{
	Uint32 Add = USER_PRG_ADD;

	void (*UserPrg)(void) = (void(*)(void))Add;

	DINT;

	UserPrg();
}


void InitUserHexDownVariable(void)
{
    memset((void *)&DownLoadingHex, 0x00, sizeof(HEX_FIELD));
}

void InitUserProgramData(void)
{
	pFlashAdd = (Uint16 *)USER_FLASH;
}

Uint16 UserProgramData(void)
{
	static Uint16 Flag = OFF;
	Uint16 Data;

	if(Flag == OFF)
	{
		Data = (*pFlashAdd >> 8) & 0xff;
		Flag = ON;
	}
	else
	{
		Data = *pFlashAdd & 0xff;
		Flag = OFF;
		pFlashAdd++;
	}

	return Data;
}


Uint16 UserPrmHexFileDownLoading(char StartState, Uint16 Source)
{
    int i;
    Uint16	CheckSum;
	Uint16  Temp;
    while( !DownLoadingHex.Status.Bit.bit0 && !DownLoadingHex.Status.Bit.bit1 )
	{  
        if( !StartState )
        {
			if(Source == SCI)
			{
				while( SCIa_RxChar() != ':' )
					;
			}
			else//FLASH
			{
				while((i = UserProgramData()) != ':')
				{
					if(i == 'F')
					{
						TxPrintf("\n  Flash Invalid!! \n");
						return FALSE;
					}
				}
			}
        }
		
        StartState = 0;
        DownLoadingHex.Checksum = CheckSum = 0;
        //      Data Length
        DownLoadingHex.DataLength = (Uint16)HEXDOWN_AsciiConvert(2, Source);
        //      Offset Address
        DownLoadingHex.Address.Word.Low = (Uint16)HEXDOWN_AsciiConvert(4, Source);
        //      Data Type
        DownLoadingHex.RecordType = (Uint16)HEXDOWN_AsciiConvert(2, Source);
    
        switch (DownLoadingHex.RecordType) 
		{
        //      DATA  
	        case 0x00:
	            // in this point, it will be able to write the data to the fresh rom directly.
	            for( i = 0; i < DownLoadingHex.DataLength; i += 2)
				{
	                if( ( DownLoadingHex.DataLength - i ) == 1 ) 
						Temp = (Uint16)HEXDOWN_AsciiConvert(2, Source);
	                else 
						Temp = (Uint16)HEXDOWN_AsciiConvert(4, Source);

					*(Uint16 *)DownLoadingHex.Address.Long = Temp;
					
	                DownLoadingHex.Address.Long ++;
	            }
	            break;
	        //      End Of File 
	        case 0x01:
	            DownLoadingHex.Status.Bit.bit0 = ON;
	            break;
	        //      Extended Linear Address 
	        case 0x04:
	            DownLoadingHex.Address.Word.High = (Uint16)HEXDOWN_AsciiConvert(4, Source);
	            break;
	        //      Start Linear Address 
	        case 0x05:
	            break;
    
        }
        CheckSum = ((~DownLoadingHex.Checksum) + 1) & 0xff;

        if( CheckSum != (Uint16)HEXDOWN_AsciiConvert(2, Source) )
		{
			SCIa_TxString("\nCheckSumError");
            return FALSE;
        }

		 SCIa_TxChar('.');
    }
 
    return TRUE;
}

Uint32 HEXDOWN_AsciiConvert( Uint16 NumByte, Uint16 Source)
{
    Uint32 Value,i;
    char Rcvdata;

    Value = 0;
    
    for ( i = 0; i < NumByte; i++ ) 
	{
		if(Source == SCI)
        	Rcvdata = SCIa_RxChar();
		else
			Rcvdata = UserProgramData();
		
        Value |= HEXDOWN_AsciiToHex( Rcvdata ) << ( ( (NumByte-1) - i ) * 4 );
    }

	if( NumByte == 4 )
	{
        DownLoadingHex.Checksum += (( Value >> 8 ) & 0xff);
        DownLoadingHex.Checksum += (Value & 0xff);
    }
    else
	{
        DownLoadingHex.Checksum += (Value & 0xff);
    }

	return Value;
}

char HEXDOWN_AsciiToHex(char Ascii)
{
	if(Ascii >= '0' && Ascii <= '9')return(Ascii-'0');
	else if(Ascii >= 'a' && Ascii<= 'f')return(Ascii-'a'+10);
	else if(Ascii >= 'A' && Ascii <= 'F')return(Ascii-'A'+10);
	else return(0xFF);
}


