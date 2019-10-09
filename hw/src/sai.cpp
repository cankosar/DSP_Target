/*
 * sai.cpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */


#ifdef __cplusplus
extern "C" {
#endif

//#include "../inc/common_handlers.hpp"
#include "../inc/sai.hpp"
#include "../inc/ser_common.hpp"

extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;

void c_sai::init(void){

	MX_SAI1_Init();
}



void c_sai::MX_SAI1_Init(void)
{

  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODESLAVE_RX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_OUTBLOCKA_ENABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_32BIT, 2) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

  hsai_BlockB1.Instance = SAI1_Block_B;
  hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_TX;
  hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
  hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockB1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_32BIT, 2) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

}

void c_sai::start(void){

	//Start DMA Streams
	printf("Initializing SAI DMA Receive stream...\n");
	HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*)&rx_buf, 4*buf_size);
	printf("SAI DMA Receive stream initialized!\n");
	printf("Initializing SAI DMA transmit stream...\n");
	HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t*)&tx_buf, 4*buf_size);
	printf("SAI DMA transmit stream initialized!\n");

}





void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
	printf("Tx\n");
//	tx_status=0;
}


void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai){
//	tx_status=1;
//		printf("TXH\n");
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai){

	//	HAL_GPIO_TogglePin(GPIOB,LD2_Pin);
	printf("Rx\n");
//	rx_status=0;

}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai){
//	rx_status=1;
}

#ifdef __cplusplus
}
#endif
