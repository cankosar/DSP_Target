/*
 * ser_common.hpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */

#ifndef INC_SER_COMMON_HPP_
#define INC_SER_COMMON_HPP_



#include "stm32h7xx_hal.h"
#include "hwconfig.hpp"


int32_t __attribute__((section (".dmadata")))  rx_buf[buf_size*4];
int32_t __attribute__((section (".dmadata")))  tx_buf[buf_size*4];

//bool dsp_r,dsp_w; //Active half of DSP array

bool tx_status;
bool rx_status;

#endif /* INC_SER_COMMON_HPP_ */
