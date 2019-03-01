#include "sonar_functions.h"


void Init_SysClock()
{
    OSCFRQbits.HFFRQ = 0b110; //HFOSC is 32 MHz //110 for 32, `//011 for 8
    OSCCON1bits.NDIV = 0b0000;
    OSCENbits.HFOEN = 1; //HFOSC manually enabled
    OSCCON1bits.NOSC = 0b110; //Asks for oscillator as HFOSC  
}

void Init_Sonar()
{
    ANSELCbits.ANSC0 = 0;
	ANSELCbits.ANSC1 = 0; 
    TRISCbits.TRISC0 = 1; //Echo pin of the sonar is set as input pin 
    TRISCbits.TRISC1 = 0; //Trigger pin of the sonar is set as output pin
    
    T0CON0bits.T0EN = 0; //disable at first
    T0CON1bits.T0CS=0b010; // Timer Clock = Fosc/4 = 8MHz
    T0CON1bits.T0ASYNC = 0; //Timer is synced with 8MHz clock, increments on this clock
    T0CON1bits.T0CKPS = 0b0110; //Prescaler is 1:64 (for 1:64 do 0110) (for 1:2 do 0001)
    T0CON0bits.OUTPS = 0b0000; //Postscaler is 1:1
    T0CON0bits.T016BIT = 1; //Sets Timer0 as a 16 bit timer  
}

void Init_USART()
{
    //RC5:Digital RX for PIC
    ANSELCbits.ANSC5 = 0;
    TRISCbits.TRISC5 = 1;
    RX1DTPPS = 0b10101; //RC5 is RX //10 for PORTC
    
    //RA4: Digital TX for PIC
    ANSELAbits.ANSA4 = 0;
    TRISAbits.TRISA4 = 1;
    RA4PPS = 0x0F; 
    
    // Set Baud Rate to 9600
    SPBRGH = 0b00000011;    
    SPBRGL = 0b01000000;
    TX1STAbits.BRGH = 1;
    BAUD1CONbits.BRG16 = 1; 
    
    
    TX1STAbits.SYNC = 0; //async transmission mode
    RC1STAbits.SPEN = 1; //enables EUSART
    
    TX1STAbits.TX9 = 0; //8 bit transmission
       
    TX1STAbits.TXEN = 1; //enables TX circuitry 
    RC1STAbits.CREN = 1; //enables RX circuitry
}

void send_Data(char data)
{
    while(!TX1STAbits.TRMT);    //while a transmission is occuring, wait
    TX1REG = data;              //insert data into the buffer to be sent
}

//unused function currently, but can be used to enable Timer1 for additional timing appliciations
void Init_Timer1()
{
    T1CONbits.ON = 0; //disable timer
    T1CONbits.CKPS = 0b01;  //Prescaler selection [00,01,10,11] ----> [1,2,4,8]
    T1CONbits.RD16 = 0; //must read result as two bytes
    T1GCONbits.GE = 0; //no gate enable for Timer1
    TMR1CLKbits.CS = 0b00001;    //Selects Timer1 clock as HFINTOSC/4
}