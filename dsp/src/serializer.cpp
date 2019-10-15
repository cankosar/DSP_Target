/*
 * serializer.cpp
 *
 *  Created on: 10.10.2019
 *      Author: Can
 */

#ifdef __cplusplus
extern "C" {
#endif


#include "../inc/serializer.hpp"
#include "stm32h7xx_hal.h"
#include "../../hw/inc/hwconfig.hpp"
#include "../inc/dsp_common.hpp"

extern c_dsp dsp;
extern bool tx_status;
extern bool rx_status;
extern int32_t rx_buf[buf_size*4];
extern int32_t tx_buf[buf_size*4];

void c_ser::init(void){

	tx_status=0;
	rx_status=0;


	//Initialize DSP
	dsp.init();

	//Update DSP - should be obsolete
	dsp.update_bank_states(0);

}

void c_ser::start(void){



	bool old_pos=1;
	unsigned i;
	//Endless main loop
	while(1){

		if((tx_status && rx_status)&&old_pos){ //When pointer is on the second half of the buffer

//			printf("1\n");
			for(i=0;i<buf_size*2;i+=2){
				tx_buf[i]=dsp.process(&rx_buf[i]); //(mono right)
//				dsp.test(i);

			}
			old_pos=0;

		}else if(!(tx_status || rx_status)&&!old_pos){//When pointer is on the first half of the buffer

//			printf("2\n");
			for(i=buf_size*2;i<buf_size*4;i+=2){
				tx_buf[i]=dsp.process(&rx_buf[i]); //(mono right)
//				dsp.test(i);

			}
			old_pos=1;
		}else{
			//Can measure idle here
		}

	}



}



#ifdef __cplusplus
}
#endif
