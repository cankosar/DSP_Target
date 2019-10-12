/*
 * chorus.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_ROTARY_HPP_
#define INC_MODULES_ROTARY_HPP_

class c_rotary{

	public:

		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);
		void set_depth(float* d);
		void set_freq(float* f);

		//Variables
		bool status;

	private:

		//Methods
		void reset_buffer(void);
		float get_current_delay(void);
		float lfo(void);
		void update_step(void);

		//Chorus parameter
		float depth; 		//Depth
		float freq;			//Rate

		//Chorus buffer
		static const unsigned rotary_len=1200;
		float* rbuf = new float[rotary_len];
		unsigned rptr;

		//LFO
		float a_step;
		float a_lfo;

		//Config parameters
		const float d_base=5;

		//Initial values
		float initial_depth=50;
		float initial_freq=5;
};



#endif /* INC_MODULES_ROTARY_HPP_ */
