/*
 * chorus.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_COMPRESSOR_HPP_
#define INC_MODULES_COMPRESSOR_HPP_

class c_compressor{

	public:

		//Methods
		void init(void);
		void start(void);
		void stop(void);
		float process(float x);

		void set_threshold(float* t);
		void set_ratio(float* r);
		void set_attack(float* a);

		//Variables
		bool status;

	private:

		//Methods
		void reset_buffer(void);
		inline float fast_log2 (float val){
		   int * const    exp_ptr = reinterpret_cast <int *> (&val);
		   int            x = *exp_ptr;
		   const int      log_2 = ((x >> 23) & 255) - 128;
		   x &= ~(255 << 23);
		   x += 127 << 23;
		   *exp_ptr = x;

		   val = ((-1.0f/3) * val + 2) * val - 2.0f/3;   // (1)

		   return (val + log_2);
		};

		inline float fast_sqroot(float number){

				const float threehalfs = 1.5F;

				float x2 = number * 0.5F;
				float y = number;

				// evil floating point bit level hacking
				long i = * ( long * ) &y;

				// value is pre-assumed
				i = 0x5f3759df - ( i >> 1 );
				y = * ( float * ) &i;

				// 1st iteration
				y = y * ( threehalfs - ( x2 * y * y ) );

				// 2nd iteration, this can be removed
				// y = y * ( threehalfs - ( x2 * y * y ) );


			return y*number;
		//	return 0;
		};

		inline double fastPow(double a, double b) {
			union {
				double d;
				int x[2];
			} u = { a };
			u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
			u.x[0] = 0;
			return u.d;
		};


		//Parameter
		float comp_gain;
		float target_gain;
		float ratio;
		float slope;
		float threshold;
		float t_attack;
		float t_release;

		float limit_gain;
		float stepsize_attack,stepsize_release;

		//RMS value
		float rms_sum;
		float c_rms;
		unsigned rms_len;

		//Monitor RMS
		unsigned update_rate;
		unsigned i_update;

		//Compressor buffer
		static const unsigned rms_max=1200;
//		float* rmsbuf = new float[rms_max];
		float rmsbuf[rms_max];
		unsigned rptr;

		//Calibration parameters
		const float k_attack2release=10;
		const float t_rms=24;
		const float t_main=0;

		//Initial parameters
		float initial_threshold=-10;
		float initial_ratio=4;
		float initial_attack=10;

		//Main buffer

		unsigned long delay_len;
		static const unsigned buf_max=1200; 	//25ms
//		float* buf = new float[buf_max];
		float buf[buf_max];
		unsigned ptr;
};



#endif /* INC_MODULES_COMPRESSOR_HPP_ */
