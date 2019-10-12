/*
 * overdrive.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_REVERB_HPP_
#define INC_MODULES_REVERB_HPP_

#include "comb.hpp"
#include "allpass.hpp"
#include "reverb_parameters.hpp"

class c_reverb{

	public:
		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);

		void set_wet(float* w);
		void set_size(float* s);
		void set_damp(float* d);


		//Variables
		bool status;

	private:
		//Methods
		void reset_buffer(void);

		//Buffer arrays comb
		float bufcomb1[sbuf_comb_1];
		float bufcomb2[sbuf_comb_2];
		float bufcomb3[sbuf_comb_3];
		float bufcomb4[sbuf_comb_4];
		float bufcomb5[sbuf_comb_5];
		float bufcomb6[sbuf_comb_6];
		float bufcomb7[sbuf_comb_7];
		float bufcomb8[sbuf_comb_8];
//
//		//Buffer arrays allpass
		float bufap1[sbuf_ap_1];
		float bufap2[sbuf_ap_2];
		float bufap3[sbuf_ap_3];
		float bufap4[sbuf_ap_4];

		//Calibration values
		const float fixedgain		= 0.01f;
		const float scaledamp		= 0.4f;
		const float scaleroom		= 0.28f;
		const float offsetroom	= 0.7f;

		//Initial values
		float initial_size	= 80.0f;
		float initial_damp	= 40.0f;
		float initial_wet	= 60.0f;

		//Instances
		comb inst_comb[n_comb];
		allpass inst_allpass[n_allpass];

		//Parameter
		float drymix;
		float wetmix;;
		float roomsize;
		float damp;

};



#endif /* INC_MODULES_REVERB_HPP_ */
