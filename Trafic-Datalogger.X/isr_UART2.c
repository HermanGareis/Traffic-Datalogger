/* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author: Nehuen Gareis      Date 16/6/21     Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
*
**********************************************************************/

#include "xc.h"
#include "config.h"
#include "common.h"

//UART
#define FCY 1000000
#define BAUDRATE 19200
#define BRGVAL ((FCY / BAUDRATE) / 4) -1 // 12

volatile unsigned char caracter;
int marco[32],cont=0;
enum estados estadoActual;


/*---------------------------------------------------------------------
  Function Name: UART2Interrupt
  Description:   UART2 Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
//Interrupciones de la UART2

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void )
{
	IFS1bits.U2RXIF = 0;
    estadoActual = EN_RECEPCION   ; //recibiendo marco
	caracter = U2RXREG;
    marco[cont] = caracter;
    cont++;
    
    if(cont == marco[1]){
        estadoActual = RECIBIDO; //marco recibido en su totalidad
        IEC1bits.U2RXIE = 0;	// Disable Recieve Interrupts
        cont = 0;
    } 
}

//Rutina de INT para transmisión 
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0;    
    U2TXREG = marco[cont];	
    cont++;
    
    if(cont == marco[1]){
        cont = 0;
        estadoActual = EMITIDO;
        IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts
        IEC1bits.U2TXIE = 0;	// Disable Transmit Interrupts
    }     
}

/*---------------------------------------------------------------------
  Function Name: InitUART2
  Description:   Inicializar UART2
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void InitUART2(void)
{
	// The HPC16 board has a DB9 connector wired to UART2, 
	// so we will be configuring this port only
	// configure U2MODE
	U2MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
	U2MODEbits.RTSMD = 1;	// Bit11 Simplex Mode
    U2MODEbits.BRGH = 1;    // High speed
    

	// Load a value into Baud Rate Generator.  Example is for 9600.
	U2BRG = BRGVAL;	// 1Mhz osc, 19200 Baud

	IPC7= 0x4400;	// Mid Range Interrupt Priority level, no urgent reason

	IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts

	U2MODEbits.UARTEN = 1;	// And turn the peripheral on
	U2STAbits.UTXEN = 1;	// UART2 transmitter enabled
    U2STAbits.UTXISEL0 = 1;  // Interrupt generated when the last transmission is over
          
    
    

	IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
	
}