//###########################################################################
//
// FILE:    flash.h
//
// TITLE:   TMS320F28335MONITOR
//
// ASSUMPTIONS:
//
//###########################################################################
// $Release Date: 11, 8, 2008 $		BY  HWANG HA YUN
//###########################################################################

#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef	__FLASH__
	#undef	__FLASH__
	#define FLASH_EXT
#else
	#define	FLASH_EXT extern
#endif

#define FLASH_OPT volatile


extern void InitFlashAPI(void);
extern void Erase_AllFlash(void);
extern void Erase_SelectFlash(void);
extern void InitUserHexDownVariable(void);
extern void SCItoRamDownloadPrm(void);
extern 	void FlashtoRamDownloadPrm(void);
extern void InitUserProgramData(void);
extern Uint16 UserProgramData(void);
extern 	void FlashBurnPrm(void);
extern Uint16 UserPrmHexFileDownLoading(char StartState, Uint16 Source);
extern char HEXDOWN_AsciiToHex(char Ascii);
extern Uint32 HEXDOWN_AsciiConvert(Uint16 NumByte , Uint16 Source);
extern void UserProgramStart(void);


typedef union 
{
    Uint16    all;
    struct
	{         
       Uint16     bit0:1;             
       Uint16     bit1:1;             
       Uint16     bit2:1;             
       Uint16     bit3:1;             
       Uint16     bit4:1;             
       Uint16     bit5:1;             
       Uint16     bit6:1;             
       Uint16     bit7:1;             
    }Bit; 
}BYTE_DEF;

typedef union 
{
    Uint32    Long;

	struct
	{
        Uint16    Low:16;
        Uint16    High:16;
    }Word;
}DIVIDE_LONG;

typedef struct
{							
    Uint16    		DataLength;
	DIVIDE_LONG		Address;			
    Uint16   	 	RecordType;
	
    Uint16  		Checksum;
    BYTE_DEF    	Status;
}HEX_FIELD;

FLASH_EXT FLASH_OPT HEX_FIELD DownLoadingHex;

#endif

