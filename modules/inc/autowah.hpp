/*
 * chorus.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_AUTOWAH_HPP_
#define INC_MODULES_AUTOWAH_HPP_

#include "biquad.hpp"

class c_autowah{

	public:

		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);
		void update_wah_frequency(void);

		void set_input_gain(float *g);
		void set_quality(float *q);
		void set_freq_low(float *fl);
		void set_freq_high(float *fh);
		void set_depth(float *d);
		void set_rate(float *r);

		//Variables
		bool status;

	private:

		//Methods
		void reset_buffer(void);
		float get_current_delay(void);
		float lfo(void);

		//Autowah parameter with initializers
		float Q;			//Quality
		float frequency;	//Current frequency
		float depth; 		//Gain of peak filter
		float rate;			//Rate

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
		float initial_rate=2.5;
		float initial_freq_low=400;
		float initial_freq_high=2800;

		c_biquad wahfilter;

		//LFO
		float a_step;
		float a_lfo;

};



#endif /* INC_MODULES_CHORUS_HPP_ */
