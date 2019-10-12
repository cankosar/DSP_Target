/*
 * chorus.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_TREMOLO_HPP_
#define INC_MODULES_TREMOLO_HPP_

class c_tremolo{

	public:

		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);
		void set_depth(float* d);
		void set_freq(float* f);
		void set_type(float* t);

		//Variables
		bool status;

	private:

		//Methods
		float get_current_attenuation(void);
		float lfo(void);
		void update_step(void);
		//Rotary parameter
		float depth; 		//Depth
		float frequency;			//Rate


		//LFO
		float a_step;
		float t_step;
		float i_lfo;

		//Type of wave generator
		unsigned type;		//0:triangle 1:square 2: sinus

		//Initial vaules
		float initial_depth=100;
		float initial_freq=20;
		float initial_type=1;
};



#endif /* INC_MODULES_CHORUS_HPP_ */
