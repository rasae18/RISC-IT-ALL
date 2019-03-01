/* 
 * File:   usart_test.c
 * Author: Rafed
 *
 * Created on November 15, 2018, 7:16 PM
 */

/*---------CONFIGURATION BITS---------*/
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits->Oscillator not enabled
#pragma config RSTOSC = 0b000        //HFINT1    // Power-up default value for COSC bits->HFINTOSC (32MHz)
#pragma config CLKOUTEN = OFF    // Clock Out Enable bit->CLKOUT function is disabled; i/o or oscillator function on OSC2
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable bit->FSCM timer enabled

// CONFIG2
#pragma config MCLRE = OFF    // Master Clear Enable bit->MCLR pin is Master Clear function
#pragma config PWRTS = OFF    // Power-up Timer Enable bit->PWRT disabled
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit->ULPBOR disabled
#pragma config BOREN = OFF    // Brown-out reset enable bits->Brown-out Reset Enabled, SBOREN bit is ignored
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (VBOR) set to 2.45V
#pragma config ZCDDIS = OFF    // Zero-cross detect disable->Zero-cross detect circuit is disabled at POR.
#pragma config PPS1WAY = ON    // Peripheral Pin Select one-way control->The PPSLOCK bit can be cleared and set only once in software
#pragma config STVREN = OFF    // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a reset

// CONFIG3
#pragma config WDTCPS = WDTCPS_31    // WDT Period Select bits->Divider ratio 1:65536; software control of WDTPS
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled, SWDTEN is ignored
#pragma config WDTCWS = WDTCWS_7    // WDT Window Select bits->window always open (100%); software control; keyed access not required
#pragma config WDTCCS = SC    // WDT input clock selector->Software Control

// CONFIG4
#pragma config BBSIZE = BB512    // Boot Block Size Selection bits->512 words boot block size
#pragma config BBEN = OFF    // Boot Block Enable bit->Boot Block disabled
#pragma config SAFEN = OFF    // SAF Enable bit->SAF disabled
#pragma config WRTAPP = OFF    // Application Block Write Protection bit->Application Block not write protected
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block not write protected
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration Register not write protected
#pragma config WRTD = OFF    // Data EEPROM write protection bit->Data EEPROM NOT write protected
#pragma config WRTSAF = OFF    // Storage Area Flash Write Protection bit->SAF not write protected
#pragma config LVP = OFF    // Low Voltage Programming Enable bit->High Voltage on MCLR/Vpp must be used for programming

// CONFIG5
#pragma config CP = OFF    // UserNVM Program memory code protection bit->UserNVM code protection disabled



#include <stdio.h>
#include <stdlib.h>
#include "sonar_functions.h"

/*--------------DEFINES FOR PINS------------*/
#define TRIG_PIN LATCbits.LATC1 //RC1 -> Trigger Pin for Sonar COM
#define ECHO_PIN PORTCbits.RC0  //RC0 -> Echo Pin for Sonar COM
#define TMR_EN T0CON0bits.T0EN  //Timer0 Enable bit
#define TMR_OV PIR0bits.TMR0IF  //TImer0 overflow bit

#define _XTAL_FREQ 32000000 //makes a constant as 32e6 to work with delay function


int main(int argc, char** argv) {
    
    Init_SysClock();   
    Init_USART();
    Init_Sonar();
 
    unsigned int distance;
       
    while(1)
    {   
        TMR0H = 0; TMR0L = 0; //clear the timer bits 
        TRIG_PIN = 1;  // trigger high
        __delay_us(10); // 10us delay
        TRIG_PIN = 0; // trigger low

        while(!ECHO_PIN); // wait for echo pin
        TMR_EN = 1; // timer start
        while(ECHO_PIN); // wait for ECHO pin to go low
        TMR_EN = 0; // Timer stop
        
        distance = (TMR0L | (TMR0H<<8)); //Reads Timer Value, appends each 8 bits into 16 bit int variable "distance"
        distance = .136*distance;   //This calculation uses the system clock frequency and the prescaler value of 64 to compute the distance in centimeters

        if (distance < 255) {send_Data(distance);}  //if the distance is less than 255 centimeters, send the data through the UART
        __delay_ms(2000);   //wait 5 seconds before repeating this loop

              
    }//end while(1)

    return (EXIT_SUCCESS);
    
}//end MAIN




