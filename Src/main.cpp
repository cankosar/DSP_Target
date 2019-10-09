/*
 * Main Program
 */

#ifdef __cplusplus
extern "C" {
#endif

//Include component headers
#include "../hw/inc/sysconfigs.hpp"
#include "../hw/inc/cs4272.hpp"
#include "../hw/inc/sai.hpp"
#include "../com/inc/com_ctrl.hpp"
#include "serialize.hpp"

//Instantiate configures
sysconfig conf;

//Instantiate codec
cs4272 codec;

//Instantiate communication stack
com_ctrl com;

//Instantiate SAI
c_sai sai;

//Main routine
int main(void){

	//Load system configurations
	conf.init();

	//Initialize communication
	com.init();

	//Initialize codec
	codec.init();

	//Activate codec
	codec.set_status(1);

	//Initialize SAI
	sai.init();

	//Start SAI
	sai.start();

	//HW test: move to serializer class
	while(1){
	}

	return 0;
}

#ifdef __cplusplus
}
#endif
