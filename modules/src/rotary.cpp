/*
 * chorus.cpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#include "stdio.h"
#include "string.h"
#include "math.h"
#include "../../dsp/inc/constants.hpp"
#include "../inc/rotary.hpp"


void c_rotary::init(void){

	//Set initial parameters
	set_depth(&initial_depth);
	set_freq(&initial_freq);

	//Resetting buffer
	reset_buffer();

	//Chorus pointer
	rptr=0;

	//Current angle of LFO
	a_lfo=0;
}

void c_rotary::update_step(void){

	//Angle step
	a_step=(freq*2*PI)/(FS);
}

void c_rotary::start(void){

	//Set status
	status=1;
}

void c_rotary::stop(void){

	//Set status
	status=0;

	//Fill the delay buffer with zeros
	reset_buffer();

	//Chorus pointer
	rptr=0;

	//Current angle of LFO
	a_lfo=0;
}

void c_rotary::reset_buffer(void){
	//Fill the delay buffer with zeros
	//The size of the float is 4

	memset(rbuf, 0, rotary_len*sizeof(*rbuf));
}

void c_rotary::set_depth(float* d){

	depth=*d*0.01;
}

void c_rotary::set_freq(float* f){

	freq=*f;
	update_step();
}


float c_rotary::process(float x){

	float y;

	float t_c;	//Current delay

	//Get current delay
	t_c=get_current_delay(); //Delay time

	//Calculate the position of the pointer
	int wptr;

	wptr=rptr-lrint(t_c*FSms);

//	printf("Ptr:%d Dist: %d\n",wptr,lrint(t_c*FSms));

	if(wptr<0){
		wptr+=rotary_len;
	}

	//Calculate output
	y=rbuf[wptr];

	//Update the buffer
	rbuf[rptr]=x;

	//Increment
	rptr++;
	if(rptr>=rotary_len){
		rptr-=rotary_len;
	}

	return y;
}


float c_rotary::get_current_delay(void){

	//Calculate time
	float t;
	t=d_base+depth*lfo();

	//Return the delay time
	return t;

}


float c_rotary::lfo(void){

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
