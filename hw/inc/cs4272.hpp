/*
 * cs4272.hpp
 *
 *  Created on: 09.10.2019
 *      Author: Can
 */

#ifndef INC_CS4272_HPP_
#define INC_CS4272_HPP_

#include "stm32h7xx_hal.h"



class cs4272{
	public:
		void init(void);
		void set_status(bool flag);

	private:
		SPI_HandleTypeDef hspi5;
		void MX_SPI5_Init(void);
};



#endif /* INC_CS4272_HPP_ */
