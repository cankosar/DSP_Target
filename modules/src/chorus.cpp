/*
 * chorus.cpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

//#include "stdio.h"
#include "string.h"
#include "math.h"
#include "../../dsp/inc/constants.hpp"
#include "../inc/chorus.hpp"
#include "stm32h7xx_hal.h"

void c_chorus::init(void){

//	printf("Init chorus\n");

	//Initialize parameters
	set_wet(&initial_wet);
	set_depth(&initial_depth);
	set_rate(&initial_rate);

	//Resetting buffer
	reset_buffer();
}

void c_chorus::stop(void){

	//Status set
	status=0;

	//Fill the delay buffer with zeros
	reset_buffer();

}

void c_chorus::start(void){

	//Status set
	status=1;
}

void c_chorus::set_wet(float *w){
	G_w=*w*0.01;
	G_d=1-G_w;
}

void c_chorus::set_depth(float *d){
	depth=*d*0.01;
}

void c_chorus::set_rate(float *r){
	rate=*r;
	a_step=(rate*2*PI)/(float)FS;
}

void c_chorus::reset_buffer(void){
	//Fill the delay buffer with zeros
	//The size of the float is 4

	memset(cbuf, 0, chorus_len*sizeof(*cbuf));

	//Chorus pointer
	cptr=0;

	//Current angle of LFO
	a_lfo=0;
}

float c_chorus::process(float x){

	float y;
//	printf("cp\n");

	float t_c;	//Current delay

	//Get tap distance
	t_c=get_current_delay(); //Tap distance

	//Calculate the position of the pointer
	int wptr;

	wptr=cptr-lrint(t_c*FSms);

	if(wptr<0){
		wptr+=chorus_len;
	}

	//Calculate output
	y=G_d*x + G_w*cbuf[wptr];

	//Update the buffer
	cbuf[cptr]=x;

	//Increment
	cptr++;
	if(cptr>=chorus_len){
		cptr-=chorus_len;
	}

	return y;
}


float c_chorus::get_current_delay(void){

	//Calculate time
	float t;
	t=d_base+depth*lfo();

	//Return the delay time
	return t;

}


float c_chorus::lfo(void){

	float y_lfo;

	//Calculate LFO value
	y_lfo=sin(a_lfo);

	//Update angle
	a_lfo+=a_step;
	if(a_lfo>=2*PI){
		a_lfo=a_lfo-2*PI;
	}

	return y_lfo;
}
