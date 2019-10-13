/*
 * com_ctrl.hpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */

#ifndef COM_CTRL_HPP_
#define COM_CTRL_HPP_

#include "stm32h7xx_hal.h"

class com_ctrl{

	public:
		void init(void);
		void start(void);

	private:

		void MX_SPI3_Init(void);


};



#endif /* COM_CTRL_HPP_ */
