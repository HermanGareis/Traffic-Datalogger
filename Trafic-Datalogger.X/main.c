/*
 * File:   main.c
 * Author: Nehuen Gareis
 *
 * Created on 24 June 2021, 16:13
 */


#include "xc.h"

/* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author: Nehuen Gareis      Date 10/6/21     Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES: TP FINAL ARQUITECTURA DE COMPUTADORAS 2021
* 
*
**********************************************************************/


#include "xc.h"
#include "config.h"
#include "common.h"
#include "confbits.h"
#define TRUE 1
#define MAX 255


t_vehiculo datalogger[MAX];

enum estadoActual;
int counterTog = 0, cantAutos = 0, qty;
unsigned int bcch, bccl;
int NACKmarco[8] = {0xFE, 0x08, 0x02, 0x03, 0x80, 0x47, 0x80, 0x52};
extern int marco[32];


void cargarDatos(void){
    
    datalogger[0].hours = 5;
    datalogger[0].minutes = 23;
    datalogger[0].seconds = 13;
    datalogger[0].velocidad = 54;
    datalogger[0].ejes = 2;
    cantAutos++; 
    datalogger[1].hours = 5;
    datalogger[1].minutes = 32;
    datalogger[1].seconds = 22;
    datalogger[1].velocidad = 68;
    datalogger[1].ejes = 3;
    cantAutos++;
}




int calcularVelocidad(void){
    
    int t;
    int v;
    
    t = (1 * 8 * TMR6)/1000; //en ms
    v = ((300/t)*18)/5; //en km/h
     
    return v; 
}

void UpdateClock (void)
{
    counterTog = 0;
    /* Actualizar Hora */
    seconds ++;
    if (seconds > 59)
    {
        seconds=0;
        minutes++;
    }
    if (minutes > 59)
    {
        minutes=0;
        hours++;
    }
    if (hours == 24)
    {
        hours=0;
    }

}

int validarMarco(){

    if(marco[0]!=0xFE){
        return 0;
    }
    if(marco[2]!=3){
        return 0;
    }
    
    if(marco[qty-1] != bccl){
        return 0;
    }
    
    if(marco[qty-2] != bcch){
        return 0;
    }
    
    return 1;
}

void checksum(void){
    int i =0;
    unsigned int  var,  acum=0;
    
    qty = marco[1]; 
    bcch = marco[i]; 
    
    for(i=1; i<=(qty-2); i++){
        if((qty-2) != i)
            bccl = marco[i++];
        else
            bccl = 0x00;
            
        var = (bcch << 8) + bccl;
        bcch = marco[i];
        
        acum += var;
    }
    bcch = 0x00FF00 & acum;
    bcch = (bcch >> 8);
    bccl = 0x00FF & acum; 
}

void NACK(){
    int i;
    IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
    for(i=0; i<=8; i++){
        marco[i] = NACKmarco[i];
        }
}




void __attribute__((interrupt, auto_psv)) _CNInterrupt( void ) {
    IFS1bits.CNIF = 0; 
    
    if(datalogger[cantAutos].ejes == 0 && PORTDbits.RD13){
        if(PORTDbits.RD6){
            T6CONbits.TON = 1; 	//habilito Timer  
        }        
        if(PORTDbits.RD7){
            T6CONbits.TON = 0;
            datalogger[cantAutos].velocidad = calcularVelocidad();
            TMR6 = 0;
            datalogger[cantAutos].ejes ++;
        }
    }
    if(datalogger[cantAutos].velocidad >= 60){ 
        PORTAbits.RA0 = 1;                     // camara           
        PORTAbits.RA0 = 0;
    }    
    if(PORTDbits.RD6 && PORTDbits.RD13){
        datalogger[cantAutos].ejes ++;
    }       
    if(PORTDbits.RD13 == 0){
        cantAutos ++;
        datalogger[cantAutos].hours = hours;
        datalogger[cantAutos].minutes = minutes;
        datalogger[cantAutos].seconds = seconds;
    }
}


int main(void) {
    
    config();
    cargarDatos();
    
    while( TRUE ){
        if(PORTAbits.RA0 == 1)
            PORTAbits.RA0 = 0;
            
        if (counterTog)
            UpdateClock();  //Actualizar hora del dispositivo
        
        if(estadoActual == RECIBIDO){
            checksum();
            if((validarMarco())){
                //estadoActual == EN_APLICACION;
                switch(marco[5]){
                    case 0x41:
                        comandoA();
                        estadoActual = EN_TRANSMISION;
                        break;
                    case 0x42:
                        comandoB();
                        estadoActual = EN_TRANSMISION;
                        break;                    
                    case 0x43:
                        comandoC();
                        estadoActual = EN_TRANSMISION;
                        break;                        
                     case 0x44:
                        comandoD();
                        estadoActual = EN_TRANSMISION;
                        break;               
                    case 0x45:
                        comandoE();
                        estadoActual = EN_TRANSMISION;
                        break;
                    default:
                        NACK();
                        estadoActual = EN_TRANSMISION;
                        break;
                }
            }
            else{
                NACK();
                estadoActual = EN_TRANSMISION;
            } 
        }
        
        
        if(estadoActual == EN_TRANSMISION){
            IFS1bits.U2TXIF = 1;  //Obligo Tx
        }
    
    }   
}