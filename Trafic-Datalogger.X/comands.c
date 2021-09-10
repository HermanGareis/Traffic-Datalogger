/*
 * File:   comands.c
 * Author: Nehuen Gareis
 *
 * Created on June 22, 2021, 5:20 PM
 */


#include "xc.h"
#include "config.h"
#include "common.h"

#define MAX 255

extern int marco[32];
extern int bcch, bccl, cantAutos, estadoMarco ;
enum estadoActual;
extern t_vehiculo datalogger[MAX];
int i=0; 

void comandoA(void){ //cant vehiculos
    marco[1] = 9;
    marco[2] = marco[3];
    marco[3] = 3;
    marco[6] = cantAutos;
    checksum();
    marco[7]=bcch;
    marco[8]=bccl;
   
    IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
}

void comandoB(void){ //resetear registros
    cantAutos = 0;
    marco[2] = marco[3];
    marco[3] = 3;
    checksum();
    marco[6]=bcch;
    marco[7]=bccl;
    
    IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
}

void comandoC(void){ //vehiculos con mas de 2 ejes
    int cont;
    for(i=0; i<=cantAutos; i++){
        if (datalogger[i].ejes>2){
            cont++;
        }
    }
    marco[1] = 9;
    marco[2] = marco[3];
    marco[3] = 3;
    marco[6] = cont;
    checksum();
    marco[7]=bcch;
    marco[8]=bccl;
   
    IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
}

void comandoD(int hora){ // vehiculos en una hora det
    int j = 6;
    hora = marco[6];
    for(i=0; i<=cantAutos; i++){
        if (datalogger[i].hours == hora){
            marco[j]=datalogger[i].hours;
            j++;
            marco[j] = datalogger[i].minutes;
            j++;
            marco[j] = datalogger[i].seconds;
            j++;
            marco[j] = datalogger[i].velocidad;
            j++;
            marco[j] = datalogger[i].ejes;
            j++;
        }
    }
    
    marco[1] = j+2;    
    marco[2] = marco[3];    
    marco[3] = 3; 
    checksum();
    marco[j]=bcch;
    marco[j+1]=bccl;
    
    IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts   
}

void comandoE(void){  //accionar cam
    PORTAbits.RA0 = 1;                
    PORTAbits.RA0 = 0;
    
    marco[2] = marco[3];
    marco[3] = 3;
    checksum();
    marco[6]=bcch;
    marco[7]=bccl;
    
    IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts  
}
