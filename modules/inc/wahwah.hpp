/*
 * chorus.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_WAHWAH_HPP_
#define INC_MODULES_WAHWAH_HPP_

#include "biquad.hpp"

class c_wahwah{

	public:

		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);

		void set_input_gain(float *g);
		void set_quality(float *q);
		void set_freq_low(float *fl);
		void set_freq_high(float *fh);
		void set_depth(float *d);
		void set_width(float *w);
		void set_position(uint16_t *pos);

		//Variables
		bool status;

	private:

		//Methods
		void update_scaler(void);

		//Autowah parameter with initializers
		float Q;			//Quality
		float frequency;	//Current frequency
		float depth; 		//Gain of peak filter
//		float width; 		//Width

		//Update
		unsigned int n_update_reset;
		unsigned int i_update;

		float freq_low;		//Lower bound of frequency
		float freq_high;	//Upper bound of frequency
		float freq_scaler;	//Intermediate var for calculating current freq.

		float input_gain;

		//Calibration parameters
		float gain_base=15;
		float gain_scaler=0.25;
		float update_rate_scaler=50;	//update n times during a sinus wave

		//Initial parameters
		float initial_input_gain=0.3;
		float initial_quality=1.2; 			//Wet gain (Dry/Wet mix)
		float initial_depth=50; 		//Depth
		float initial_position=0;
		float initial_freq_low=400;
		float initial_freq_high=2800;
		float freq_mid=1600;			//Middle frequency for width
		float width_range=1300;

		float position;
		float position_max=4096;
		float pos_scaler=1/position_max;

		c_biquad wahfilter;

};



#endif /* INC_MODULES_CHORUS_HPP_ */
