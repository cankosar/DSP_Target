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
	unsigned short int i;
	for(i=0;i<n_EQ;i++){
		//Initialize
		biquad[i].init();
	}

//	biquad[0].apply_filter(0,1,2000,1);

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

	//Set status
	status=1;

}

void c_dsp::test(uint32_t id){

	if((id-last_id)!=2){
//		printf("Id: %d, Last:%d\n",id,last_id);
	}

	last_id=id;
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
		unsigned short i;
		for(i=0;i<n_EQ;i++){
			if(biquad[i].status){

				y=biquad[i].process(&y);
			}
		}

		//Pass through delay
		if(delay.status){
			y=delay.process(y);
		}

		//Pass through chorus
		if(chorus.status){

			y=chorus.process(y);
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

	//Reset biquads
	unsigned int i;
	for(i=0;i<n_EQ;i++){
		biquad[i].stop();
	}

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

	//Reset tuner
	tuner.stop();

}

void c_dsp::start(void){
	status=1;
}


void c_dsp::update(void){

	/*Here comes the update hash */
	//Dummy hash
//	unsigned banks=0b111111111101;			//Full performance test
//	unsigned banks=0b111101011101;			//wo delay and od
	unsigned banks=0b000000100001;			//Pass through

	//General DSP bank
	if(banks&(1<<c_dsp_bank)){
		start();
	}else{
		stop();
	}

	//Tuner bank
	if(banks&(1<<c_tuner_bank)){
		//Stop DSP
		stop();
		tuner.start();

	}else{
		tuner.stop();
		//Start DSP
		start();
	}

	//EQ banks
	unsigned i;
	for(i=0;i<n_EQ;i++){
		//EQ banks
		if(banks&(1<<(i+c_EQ_bank))){
			biquad[i].start();
		}else{
			biquad[i].stop();
		}
	}

	//Delay bank
	if(banks&(1<<(c_delay_bank))){
		delay.start();
	}else{
		delay.stop();
	}

	//Chorus bank
	if(banks&(1<<(c_chorus_bank))){
		chorus.start();
	}else{
		chorus.stop();
	}

	//Overdrive bank
	if(banks&(1<<(c_overdrive_bank))){
		overdrive.start();
	}else{
		overdrive.stop();
	}

	//Reverb bank
	if(banks&(1<<(c_reverb_bank))){
		reverb.start();
	}else{
		reverb.stop();
	}

	//Tremolo bank
	if(banks&(1<<(c_tremolo_bank))){
		tremolo.start();
	}else{
		tremolo.stop();
	}

	//Rotary bank
	if(banks&(1<<(c_rotary_bank))){
		rotary.start();
	}else{
		rotary.stop();
	}

	//Compressor bank
	if(banks&(1<<(c_compressor_bank))){
		compressor.start();
	}else{
		compressor.stop();
	}
}



#ifdef __cplusplus
}
#endif
