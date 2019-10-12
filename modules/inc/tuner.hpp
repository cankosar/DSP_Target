/*
 * overdrive.hpp
 *
 *  Created on: 29.09.2019
 *      Author: Can
 */

#ifndef INC_MODULES_TUNER_HPP_
#define INC_MODULES_TUNER_HPP_

class c_tuner{

	public:
		//Methods
		void init(void);
		void start(void);
		void stop(void);
		void process(float x);

		//Variables
		bool status;

		//Current estimated frequency
		float fc;

	private:
		//Methods
		void detect_pitch(void);
		float calc_ac(unsigned dist);
		void find_peaks(void);
		unsigned get_exact_peak_linear(unsigned dist);
		void estimate_freq(void);
		void reset_buffer(void);

		//Variables
		unsigned long tptr;
		static const unsigned bufsize=4800;
		float buffer[bufsize];

		//Predefined note bins
		static const unsigned s_bins=46;
		unsigned note_bins[s_bins]={1745,1647,1555,1467,1385,1307,1234,1164,1099,1037,979,924,872,823,777,733,692,653,617,582,549,518,489,462,436,411,388,366,346,326,308,291,274,259,244,231,218,205,194,183,173,163,154,145,137,129};
		float arr_ac[s_bins];

		//Number of full waves for Auto correlation
		unsigned n_waves_ac=8;

		//Peaks
		unsigned p,p1,p2;

		//Moving average filter
		unsigned maptr;
		static const unsigned l_maf=4;
		float buf_maf[l_maf]; 		//Moving average filter
		double devtol= 1.02930223664;	//Deviation tolerance 50 cents


		//Predictive filter
		float last_conf_f;
		unsigned long last_conf_t;
		unsigned long conf_timeout=500;		//Timeout in ms

};



#endif /* INC_MODULES_OVERDRIVE_HPP_ */
