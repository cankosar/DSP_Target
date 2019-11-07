/*
 * dsp.cpp
 *
 *  Created on: 10.10.2019
 *      Author: Can
 */


#ifdef __cplusplus
extern "C" {
#endif


#include "../inc/dsp.hpp"

void c_dsp::init(void){

	//Initialize Biquad filters

	//Lowshelf filter
	lowshelf.init();
	lowshelf.set_filter_type(3);

	//Low mid filter
	lowmid.init();
	lowmid.set_filter_type(2);

	//Low mid filter
	lowmid.init();
	lowmid.set_filter_type(2);

	//Low mid filter
	mid.init();
	mid.set_filter_type(2);

	//High shelf filter
	highshelf.init();
	highshelf.set_filter_type(4);


	//Initialize tuner
	tuner.init();


	//Initialize delay
	delay.init();

	//Initialize chorus
	chorus.init();

	//Initialize overdrive
	overdrive.init();

	//Initialize reverb
	reverb.init();


	//Initialize tremolo
	tremolo.init();

	//Initialize rotary
	rotary.init();

	//Initialize rotary
	compressor.init();

	//Initialize flanger
	flanger.init();

	//Initialize autowah
	autowah.init();

	//Initialize wahwah
	wahwah.init();

	//Set status
	status=1;

}

int32_t c_dsp::process(int32_t *x){


//Convert to float
	float y=(float) *x;

	//Main routine: Check the status of the components and feed them
	if(status){

		//Pass through compressor
		if(compressor.status){
			y=compressor.process(y);
		}

		//Pass through the EQ section
		if(lowshelf.status){
			y=lowshelf.process(y);
		}

		if(lowmid.status){
			y=lowmid.process(y);
		}

		if(mid.status){
			y=mid.process(y);
		}

		if(highmid.status){
			y=highmid.process(y);
		}

		if(highshelf.status){
			y=highshelf.process(y);
		}


		//Pass through delay
		if(delay.status){
			y=delay.process(y);
		}

		//Pass through chorus
		if(chorus.status){

			y=chorus.process(y);
		}

		//Pass through flanger
		if(flanger.status){

			y=flanger.process(y);
		}

		//Pass through rotary
		if(rotary.status){
			y=rotary.process(y);
		}

		//Pass through tremolo
		if(tremolo.status){
			y=tremolo.process(y);
		}

		//Pass through the overdrive
		if(overdrive.status){
			y=overdrive.process(y);	//Overdrive
		}

		//Pass through autowah
		if(autowah.status){

			y=autowah.process(y);
		}

		//Pass through wahwah
		if(wahwah.status){

			y=wahwah.process(y);
		}

		//Pass through reverb
		if(reverb.status){
			y=reverb.process(y);
		}
	}
	else if(tuner.status){
		tuner.process(y);
	}

	//Convert to int and return
	return (int32_t) y;

//	return *x;
}

void c_dsp::stop(void){


	//This routine reset all the DSP functionalities

	//Reset status
	status=0;

	//Stop EQ
	lowshelf.stop();
	lowmid.stop();
	mid.stop();
	highmid.stop();
	highshelf.stop();

	//Reset delay
	delay.stop();

	//Reset chorus
	chorus.stop();

	//Reset overdrive
	overdrive.stop();

	//Reset reverb
	reverb.stop();

	//Reset tremolo
	tremolo.stop();

	//Reset rotary
	rotary.stop();

	//Reset compressor
	compressor.stop();

	//Reset flanger
	flanger.stop();

	//Reset autowah
	autowah.stop();

	//Reset wahwah
	wahwah.stop();

	//Reset tuner
//	tuner.stop();

}

void c_dsp::start(void){
	status=1;
}


void c_dsp::update_bank_states(uint32_t banks){

//	printf("U:%ld\n",banks);

	//Tuner bank
	if(banks&(1<<bankid_tuner)){
		//Stop DSP if it still runs
		if(status){
			stop();
		}

		//Start tuner if it's not running
		if(!tuner.status){
			tuner.start();
		}

	}else{

		//General DSP bank
		if(banks&(1<<bankid_general)){

			//Stop tuner if it's running
			if(tuner.status){
				tuner.stop();
			}

			//Start DSP if it's not running
			if(!status){
				start();
			}
		}else{

			//Stop DSP if it's not running
			if(status){
				stop();
			}
		}

	}

	//Lowshelf bank
	if(banks&(1<<(bankid_lowshelf))){
		lowshelf.start();
	}else{
		lowshelf.stop();
	}

	//Lowmid bank
	if(banks&(1<<(bankid_lowmid))){
		lowmid.start();
	}else{
		lowmid.stop();
	}

	//Mid bank
	if(banks&(1<<(bankid_mid))){
		mid.start();
	}else{
		mid.stop();
	}

	//High mid bank
	if(banks&(1<<(bankid_highmid))){
		highmid.start();
	}else{
		highmid.stop();
	}

	//High shelf bank
	if(banks&(1<<(bankid_highshelf))){
		highshelf.start();
	}else{
		highshelf.stop();
	}

	//Delay bank
	if(banks&(1<<(bankid_delay))){
		delay.start();
	}else{
		delay.stop();
	}

	//Chorus bank
	if(banks&(1<<(bankid_chorus))){
		chorus.start();
	}else{
		chorus.stop();
	}

	//Overdrive bank
	if(banks&(1<<(bankid_overdrive))){
		overdrive.start();
	}else{
		overdrive.stop();
	}

	//Reverb bank
	if(banks&(1<<(bankid_reverb))){
		reverb.start();
	}else{
		reverb.stop();
	}

	//Tremolo bank
	if(banks&(1<<(bankid_tremolo))){
		tremolo.start();
	}else{
		tremolo.stop();
	}

	//Rotary bank
	if(banks&(1<<(bankid_rotary))){
		rotary.start();
	}else{
		rotary.stop();
	}

	//Compressor bank
	if(banks&(1<<(bankid_comp))){
		compressor.start();
	}else{
		compressor.stop();
	}

	//Flanger bank
	if(banks&(1<<(bankid_flanger))){
		flanger.start();
	}else{
		flanger.stop();
	}

	//Compressor bank
	if(banks&(1<<(bankid_autowah))){
		autowah.start();
	}else{
		autowah.stop();
	}

	//Wahwah bank
	if(banks&(1<<(bankid_wahwah))){
		wahwah.start();
	}else{
		wahwah.stop();
	}
}



#ifdef __cplusplus
}
#endif
