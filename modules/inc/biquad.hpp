/*
 * biquad.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_BIQUAD_HPP_
#define INC_MODULES_BIQUAD_HPP_

class c_biquad{

	public:
		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float);
		void set_param(float *b0, float *b1, float *b2, float *a0, float *a1);
		void update_param(void);
		void set_filter_type(short unsigned t);
		void set_gain(float g);
		void set_freq(float f);
		void set_quality(float q);

		//Variables
		bool status;

	private:

		//Methods
		void reset_buffer(void);

		//Buffer
		float y,y1,y2,x1,x2;

		//Filter build parameters
		unsigned short type;
		float gain;
		float f0;
		float Q;

		//Parameters with default initializers
		float param[5];

		unsigned short initial_type=1;
		float initial_gain=0;
		float initial_freq=0;
		float initial_quality=1;

};


#endif /* INC_MODULES_BIQUAD_HPP_ */
