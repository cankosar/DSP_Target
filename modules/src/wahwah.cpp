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
#include "../inc/wahwah.hpp"


void c_wahwah::init(void){

	//Initialize parameters
	set_input_gain(&initial_input_gain);
	set_quality(&initial_quality);
	set_depth(&initial_depth);
	uint16_t d_initial_position=(uint16_t)initial_position;
	set_position(&d_initial_position);
	set_freq_low(&initial_freq_low);
	set_freq_high(&initial_freq_high);


	//Initialize wahfilter
	wahfilter.init();
	wahfilter.set_filter_type(2);
	wahfilter.set_gain(depth);
	wahfilter.set_quality(Q);

	//Set update
	i_update=0;
}

void c_wahwah::stop(void){

	//Status set
	status=0;


}

void c_wahwah::start(void){

	//Status set
	status=1;
}

void c_wahwah::set_quality(float *q){
	Q=*q;
}

void c_wahwah::set_input_gain(float *g){
	input_gain=*g;
}

void c_wahwah::set_depth(float *d){
	depth=gain_base+*d*gain_scaler;
}

void c_wahwah::set_width(float *w){
	width=*w;

	//To be done

}

void c_wahwah::set_freq_low(float *fl){

	freq_low=*fl;
	freq_scaler=(freq_high-freq_low)/2;
}

void c_wahwah::set_freq_high(float *fh){
	freq_high=*fh;
	freq_scaler=(freq_high-freq_low)/2;
}

void c_wahwah::update_scaler(void){

	frequency=freq_low+(freq_high-freq_low)*pos_scaler*position;

}
void c_wahwah::set_position(uint16_t *pos){
	position=(float)*pos;
	update_scaler();
	wahfilter.set_freq(frequency);
}


float c_wahwah::process(float x){

	return wahfilter.process(input_gain*x);
}


