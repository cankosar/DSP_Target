/*
 * chorus.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_FLANGER_HPP_
#define INC_MODULES_FLANGER_HPP_
#include "../../dsp/inc/constants.hpp"

class c_flanger{

	public:

		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);
		float process_nondelay(float x);

		void set_wet(float *w);
		void set_depth(float *d);
		void set_rate(float *r);
		void set_feedback(float *fb);

		//Variables
		bool status;

	private:

		//Methods
		void reset_buffer(void);
		float get_current_delay(void);
		float lfo(void);

		//Chorus parameter with initializers
		float G_d; 			//Dry gain (Dry/Wet mix)
		float G_w; 			//Wet gain (Dry/Wet mix)
		float G_fb;			//Feedback gain
		float depth; 		//Depth
		float rate;			//Rate

		//Config parameters


		//Initial parameters
		float initial_wet=50; 			//Wet gain (Dry/Wet mix)
		float initial_depth=50; 		//Depth
		float initial_rate=2;
		float initial_feedback=50;


		//Chorus buffer
		static const unsigned flanger_len=1300;
		float* fbuf = new float[flanger_len];
		unsigned fptr;

		//Dry delay
		static const unsigned d_base=10;		//Base delay in ms
		static const unsigned l_drybuf=d_base*FSms;
		float* drybuf = new float[l_drybuf];
		unsigned dryptr;

		//LFO
		float a_step;
		float a_lfo;

};



#endif /* INC_MODULES_CHORUS_HPP_ */
