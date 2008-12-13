//###########################################################################
//
// FILE:   serial.h
//
// TITLE:  SSM_ DSPMATE  28335 Serial module
//
// DESCRIPTION:
//	    				
//         Jeon yu hun  serial test
//
//###########################################################################
// $Release Date: 11-25, 2008 $
//###########################################################################

#ifndef SERIAL_H
#define SERIAL_H


#ifdef __cplusplus
extern "C" {
#endif

extern char SCIa_RxChar(void);
extern void SCIa_TxChar(char Data);
extern void SCIa_TxString(char *Str);
extern void TxPrintf(char *Form, ... );

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif

