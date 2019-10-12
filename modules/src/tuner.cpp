/*
 * overdrive.cpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */
//#include "stdio.h"
#include "math.h"
#include "string.h"
#include "../inc/tuner.hpp"
#include "../../dsp/inc/constants.hpp"
#include "stm32h7xx_hal.h"


void c_tuner::init(void){

	//Reset buffer
	reset_buffer();

	//Set status
	status=1;

}

void c_tuner::start(void){

	//Set status
	status=1;
}


void c_tuner::stop(void){

	//Set status
	status=0;
	//Reset buffer
	reset_buffer();




}

void c_tuner::reset_buffer(void){

	//Reset the main buffer
	memset(buffer, 0, bufsize*sizeof(*buffer));

	//Reset moving average buffer
	memset(buf_maf, 0, l_maf*sizeof(*buf_maf));

	//Reset the pointer
	tptr=0;
	maptr=0;

	last_conf_f=0;
	last_conf_t=0;


}

void c_tuner::process(float x){

//	printf("tptr:%d\n",tptr);

	if(tptr<bufsize){
		//Fill until the half buffer is full
		buffer[tptr]=x;
		//Increment pointer
		tptr++;

	}else{
		//If one half full =>Process the buffer
		tptr=0;
		detect_pitch();
	}

}


void c_tuner::detect_pitch(void){

	//Declare and initialize variables
	unsigned long i=0;

	//Auto-correlation algorithm for the first iteration
	for(i=0; i<s_bins;i++){
		//Calculate auto-correlation normalized score
		arr_ac[i]=calc_ac(note_bins[i]);
	}

	//Find the major peaks of the first iteration
	find_peaks();

	if(note_bins[p1]>note_bins[p2]){
		p=get_exact_peak_linear(p1);
	}else{
		p=get_exact_peak_linear(p2);
	}

	//Filter output and estimate frequency
	estimate_freq();
}


float c_tuner::calc_ac(unsigned dist){

	unsigned i=0;
	float ac_score;

	ac_score=0;
	unsigned l_acbuf;
	unsigned l_max;

	//Length of samples we need
	l_acbuf=n_waves_ac*dist;
	l_max=bufsize-dist;

	if(l_acbuf>l_max){
		l_acbuf=l_max;
	}

	for(i=0;i<l_acbuf;i++){
		ac_score+=buffer[i]*buffer[i+dist];
	}

	//Norming AC Score: Divide by the count of the samples
	ac_score=ac_score/l_acbuf;

	return ac_score;
}

void c_tuner::find_peaks(void){

	p1=0;
	p2=0;


	unsigned n_peak;	//Peak count
	unsigned i;
	//Finding the peaks
	n_peak=0;											//Number of found peaks
		for(i=1; i<s_bins-1;i++){
			if(arr_ac[i]>0){								//Find only positive peaks
				if(arr_ac[i]>arr_ac[i-1] && arr_ac[i]>arr_ac[i+1]){	//Peak found
//					printf("Peak found:%f \t%f\n",(float)(FS/note_bins[i]),arr_ac[i]);
					n_peak++;								//Number of found peaks
					if(n_peak==1){							//Check if the found peak is the first peak
						p1=i;							//Initialize first peak (distance)
					}else if(n_peak==2){					//Check if the found peak is the second peak
						p2=i;							//Initialize second peak (distance)
					}

					if(arr_ac[p1]<arr_ac[i]){			//If the current peak distance is greater than the last peak distance
						p2=p1;						//Update the peak distances: Shift the next greatest to the second place
						p1=i;							//Set the greatest
					}else if(arr_ac[p2]<arr_ac[i]){		//If the current peak distance is greater than the second peak distance
						p2=i;							//Update the second peak
					}
				}
			}
		}

}

unsigned c_tuner::get_exact_peak_linear(unsigned index){


	unsigned dist=note_bins[index];


	float ac_prev=calc_ac(dist-1);
	float ac=arr_ac[index];
	float ac_next=calc_ac(dist+1);

	unsigned n=0;		//Loop counter
	unsigned n_exit=100;	//Emergency exit

	//Get gradient
	while(n<n_exit){
		n++;

		if(ac>=ac_prev){

			if(ac<ac_next){
				//Gradient positive
				dist++;
				ac_prev=ac;
				ac=ac_next;
				ac_next=calc_ac(dist+1);

			}else if(ac>=ac_next){
				//Found peak
				break;
			}
		}else if(ac<ac_prev){
			//Gradient negative
			if(ac>=ac_next){
				dist--;
				ac_next=ac;
				ac=ac_prev;
				ac_prev=calc_ac(dist-1);
			}else{
				//Negative peak
				dist=0;
				break;
			}

		}

	}
	return dist;
}


/*This function uses methods of stochastics to estimate the frequency*/
void c_tuner::estimate_freq(void){

	float ma;					//Value of moving average
	float minimum, maximum;		//Minimum and maximum values of one array
	double deviation;			//Current deviation in the array
	float fin;



	fin=(float)FS/(float)p;		//Input frequency

	//Moving average filter

	//Insert value
	buf_maf[maptr]=fin;

	//Update pointer
	maptr++;

	if(maptr>=l_maf){
		maptr=0;
	}

	//Calculate moving average

	ma=0;
	minimum=buf_maf[0];
	maximum=buf_maf[0];

	unsigned i=0;
	for(i=0;i<l_maf;i++){
		//Update moving average
		ma+=buf_maf[i];

		//Find the minimum value of array
		if(buf_maf[i]<minimum){
			minimum=buf_maf[i];
		}

		//Find the minimum value of array
		if(buf_maf[i]>maximum){
			maximum=buf_maf[i];
		}
	}

	//Moving average value
	ma=ma/l_maf;

	//Deviation
	deviation=maximum/minimum;

	//Check if deviation is within the tolerance band
	if(deviation<=devtol){		//Note confidence good=>Everything fine
		last_conf_f=ma;

		last_conf_t=HAL_GetTick(); //Commented out for Win
		fc=ma;

	}else{												//Note confidence bad => Set a timer
		if((HAL_GetTick()-last_conf_t)<conf_timeout){ //Comment out for Win
//		if(1){
			fc=last_conf_f;

		}else{
			//Timeout
			last_conf_f=0;
			last_conf_t=0;
			fc=0;
		}

	}

//	printf("Fc=%.2f\n",fc);

}
