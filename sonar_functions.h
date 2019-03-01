/* 
 * File:   sonar_functions.h
 * Author: Rafed
 *
 * Created on November 25, 2018, 12:34 PM
 */
#include <xc.h>

#ifndef SONAR_FUNCTIONS_H
#define	SONAR_FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void Init_SysClock();    

void Init_Sonar();

void Init_USART();

void send_Data(char data);

void Init_Timer1();

#ifdef	__cplusplus
}
#endif

#endif	/* SONAR_FUNCTIONS_H */

