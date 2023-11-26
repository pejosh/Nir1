/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN Private defines */
typedef uint32_t BME280_S32_t;

#define SPI_BUFFER_SIZE 50
#define SPI_READ_BIT_MASK 0b11111111;
#define SPI_WRITE_BIT_MASK 0b01111111;
// BME80 REGISTER ADDRESS
#define ADD_CAL00   	0x88
#define ADD_CAL25   	0xA1
#define ADD_ID			0xD0
#define ADD_RESET   	0xE0
#define ADD_CAL26 	    0xE1
#define ADD_CAL41    	0xF0
#define ADD_CTRHUM  	0xF2
#define ADD_STATUS  	0xF3
#define ADD_CTRMEAS  	0xF4
#define ADD_CONFIG  	0xF5
#define ADD_PRESSMSB    0xF7
#define ADD_PRESSLSB    0xF8
#define ADD_PRESSXLSB   0xF9
#define ADD_TEMPMSB     0xFA
#define ADD_TEMPLSB     0xFB
#define ADD_TEMPXLSB    0xFC
#define ADD_HUMMSB      0xFD
#define ADD_HUMLSB      0xFE

extern uint8_t SpiBuffer[SPI_BUFFER_SIZE];
/* USER CODE END Private defines */

void MX_SPI2_Init(void);

/* USER CODE BEGIN Prototypes */
int16_t TestSpi();
float SpiConvertValue(uint16_t inputValue);
void SpiChipSelectEnable();
void SpiChipSelectDisable();
void SpiRead(uint8_t address, uint8_t *pData, uint16_t numBytes);
void SpiWrite(uint8_t *pAddress, uint8_t *pData, uint8_t numBytes);
void SpiRxCallback();
void SpiTxCallback();
void SpiTxRxCallback();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

