/*
 * delay.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_DELAY_HPP_
#define INC_MODULES_DELAY_HPP_



class c_delay{

	public:
		//Methods
		void init(void);
		void set_wet(float* wet);
		void set_dry(float* dry);
		void set_time(float* time);
		void set_feedback(float* fb);
		void start(void);
		void stop(void);
		float process(float x);

		//Variables
		bool status=0;

	private:
		//Methods
		void reset_buffer(void);

		//Parameters with default values
		float G_fb; 		//Feedback gain
		float G_d; 			//Dry gain (Dry/Wet mix)
		float G_w; 			//Wet gain (Dry/Wet mix)
		float delay_time; 	//Delay time in seconds

		//Buffers
//		static const unsigned delay_len=100000;

		unsigned long dptr;
		unsigned long n_distance;

		//Default parameters
		float initial_fb=30;
		float initial_dry=70;
		float initial_wet=30;
		float initial_time=100;
};



#endif /* INC_MODULES_DELAY_HPP_ */
