/*
 * overdrive.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_OVERDRIVE_HPP_
#define INC_MODULES_OVERDRIVE_HPP_

#include "biquad.hpp"

class c_overdrive{

	public:
		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);
		void set_gain(float *g);
		void set_HP_freq(float *f);
		void set_LP_freq(float *f);
		void init_biquad_filters(void);

		//Variables
		bool status;

	private:
		//Variables
		float downscaler;
		float upscaler=1000;
		float gain;

		//Initial parameters
		float initial_gain=10;
		float initial_HP_freq=300;
		float initial_LP_freq=3500;



		c_biquad pre_filter;
		c_biquad post_filter;

};



#endif /* INC_MODULES_OVERDRIVE_HPP_ */
