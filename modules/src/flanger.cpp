/*
 * chorus.cpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

//#include "stdio.h"
#include "string.h"
#include "math.h"
#include "../inc/flanger.hpp"


void c_flanger::init(void){

	//Initialize parameters
	set_wet(&initial_wet);
	set_wet(&initial_feedback);
	set_depth(&initial_depth);
	set_rate(&initial_rate);

	//Resetting buffer
	reset_buffer();
}

void c_flanger::stop(void){

	//Status set
	status=0;

	//Fill the delay buffer with zeros
	reset_buffer();

}

void c_flanger::start(void){

	//Status set
	status=1;
}

void c_flanger::set_feedback(float *fb){
	G_fb=*fb*0.0091;
}

void c_flanger::set_wet(float *w){
	G_w=*w*0.01;
	G_d=1-G_w;
}

void c_flanger::set_depth(float *d){
	depth=*d*0.03;
}

void c_flanger::set_rate(float *r){
	rate=*r;
	a_step=(rate*2*PI)/(float)FS;
}

void c_flanger::reset_buffer(void){
	//Fill the delay buffer with zeros
	//The size of the float is 4

	memset(fbuf, 0, flanger_len*sizeof(*fbuf));
	memset(drybuf, 0, l_drybuf*sizeof(*drybuf));

	//Chorus pointer
	fptr=0;

	//Dry pointer
	dryptr=0,

	//Current angle of LFO
	a_lfo=0;
}

float c_flanger::process(float x){

	float y;

	float t_c;	//Current delay

	//Get tap distance
	t_c=get_current_delay(); //Tap distance

	//Calculate the position of the pointer
	int wptr;

	wptr=fptr-lrint(t_c*FSms);

	if(wptr<0){
		wptr+=flanger_len;
	}

	//Calculate output
	y=G_d*drybuf[dryptr] + G_w*fbuf[wptr];

	//Update the buffer
	fbuf[fptr]=x+G_fb*fbuf[wptr];
	drybuf[dryptr]=x;

	//Increment
	fptr++;
	if(fptr>=flanger_len){
		fptr=0;
	}

	dryptr++;
	if(dryptr>=l_drybuf){
		dryptr=0;
	}

	return y;
}

float c_flanger::process_nondelay(float x){

	float y;

	float t_c;	//Current delay

	//Get tap distance
	t_c=get_current_delay(); //Tap distance

	//Calculate the position of the pointer
	int wptr;

	wptr=fptr-lrint(t_c*FSms);

	if(wptr<0){
		wptr+=flanger_len;
	}

	//Calculate output
	y=G_d*x + G_w*fbuf[wptr];

	//Update the buffer
	fbuf[fptr]=x+G_fb*fbuf[wptr];

	//Increment
	fptr++;
	if(fptr>=flanger_len){
		fptr=0;
	}

	return y;
}


float c_flanger::get_current_delay(void){

	//Calculate time
	float t;
	t=(float)d_base+depth*lfo();

	//Return the delay time
	return t;

}


float c_flanger::lfo(void){

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
