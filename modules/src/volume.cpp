/*
 * biquad.cpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#include "math.h"
#include "../inc/volume.hpp"
//#include "stdio.h"


void c_volume::init(void){

	set_volume(initial_volume);

}

void c_volume::set_volume(float v){

	if(v>1){
		gain=pregain*(fastPow(10,(double)v*0.01)-1);
	}else{
		//Absolute silence
		gain=0;
	}

//	printf("Volume:%f \t Gain: %f\n",*v,gain);
}

float c_volume::process(float x){

	return x*gain;

}


