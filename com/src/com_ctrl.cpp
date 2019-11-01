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

void c_com_ctrl::setup_uC_com(void){



	//Wait until uC is ready and set up the ready flag
	while(1){

			//Copy incoming message to union
			memcpy(rx_com,ctrl_rx,4*l_ctrl);

			if(rx_com[n_cmd].u8[i_bank]==i_com_bank && rx_com[n_val].u32 == uC_ready){
				//Set the ready flag high

//				LOCK=1;
				tx_com[n_cmd].u8[i_bank]=i_com_bank;
				tx_com[n_cmd].u8[i_type]=0;
				tx_com[n_cmd].u8[i_id]=0;
				tx_com[n_cmd].u8[i_reserved]=0;

				tx_com[n_val].u32=DSP_ready;

				//Copy outgoing message to SPI buffer
				memcpy(ctrl_tx,tx_com,4*l_ctrl);

				HAL_Delay(2*t_com_wait);
				LOCK=0;
				printf("Communication to uC is up!\n");
				break;
			}else{
				HAL_Delay(t_com_wait);
				printf("Waiting for uC!\n");
			}
		}

}

void c_com_ctrl::provide_tuner_freq(float *freq){

	if(!LOCK){
		//Set the tuner frequency
		tx_com[n_cmd].u8[i_bank]=bankid_tuner;
		tx_com[n_cmd].u8[i_type]=0;
		tx_com[n_cmd].u8[i_id]=i_tuner_freq;
		tx_com[n_cmd].u8[i_reserved]=0;

		tx_com[n_val].f32=*freq;

		//Copy outgoing message to SPI buffer
		memcpy(ctrl_tx,tx_com,4*l_ctrl);
	}
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

//	printf("b\n");
	//Update bank states
	if(rx_com[n_cmd].u8[i_type]==1 && rx_com[n_cmd].u8[i_id]==0){
		dsp.update_bank_states(rx_com[n_val].u32);
	}

	switch(rx_com[0].u8[0]){


		case bankid_lowshelf:
			//Low shelf
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.lowshelf.set_gain(rx_com[n_val].f32);
						break;
					case 1:
						dsp.lowshelf.set_freq(rx_com[n_val].f32);
						break;
					case 2:
						dsp.lowshelf.set_quality(rx_com[n_val].f32);
						break;
				}
			}
			break;

		case bankid_lowmid:
			//Low mid
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.lowmid.set_gain(rx_com[n_val].f32);
						break;
					case 1:
						dsp.lowmid.set_freq(rx_com[n_val].f32);
						break;
					case 2:
						dsp.lowmid.set_quality(rx_com[n_val].f32);
						break;
				}
			}
			break;

		case bankid_mid:
			//Mid
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.mid.set_gain(rx_com[n_val].f32);
						break;
					case 1:
						dsp.mid.set_freq(rx_com[n_val].f32);
						break;
					case 2:
						dsp.mid.set_quality(rx_com[n_val].f32);
						break;
				}
			}
			break;

		case bankid_highmid:
			//High mid
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.highmid.set_gain(rx_com[n_val].f32);
						break;
					case 1:
						dsp.highmid.set_freq(rx_com[n_val].f32);
						break;
					case 2:
						dsp.highmid.set_quality(rx_com[n_val].f32);
						break;
				}
			}
			break;

		case bankid_highshelf:
			//High shelf
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.highshelf.set_gain(rx_com[n_val].f32);
						break;
					case 1:
						dsp.highshelf.set_freq(rx_com[n_val].f32);
						break;
					case 2:
						dsp.highshelf.set_quality(rx_com[n_val].f32);
						break;
				}
			}
			break;

		case bankid_delay:
			//Delay
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.delay.set_dry(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.delay.set_time(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.delay.set_feedback(&rx_com[n_val].f32);
						break;
				}

			}
			break;

		case bankid_chorus:
			//chorus
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.chorus.set_wet(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.chorus.set_depth(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.chorus.set_rate(&rx_com[n_val].f32);
						break;
				}

			}

			break;

		case bankid_overdrive:
			//Overdrive
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.overdrive.set_gain(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.overdrive.set_HP_freq(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.overdrive.set_HP_freq(&rx_com[n_val].f32);
						break;
				}

			}

			break;

		case bankid_reverb:
			//Reverb
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.reverb.set_wet(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.reverb.set_size(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.reverb.set_damp(&rx_com[n_val].f32);
						break;
				}

			}

			break;

		case bankid_tremolo:
			//Tremolo
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.tremolo.set_depth(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.tremolo.set_freq(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.tremolo.set_type(&rx_com[n_val].f32);
						break;
				}

			}

			break;

		case bankid_rotary:
			//Rotary
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.rotary.set_depth(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.rotary.set_freq(&rx_com[n_val].f32);
						break;
					case 2:
						break;
				}

			}

			break;

		case bankid_comp:
			//Compressor
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.compressor.set_threshold(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.compressor.set_ratio(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.compressor.set_attack(&rx_com[n_val].f32);
						break;
				}

			}

			break;

		case bankid_flanger:
			//Flanger
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.flanger.set_wet(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.flanger.set_rate(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.flanger.set_feedback(&rx_com[n_val].f32);
						break;
				}

			}

			break;

		case bankid_autowah:
			//Autowah
			if(rx_com[n_cmd].u8[i_type]==type_enc){
				//Encoders
				switch(rx_com[n_cmd].u8[i_id]){	//Switch on encoders
					case 0:
						dsp.autowah.set_depth(&rx_com[n_val].f32);
						break;
					case 1:
						dsp.autowah.set_rate(&rx_com[n_val].f32);
						break;
					case 2:
						dsp.autowah.set_quality(&rx_com[n_val].f32);
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
