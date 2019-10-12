/*
 * overdrive.cpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */
#include "stdio.h"
#include "math.h"
#include "../inc/overdrive.hpp"


void c_overdrive::init(void){

	set_gain(&initial_gain);
	set_HP_freq(&initial_HP_freq);
	set_LP_freq(&initial_LP_freq);

	//Init intermediate parameters
	downscaler=1/upscaler;

}

void c_overdrive::start(void){

	//Start filters
	pre_filter.start();
	post_filter.start();

	//Set status
	status=1;
}

void c_overdrive::stop(void){

	//Set status
	status=0;

	//Stop filters
	pre_filter.stop();
	post_filter.stop();
}

void c_overdrive::set_gain(float *g){

	gain=*g;

}

void c_overdrive::set_HP_freq(float *f){

	pre_filter.apply_filter(1,0,*f,1);

}

void c_overdrive::set_LP_freq(float *f){

	post_filter.apply_filter(0,0,*f,1);

}

float c_overdrive::process(float x){

	float y;

	x=pre_filter.process(&x);

	x=x*downscaler*gain;

	//Algorithm 1: y=sign(x)*(1-e(sign(x)*x))
	if(x>0){
		y=1-exp(-1*x);
	}else{
		y=-1+exp(x);
	}

	y=y*upscaler;

//	y=x;

	y=post_filter.process(&y);

	return y;
}


