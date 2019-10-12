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
#include "../inc/compressor.hpp"


void c_compressor::init(void){

	//Initialize parameter
	set_threshold(&initial_threshold);
	set_ratio(&initial_ratio);
	set_attack(&initial_attack);


	//Calculate the length of delay line and RMS buffer
	delay_len=(unsigned long) (t_main*(float)FSms);
	rms_len=(unsigned long) (t_rms*(float)FSms);

	reset_buffer();
}

void c_compressor::start(void){

	//Set status
	status=1;
}

void c_compressor::stop(void){

	//Set status
	status=0;

	//Fill the delay buffer with zeros
	reset_buffer();
}

void c_compressor::set_threshold(float* t){

	//2^15 is the max value of int16
	//Multiplicating by 0.1666 is an approximation of conversion from log2 to log10
	threshold=31+(*t*0.25);


}

void c_compressor::set_ratio(float* r){

	//Set ratio
	ratio=*r;
	slope=1-1/ratio;

	//Set help variable limit_gain
	limit_gain=1/ratio;


}

void c_compressor::set_attack(float* a){

	//Set attack
	t_attack=*a;
	t_release=t_attack*k_attack2release;

	//Update steps
//	i_attack=(unsigned long) (t_attack*(float)FSms);
//	i_release=(unsigned long) (t_release*(float)FSms);
	stepsize_attack=1/(t_attack*(float)FSms);
	stepsize_release=1/(t_release*(float)FSms);


}


void c_compressor::reset_buffer(void){
	//Fill the delay buffer with zeros
	//The size of the float is 4

	memset(rmsbuf, 0, rms_max*sizeof(*rmsbuf));
	memset(buf, 0, buf_max*sizeof(*buf));

	rms_sum=0;
	comp_gain=0;
	target_gain=0;

	//Init pointer
	rptr=0;
	ptr=0;

}

float c_compressor::process(float x){

//	x*=0.001;
	//Calculate RMS

	//Subtract oldest the value (rmsbuf[rptr]) from the sum
	rms_sum-=(rmsbuf[rptr]*rmsbuf[rptr])/(float)rms_len;



	//Add the newest value to the sum
	rms_sum+=(x*x)/rms_len;

	if(rms_sum<0){
//		printf("Warning: RMS sum negaive %f\n",rms_sum);
		rms_sum=0;
	}


	//Update the RMS Value
	c_rms=fast_sqroot(rms_sum);

//	printf("c_rms:%f\n",c_rms);
	//Get logarithmic value
	c_rms=fast_log2(c_rms);
//	c_rms=log2(c_rms);

//	printf("c_rms_lg:%f\n",c_rms);

	//Update the buffer
	rmsbuf[rptr]=x;

	//Increment
	rptr++;

	if(rptr>=rms_len){
		rptr=0;;
	}

	//Update main buf

	//Output is the last value on the buffer
	float y;
	y=buf[ptr];

	//Set the new value to the buffer
	buf[ptr]=x;

	//Increment
	ptr++;

	if(ptr>=delay_len){
		ptr=0;;
	}

	//Calculating attenuation
	float exponent;

	if(c_rms>threshold){

		//Calculating exponent according Udo Zölzer
		exponent=-1*(c_rms-threshold)*slope;
//		target_gain=pow(2,exponent);

		//Exponents smaller than -1500 are making fast algorithms unstable
		if(exponent<-100){
			target_gain=0;
		}else{
			target_gain=fastPow(2,exponent);
//			target_gain=exp_fast(natlogof2*attenuation);
//			target_gain=pow(2,exponent);

		}
	}else{
		target_gain=1;
	}

//	target_gain=1;
//	printf("TG:%f\n",target_gain);

	//Attack and release


	if(comp_gain>target_gain){

		//Attack
		comp_gain-=stepsize_attack;
//		printf("attack:%.3f\n",comp_gain);

		//Break decreasing
		if(comp_gain<=target_gain){
			comp_gain=target_gain;
		}
//		printf("Attack: Target:%.3f\t Current:%.3f\n",target_gain, comp_gain);

	}else{

		//Release
		comp_gain+=stepsize_release;

		//Break increasing
		if(comp_gain>=1){
			comp_gain=1;
		}

//		printf("Release: Target:%.3f\t Current:%.3f\n",target_gain, comp_gain);
	}





	y=y*comp_gain;

	if(comp_gain>0.1){
//		printf("Release: Target:%.3f\t Current:%.3f\n",target_gain, comp_gain);
//		printf("x:%f, y:%f, \tG=%f \t RMS: %f\n",x,y,comp_gain,c_rms);
	}

//	return x;
	return y;
}




