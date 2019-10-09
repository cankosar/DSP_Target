/*
 * cs4272.cpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "../inc/cs4272.hpp"



void cs4272::init(void){


	//Initialize SPI
	printf("Initing SPI5\n");
	MX_SPI5_Init();


	printf("Initing codec\n");

	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,(GPIO_PinState)0); //RST low

		uint8_t i=0;
		uint8_t reg[8][3];
		uint8_t adr=0b00100000;

		//Init address
		for(i=0;i<sizeof(reg)/3;i++){
			reg[i][0]=adr;
		}

		//Define startup sequence
		reg[0][1]=0x7; //Power down control
		reg[0][2]=0b00000011; //Set Serial mode + power down

		reg[1][1]=0x1; //Mode control 1
		reg[1][2]=0b00101001; //48kHz Master

		reg[2][1]=0x2;	//DAC Control
		reg[2][2]=0b00000000;

		reg[3][1]=0x3; //DAC Volume control
		reg[3][2]=0b00001001;

		reg[4][1]=0x4;	//DAC Volume A
		reg[4][2]=0b10000000; //No mute + 0dB

		reg[5][1]=0x5;	//DAC Volume B
		reg[5][2]=0b10000000; //No mute + 0dB

		reg[6][1]=0x6;	//ADC Control
		reg[6][2]=0b00010000;

		reg[7][1]=0x7;	//Mode control 2
		reg[7][2]=0b00001010;
	//	reg[7][2]=0b00010010;//Loop ADC->DAC


		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)1); //MS high: SPI Unselect device
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,(GPIO_PinState)1); //RST High
		HAL_Delay(2); //Wait
		//HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,0); //RST Low

		for(i=0;i<sizeof(reg)/3;i++){
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)0); //MS low: SPI Select device
			HAL_SPI_Transmit(&hspi5,(uint8_t *)&reg[i],3,1000); //Send config register
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)1);//MS high: SPI Unselect device
			HAL_Delay(1); //Wait

		}
}


void cs4272::set_status(bool flag){
	uint8_t mutereg[3];
	mutereg[0]=0b00100000;
	mutereg[1]=0x4; //DAC Volume control A register (MUTECA=B is on)

	if(flag){
		mutereg[2]=0b00000000;
	}else{
		mutereg[2]=0b10000000;
	}

	//Send SPI command to CODEC
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)0); //MS low: SPI Select device
	HAL_SPI_Transmit(&hspi5,(uint8_t *)&mutereg,3,1000); //Send config register
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)1);//MS high: SPI Unselect device
}


void cs4272::MX_SPI5_Init(void){

  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT; //16 bit for WM8731
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 7;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

}

#ifdef __cplusplus
}
#endif
