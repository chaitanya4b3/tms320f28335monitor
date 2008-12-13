//###########################################################################
//
// FILE:   serial.c
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

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


/*   SCIA_RXCHAR   */
/*************************************************************************
*	@name    	SCIa_RxChar
*	@memo	       Uart_a char 입력 리턴
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
char SCIa_RxChar(void)
{
    while( !(SciaRegs.SCIRXST.bit.RXRDY) );
    return (char)SciaRegs.SCIRXBUF.all;
}

/*   SCIA_TXCHAR   */
/*************************************************************************
*	@name    	SCIa_TxChar
*	@memo	       Uart_a 문자 출력
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
void SCIa_TxChar(char Data)
{
    while(!(SciaRegs.SCICTL2.bit.TXRDY));
    SciaRegs.SCITXBUF = Data;
}

/*   SCIA_TXSTRING   */
/*************************************************************************
*	@name    	SCIa_TxString
*	@memo	       Uart_a 문자열 출력
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
void SCIa_TxString(char *Str)
{
    while(*Str) 
    {
        if(*Str == '\n'){
            SCIa_TxChar('\r');
        }
		
        SCIa_TxChar(*Str++ );
    }
}      
/*   TXPRINTF   */
/*************************************************************************
*	@name    	TxPrintf
*	@memo	       Uart_a printf 형식 출력
*	@author	       Joen yu hun
*	@company	SSM
*    
**************************************************************************/
void TxPrintf(char *Form, ... )
{
    static char Buff[100];
    va_list ArgPtr;
    va_start(ArgPtr,Form);	 
    vsprintf(Buff, Form, ArgPtr);
    va_end(ArgPtr);
    SCIa_TxString(Buff);
}

