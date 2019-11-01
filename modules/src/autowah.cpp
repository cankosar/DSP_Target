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
#include "../inc/autowah.hpp"


void c_autowah::init(void){

	//Initialize parameters
	set_input_gain(&initial_input_gain);
	set_quality(&initial_quality);
	set_depth(&initial_depth);
	set_rate(&initial_rate);
	set_freq_low(&initial_freq_low);
	set_freq_high(&initial_freq_high);

	//Resetting buffer
	reset_buffer();

	//Initialize wahfilter
	wahfilter.init();
	wahfilter.set_filter_type(2);
	wahfilter.set_gain(depth);
	wahfilter.set_quality(Q);

	//Set update
	i_update=0;
}

void c_autowah::stop(void){

	//Status set
	status=0;

	//Fill the delay buffer with zeros
	reset_buffer();

}

void c_autowah::start(void){

	//Status set
	status=1;
}

void c_autowah::set_quality(float *q){
	Q=*q;
}

void c_autowah::set_input_gain(float *g){
	input_gain=*g;
}

void c_autowah::set_depth(float *d){
	depth=gain_base+*d*gain_scaler;
}

void c_autowah::set_freq_low(float *fl){

	freq_low=*fl;
	freq_scaler=(freq_high-freq_low)/2;
}

void c_autowah::set_freq_high(float *fh){
	freq_high=*fh;
	freq_scaler=(freq_high-freq_low)/2;
}

void c_autowah::set_rate(float *r){
	rate=*r;
	a_step=(rate*2*PI)/(float)FS;

	//Update rate for the filter
	float update_rate;
	//Set update rate
	update_rate=*r*update_rate_scaler;
	n_update_reset=(unsigned) ((float)FS/update_rate);
}

void c_autowah::reset_buffer(void){

	//Current angle of LFO
	a_lfo=0;
}

float c_autowah::process(float x){

	update_wah_frequency();
	return wahfilter.process(input_gain*x);
}


float c_autowah::lfo(void){

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

void c_autowah::update_wah_frequency(void){

	float sin_x;

	sin_x=lfo();

	if(i_update>=n_update_reset){
		//Update filter frequency
		frequency=freq_low+(sin_x+1)*freq_scaler;
		wahfilter.set_freq(frequency);
		i_update=0;
	}else{
		i_update++;
	}
}
