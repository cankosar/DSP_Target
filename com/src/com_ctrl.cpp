/*
 * com_ctrl.cpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */



#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include "../inc/com_ctrl.hpp"
#include "../../hw/inc/common_handlers.hpp"
#include "../inc/com_common.hpp"
#include "./../dsp/inc/dsp.hpp"
//#include "../../dsp/inc/dsp_common.hpp"

extern int32_t ctrl_tx[l_ctrl*4];
extern int32_t ctrl_rx[l_ctrl*4];

extern c_dsp dsp;

void com_ctrl::init(void){

	MX_SPI3_Init();

}

void com_ctrl::start(void){

	//Start DMA stream
	HAL_SPI_TransmitReceive_DMA(&hspi3,(uint8_t*)ctrl_tx, (uint8_t*) ctrl_rx, 4*l_ctrl);
}

void com_ctrl::MX_SPI3_Init(void){

	printf("Init SPI3\n");

  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_SLAVE;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
//    printf("Error SPI3\n");
  }

}

void HAL_SPI_TxRxCpltCallback (SPI_HandleTypeDef * hspi){

//	rx_com=ctrl_rx;

	memcpy(rx_com,ctrl_rx,4*l_ctrl);

	printf("Bank: %d\t"
			"Type:%d\t"
			"Id:%d\t"
			"Value:%lu\n",rx_com[0].u8[0],
			rx_com[0].u8[1],
			rx_com[0].u8[2],
			rx_com[1].u32);


	if(rx_com[0].u8[0]==4){
		if(rx_com[0].u8[1]==0){
			if(rx_com[0].u8[2]==2){
				printf("Setting delay to %ld\n",(int32_t)rx_com[1].f32);
				dsp.delay.set_time(&rx_com[1].f32);
			}
		}
	}




}


#ifdef __cplusplus
}
#endif
