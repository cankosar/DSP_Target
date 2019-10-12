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
		void set_param(float *b0, float *b1, float *b2, float *a0, float *a1);
		float process(float *x);
		void apply_filter(unsigned short type, float g, float f0, float Q);

		//Variables
		bool status;

	private:

		//Methods
		void reset_buffer(void);

		//Buffer
		float y,y1,y2,x1,x2;

		//Parameters with default initializers
		float param[5];

		float initial_param[5]={1,0,0,0,0};

};


#endif /* INC_MODULES_BIQUAD_HPP_ */
