/* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author: Nehuen Gareis      Date 10/6/21     Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
*
**********************************************************************/


#include "xc.h"
#include "config.h"

void config(void) {
    
    //ports
    TRISD = 0xFFFF ;        // PortD as input
    TRISA = 0x0000 ;        // PortA as output
    
    InitUART2();
    Init_Timer4();
    Init_Timer6();
    
    //interrupts enabled
    IEC1bits.CNIE = 1;
    CNEN1bits.CN15IE = 1;   //CN15 -> RD6
    CNEN2bits.CN16IE = 1;   //CN16 -> RD7
    CNEN2bits.CN19IE = 1;   //CN19 -> RD13
    IFS1bits.CNIF = 0;  
}

