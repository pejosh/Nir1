/*
 * DS18B20.c
 *
 *  Created on: Nov 23, 2023
 *      Author: JOSE
 */


#include "DS18B20.h"
#include "tim.h"


Scratchpad_Typedef ScratchPad;
uint16_t ProbeTemperature = 0;




float DS18B20_Test()
{
    if (!DS18B20_TriggerConversion())
    	return 0;

    // Conversion delay 800 ms
    HAL_Delay(800);

    if (DS18B20_GetScratchPad())
    {
    	return DS18B20_CalculateTemperature();
    }
    else
    	return 0;;
}



float DS18B20_CalculateTemperature()
{
	uint8_t TempSign, TempInt, TempFrac;
	float tp;

	//ScratchPad.TempMsb = 0b11111110;
	//ScratchPad.TempLsb = 0b01101111;

	TempSign = ScratchPad.TempMsb >> 7;
	TempInt = ((ScratchPad.TempMsb & 0x0f) << 4) | (ScratchPad.TempLsb >> 4);
	TempFrac = ScratchPad.TempLsb & (uint8_t)0x0f;

	__NOP();

	if(TempSign != 0)
	{
		// two's complement
		if (TempInt == 0b11111111)
			TempInt = 0;
		else
		{
			TempInt -= 1;
			TempInt = ~TempInt;
		}

		TempFrac -= 1;
		TempFrac = (~TempFrac) & 0x0f;
		tp = ((float) TempInt + ((float) TempFrac * 0.0625)) * (-1);
	}
	else
	    tp = (float) TempInt + ((float) TempFrac * 0.0625);

	return tp;
}




bool DS18B20_TriggerConversion()
{
	if (DS18B20_ResetSequence())
	{
		DS18B20_WriteByte(COMMAND_SKIP_ROM);
		DS18B20_WriteByte(COMMAND_CONVERT_TEMP);
		return true;
	}
	else
		return false;
}



bool DS18B20_GetScratchPad()
{
	if(DS18B20_ResetSequence())
	{
		DS18B20_WriteByte(COMMAND_SKIP_ROM);
	    DS18B20_WriteByte(COMMAND_READ_SCRATCHPAD);
		ScratchPad.TempLsb = DS18B20_ReadByte();
		ScratchPad.TempMsb = DS18B20_ReadByte();
		ScratchPad.TempHighReg = DS18B20_ReadByte();
		ScratchPad.TempLowReg = DS18B20_ReadByte();
		ScratchPad.Config = DS18B20_ReadByte();
		ScratchPad.Reserved1 = DS18B20_ReadByte();
		ScratchPad.Reserved2 = DS18B20_ReadByte();
		ScratchPad.Reserved3 = DS18B20_ReadByte();
		ScratchPad.CrcReg = DS18B20_ReadByte();
		return true;
	}
	else
		return false;
}


uint8_t DS18B20_ReadByte()
{
	uint8_t temp = 0;

	for (int i = 0; i < 8; i++)
	{
		temp |= (DS18B20_ReadBit() << i);
		__NOP();
	}

	return temp;
}



uint8_t DS18B20_ReadBit()
{
	uint8_t temp = 0;

	// Pull line down
	DS18B20_OutputSet();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	// Release line
	DS18B20_InputSet();

	// Reset timer 2 counter and sample line until sample time elapses
	TIM2->CNT = 0;
	while(TIM2->CNT < READTIMESLOT_SAMPLETIME)
	{
		temp = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
	}

	// Reset timer 2 counter to complete slot time...No need for recovery time
	TIM2->CNT = 0;
	while(TIM2->CNT < READTIMESLOT_SLOTTIME);


	return temp;
}





void DS18B20_WriteByte(uint8_t CommandValue)
{
	uint8_t temp = CommandValue;

	for (int i = 0; i < 8; i++)
	{
		DS18B20_WriteBit(temp & 1);
		temp >>= 1;
	}
}



void DS18B20_WriteBit(uint8_t bitValue)
{
	if (bitValue == 0)
	{
		DS18B20_OutputSet();
		// Pull line down
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		//Tim2Delay_us(WRITETIMESLOT_LOWTIME);
		Tim2Delay_us(WRITETIMESLOT_SAMPLETIME);
		DS18B20_InputSet();
		Tim2Delay_us(WRITETIMESLOT_RECOVERYTIME);
		DS18B20_InputSet();
	}
	else if (bitValue == 1)
	{
		DS18B20_OutputSet();
		// Pull line down
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		//Tim2Delay_us(WRITETIMESLOT_LOWTIME);
		DS18B20_InputSet();
		Tim2Delay_us(WRITETIMESLOT_SAMPLETIME);
		Tim2Delay_us(WRITETIMESLOT_RECOVERYTIME);
		DS18B20_InputSet();
	}
}





bool DS18B20_ResetSequence()
{
	int i;

	// Pull line low
	DS18B20_OutputSet();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

	// Wait at least 500 us
	Tim2Delay_us(500);

	// Release bus to wait for presence pulse
	DS18B20_InputSet();

	// Wait for sensor to pull line low
	i = 0;
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) ==  GPIO_PIN_SET)
	{
		if(i > 500)
		{
			// Sensor not responding after 500 us wait...error
			return false;
		}
		Tim2Delay_us(1);
		i++;
	}

	// Sensor pulled line low... Wait for sensor to release line
	i = 0;
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) ==  GPIO_PIN_RESET)
	{
		if(i > 500)
		{
			// Sensor not responding after 500 us wait...error
			return false;
		}
		Tim2Delay_us(1);
		i++;
	}

	Tim2Delay_us(WRITETIMESLOT_RECOVERYTIME);

	return true;
}







void DS18B20_OutputSet()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}



void DS18B20_InputSet()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}












