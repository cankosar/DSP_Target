/*
 * com_common.hpp
 *
 *  Created on: 13.10.2019
 *      Author: Can
 */

#ifndef INC_COM_COMMON_HPP_
#define INC_COM_COMMON_HPP_

#include "stm32h7xx_hal.h"
#include "../../../common/common.hpp"

int32_t __attribute__((section (".dmadata")))  ctrl_tx[l_ctrl*4];
int32_t __attribute__((section (".dmadata")))  ctrl_rx[l_ctrl*4];


//Waiting time in ms
#define t_com_wait 200

union ctrltypes rx_com[l_ctrl];
union ctrltypes tx_com[l_ctrl];

//Instance
c_com_ctrl com;

#endif /* INC_COM_COMMON_HPP_ */
