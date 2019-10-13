/*
 * com_common.hpp
 *
 *  Created on: 13.10.2019
 *      Author: Can
 */

#ifndef INC_COM_COMMON_HPP_
#define INC_COM_COMMON_HPP_

#include "stm32h7xx_hal.h"

#define l_ctrl 6
int32_t __attribute__((section (".dmadata")))  ctrl_tx[l_ctrl*4];
int32_t __attribute__((section (".dmadata")))  ctrl_rx[l_ctrl*4];

union ctrltypes{
	float f32;
	uint32_t u32;
	uint16_t u16[2];
	uint8_t u8[4];
};

union ctrltypes rx_com[l_ctrl];
union ctrltypes tx_com[l_ctrl];

#endif /* INC_COM_COMMON_HPP_ */
