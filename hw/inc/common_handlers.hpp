/*
 * hw_handlers.hpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */

#ifndef INC_COMMON_HANDLERS_HPP_
#define INC_COMMON_HANDLERS_HPP_

#include "stm32h7xx_hal.h"

//Communication stack
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;

//SAI and its DMA handlers
SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;
DMA_HandleTypeDef hdma_sai1_a;
DMA_HandleTypeDef hdma_sai1_b;

#endif /* INC_COMMON_HANDLERS_HPP_ */
