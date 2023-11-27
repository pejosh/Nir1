/*
 * console.c
 *
 *  Created on: Nov 10, 2023
 *      Author: JOSE
 */

#include "console.h"
#include "stdio.h"


CONSOLE_DataStreamType ConsoleDataStream;

bool DebugOutputEnabled = true;


void ConsoleDebugDataFormat()
{
	__NOP();
	sprintf((char*)UartTxBuffer, "#DBG: %.2f, %.2f ,%.2f\r",
			 ConsoleDataStream.AdcValue, ConsoleDataStream.SpiValue, ConsoleDataStream.DS18B20Value);
}



void ConsoleDebugDataSend()
{
	if(DebugOutputEnabled)
	{
		ConsoleDebugDataFormat();
		HAL_UART_Transmit(&huart3, UartTxBuffer, strlen((char*)UartTxBuffer), 100);
	}
}


