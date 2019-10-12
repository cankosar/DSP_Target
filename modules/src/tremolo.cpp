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
#include "../inc/tremolo.hpp"


void c_tremolo::init(void){

	//Tremolo parameter
	set_depth(&initial_depth);
	set_freq(&initial_freq);
	set_type(&initial_type);

	//Current angle of LFO
	i_lfo=0;

}


void c_tremolo::start(void){

	status=1;

}

void c_tremolo::stop(void){

	status=0;

	//Current angle of LFO
	i_lfo=0;
}

void c_tremolo::set_depth(float* d){

	depth=*d*0.01;

}

void c_tremolo::set_freq(float* f){

	frequency=*f;
	update_step();
}

void c_tremolo::set_type(float *t){

	type=(unsigned)*t;

}

void c_tremolo::update_step(void){

	//Calculate algorithm parameters
	t_step=frequency/FS;

	//Angle step
	a_step=frequency*(2*PI)/FS;
}



float c_tremolo::process(float x){

	return x*get_current_attenuation();
}



float c_tremolo::get_current_attenuation(void){

	return 1-depth*lfo();

}


float c_tremolo::lfo(void){

	float y_lfo;

	//Calculate LFO value

	switch(type){
	//Sinus
	case 0:
		y_lfo=0.5*(1+sin(i_lfo));
		//Update angle
		i_lfo+=a_step;
		if(i_lfo>=2*PI){
			i_lfo-=2*PI;
		}
		break;

	//Triangle
	case 1:

		y_lfo = fabs(fmod(i_lfo, 2) -1 );
		i_lfo+=t_step;

		break;
	case 2:		//Square
		y_lfo=fmod(i_lfo,2) < 1 ? 1 : 0;
		i_lfo+=t_step;
		break;

	}

//	printf("%.3f\n",y_lfo);

	return y_lfo;
}
