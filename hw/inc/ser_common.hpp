/*
 * ser_common.hpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */

#ifndef INC_SER_COMMON_HPP_
#define INC_SER_COMMON_HPP_



#include "stm32h7xx_hal.h"

#define buf_size 240
uint32_t __attribute__((section (".dmadata")))  rx_buf[buf_size*4];
uint32_t __attribute__((section (".dmadata")))  tx_buf[buf_size*4];



#endif /* INC_SER_COMMON_HPP_ */
