//###########################################################################
//
// FILE:   flash.h
//
// TITLE:  SSM_ DSPMATE  28335 flash.c header
//
// DESCRIPTION:
//	    				
//         Jeon yu hun  
//
//###########################################################################
// $Release Date: 11-25, 2008 $
//###########################################################################

#ifndef FLASH_H
#define FLASH_H


#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
   These key values are used to unlock the CSM by this example
   They are defined in Example_Flash2833x_CsmKeys.asm
--------------------------------------------------------------------------*/
extern Uint16 PRG_key0;        //   CSM Key values
extern Uint16 PRG_key1;
extern Uint16 PRG_key2;
extern Uint16 PRG_key3;
extern Uint16 PRG_key4;
extern Uint16 PRG_key5;
extern Uint16 PRG_key6;
extern Uint16 PRG_key7;  

extern void InitFlashAPI(void);
extern void Erase_AllFlash(void);
extern void Erase_SelectFlash(void);
extern void InitUserHexDownVariable(void);
extern void SCItoRamDownloadPrm(void);
extern void FlashtoRamDownloadPrm(void);
extern void InitUserProgramData(void);
extern Uint16 UserProgramData(void);
extern void FlashBurnPrm(void);
extern Uint16 UserPrmHexFileDownLoading(char StartState, Uint16 Source);
extern char HEXDOWN_AsciiToHex(char Ascii);
extern Uint32 HEXDOWN_AsciiConvert(Uint16 NumByte , Uint16 Source);
extern void UserProgramStart(void);
extern Uint16 CsmUnlock(void);


struct BYTE8
{         
	Uint16     bit0:1;             
	Uint16     bit1:1;             
	Uint16     bit2:1;             
	Uint16     bit3:1;             
	Uint16     bit4:1;             
	Uint16     bit5:1;             
	Uint16     bit6:1;             
	Uint16     bit7:1;             
}; 

union BYTE_DEF
{
	Uint16    all;
	struct BYTE8 bit;
};

struct LONG32 
{
	Uint16    low16:16;
	Uint16    high16:16;
};

union DIVIDE_LONG
{
	Uint32    all;
	struct LONG32 Word; 
};

struct HEX_FIELD
{							
	Uint16 DataLength;
	union DIVIDE_LONG Address;			
	Uint16 RecordType;
	Uint16 Checksum;
	union BYTE_DEF Status;
};

extern volatile struct HEX_FIELD DownLoadingHex;

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif

