/*
 * DS18B20.h
 *
 *  Created on: Nov 23, 2023
 *      Author: JOSE
 */



#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_



#include "gpio.h"





#define COMMAND_WRITE_SCRATCHPAD		0x4E
#define COMMAND_READ_SCRATCHPAD		    0xBE
#define COMMAND_COPY_SCRATCHPAD			0x48
#define COMMAND_RECALL_EEPROM			0xB8
#define COMMAND_SEARCH_ROM			    0xF0
#define COMMAND_READ_ROM			    0x33
#define COMMAND_SKIP_ROM			    0xCC
#define COMMAND_ALARM_SEARCH 			0xEC
#define COMMAND_CONVERT_TEMP 			0x44
#define COMMAND_READ_PSU    			0xB4

#define WRITETIMESLOT_LOWTIME              5
#define WRITETIMESLOT_SAMPLETIME          70
#define WRITETIMESLOT_RECOVERYTIME         5

#define READTIMESLOT_SLOTTIME             50
#define READTIMESLOT_SAMPLETIME           14
#define READTIMESLOT_RECOVERYTIME          5

#define SIZE_SCRATCHPAD                    8

typedef struct
{
	uint8_t TempLsb;
	uint8_t TempMsb;
	uint8_t TempHighReg;
	uint8_t TempLowReg;
	uint8_t Config;
	uint8_t Reserved1;
	uint8_t Reserved2;
	uint8_t Reserved3;
	uint8_t CrcReg;
} Scratchpad_Typedef;




float DS18B20_Test();
float DS18B20_CalculateTemperature();
bool DS18B20_TriggerConversion();
bool DS18B20_GetScratchPad();
bool DS18B20_ResetSequence();
void DS18B20_InputSet();
void DS18B20_OutputSet();
uint8_t DS18B20_ReadBit();
uint8_t DS18B20_ReadByte();
void DS18B20_WriteBit(uint8_t bitValue);
void DS18B20_WriteByte(uint8_t CommandValue);








#endif /* INC_DS18B20_H_ */
