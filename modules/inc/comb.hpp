/*
 * overdrive.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_COMB_HPP_
#define INC_MODULES_COMB_HPP_

class comb{

		public:
		//Methods
		void init(float* bufptr, unsigned bufsize);
		void start(void);
		void stop(void);
		void reset(void);
		float process(float x);
		void set_feedback(float* fb);
		void set_damp(float *damp);

		//Variables
		bool status;

	private:
		//Methods
		void reset_buffer(void);

		//Variables
		float filterstore;
		float damp1,damp2,feedback;

		//Buffer
		unsigned long bufsize;
		float *buffer;
		unsigned long cptr;

};



#endif /* INC_MODULES_REVERB_HPP_ */
