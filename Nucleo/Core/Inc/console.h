/*
 * console.h
 *
 *  Created on: Nov 10, 2023
 *      Author: JOSE
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_


// INCLUDES
#include "stm32f1xx_hal.h"
#include <string.h>
#include "stdbool.h"
#include "usart.h"

// TYPE DEFINITIONS
typedef struct
{

  float AdcValue;
  float SpiValue;
  float DS18B20Value;

} CONSOLE_DataStreamType;

// DELCARATIONS
extern CONSOLE_DataStreamType ConsoleDataStream;


// PROTOTYPES
void ConsoleDebugDataFormat();
void ConsoleDebugDataSend();


































#endif /* INC_CONSOLE_H_ */
