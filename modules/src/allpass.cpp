/*
 * chorus.cpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#include "stdio.h"
#include "string.h"
//#include "math.h"
//#include "../../inc/constants.hpp"
#include "../inc/allpass.hpp"
#include "../inc/reverb_parameters.hpp"
//#include "../inc/denormals.h"

void allpass::init(float* bufptr, unsigned size){

	//Set the buffer to the incoming pointer
	buffer=bufptr;
	bufsize=size;

	//Initialize pointer
	aptr=0;

	//Resetting buffer
	reset_buffer();



}
void allpass::start(void){

	//Status set
	status=1;

}

void allpass::stop(void){

	//Status set
	status=0;

	//Fill the delay buffer with zeros
	reset_buffer();

}

void allpass::reset(void){

	//Fill the delay buffer with zeros
	reset_buffer();
}

void allpass::reset_buffer(void){

	//Fill the delay buffer with zeros
	memset(buffer, 0, bufsize*sizeof(*buffer));

//	unsigned i;
//	for(i=0;i<bufsize;i++){
//		buffer[i]=0;
//	}
}

void allpass::set_feedback(float *fb){

	feedback=*fb;

}


float allpass::process(float x){

	float y;
	float bufout;

	bufout = buffer[aptr];
//	printf("aptr=%d\n",aptr);
//	undenormalise(bufout);

//	y = -x + bufout;
//	buffer[aptr] = x + (bufout*feedback);

	buffer[aptr] = x + (bufout*feedback);
	y = -buffer[aptr]*feedback + bufout;

	if(++aptr>=bufsize) aptr = 0;

	return y;
}
