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
#include "../dsp/inc/serializer.hpp"

//Instantiate configures
sysconfig conf;

//Instantiate codec
cs4272 codec;

//Instantiate communication stack
com_ctrl com;

//Instantiate SAI
c_sai sai;

c_ser ser;

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

	//Initialize serializer
	ser.init();

	//Start serializer
	ser.start();


	printf("Error. Main loop broken\n");




	return 0;
}

#ifdef __cplusplus
}
#endif
