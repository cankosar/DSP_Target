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
extern c_com_ctrl com;

void c_com_ctrl::init(void){

	MX_SPI3_Init();

}

void c_com_ctrl::start(void){

	//Start DMA stream
	HAL_SPI_TransmitReceive_DMA(&hspi3,(uint8_t*)ctrl_tx, (uint8_t*) ctrl_rx, 4*l_ctrl);
}

void c_com_ctrl::MX_SPI3_Init(void){

//	printf("Init SPI3\n");

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

void c_com_ctrl::apply_update(void)
{
	//Copy incoming message to union
	memcpy(rx_com,ctrl_rx,4*l_ctrl);

//	printf("Bank: %d\t Type:%d\t Id:%d\t Value:%lu\n",rx_com[0].u8[0],rx_com[0].u8[1],	rx_com[0].u8[2],rx_com[1].u32);

	//Update bank states
	if(rx_com[0].u8[1]==1 && rx_com[0].u8[2]==0){
		dsp.update_bank_states(rx_com[1].u32);
	}

	switch(rx_com[0].u8[0]){

		case c_delay_bank:
			//Delay
			if(rx_com[0].u8[1]==0){
				//Encoders
				switch(rx_com[0].u8[2]){	//Switch on encoders
					case 0:
						dsp.delay.set_dry(&rx_com[1].f32);
						break;
					case 1:
						dsp.delay.set_feedback(&rx_com[1].f32);
						break;
					case 2:
						dsp.delay.set_time(&rx_com[1].f32);
						break;
				}

			}
			break;

		case c_chorus_bank:
			//chorus
			if(rx_com[0].u8[1]==0){
				//Encoders
				switch(rx_com[0].u8[2]){	//Switch on encoders
					case 0:
						dsp.chorus.set_wet(&rx_com[1].f32);
						break;
					case 1:
						dsp.chorus.set_depth(&rx_com[1].f32);
						break;
					case 2:
						dsp.chorus.set_rate(&rx_com[1].f32);
						break;
				}

			}

			break;
		case c_overdrive_bank:
			//Overdrive
			if(rx_com[0].u8[1]==0){
				//Encoders
				switch(rx_com[0].u8[2]){	//Switch on encoders
					case 0:
						dsp.overdrive.set_gain(&rx_com[1].f32);
						break;
					case 1:
						dsp.overdrive.set_HP_freq(&rx_com[1].f32);
						break;
					case 2:
						dsp.overdrive.set_HP_freq(&rx_com[1].f32);
						break;
				}

			}

			break;

		case c_reverb_bank:
			//Reverb
			if(rx_com[0].u8[1]==0){
				//Encoders
				switch(rx_com[0].u8[2]){	//Switch on encoders
					case 0:
						dsp.reverb.set_wet(&rx_com[1].f32);
						break;
					case 1:
						dsp.reverb.set_size(&rx_com[1].f32);
						break;
					case 2:
						dsp.reverb.set_damp(&rx_com[1].f32);
						break;
				}

			}

			break;

		case c_tremolo_bank:
			//Tremolo
			if(rx_com[0].u8[1]==0){
				//Encoders
				switch(rx_com[0].u8[2]){	//Switch on encoders
					case 0:
						dsp.tremolo.set_depth(&rx_com[1].f32);
						break;
					case 1:
						dsp.tremolo.set_freq(&rx_com[1].f32);
						break;
					case 2:
						dsp.tremolo.set_type(&rx_com[1].f32);
						break;
				}

			}

			break;

		case c_rotary_bank:
			//Rotary
			if(rx_com[0].u8[1]==0){
				//Encoders
				switch(rx_com[0].u8[2]){	//Switch on encoders
					case 0:
						dsp.rotary.set_depth(&rx_com[1].f32);
						break;
					case 1:
						dsp.rotary.set_freq(&rx_com[1].f32);
						break;
					case 2:
						break;
				}

			}

			break;

		case c_compressor_bank:
			//Compressor
			if(rx_com[0].u8[1]==0){
				//Encoders
				switch(rx_com[0].u8[2]){	//Switch on encoders
					case 0:
						dsp.compressor.set_threshold(&rx_com[1].f32);
						break;
					case 1:
						dsp.compressor.set_ratio(&rx_com[1].f32);
						break;
					case 2:
						dsp.compressor.set_attack(&rx_com[1].f32);
						break;
				}

			}

			break;

	}


	if(rx_com[0].u8[0]==4){
		if(rx_com[0].u8[1]==0){
			if(rx_com[0].u8[2]==2){
				dsp.delay.set_time(&rx_com[1].f32);
			}
		}
	}



}

void HAL_SPI_TxRxCpltCallback (SPI_HandleTypeDef * hspi){

	//Call main routine applying the updates
	com.apply_update();
}


#ifdef __cplusplus
}
#endif
