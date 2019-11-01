/*
 * com_ctrl.hpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */

#ifndef COM_CTRL_HPP_
#define COM_CTRL_HPP_

#include "stm32h7xx_hal.h"

class c_com_ctrl{

	public:
		void init(void);
		void start(void);
		void apply_update(void);
		void setup_uC_com(void);
		void provide_tuner_freq(float *freq);

	private:

		bool LOCK=1;
		void MX_SPI3_Init(void);
};




#endif /* COM_CTRL_HPP_ */
