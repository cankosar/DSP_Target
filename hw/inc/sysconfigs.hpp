/*
 * sysconfigs.hpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */

#ifndef INC_SYSCONFIGS_HPP_
#define INC_SYSCONFIGS_HPP_

#include "stm32h7xx_hal.h"

class sysconfig{

	public:
		void init(void);

	private:
		static const uint32_t SWD_SPD=2000000;
		void SystemClock_Config(void);
		void MX_GPIO_Init(void);
		void SWD_Init(void);
};


#endif /* INC_SYSCONFIGS_HPP_ */
