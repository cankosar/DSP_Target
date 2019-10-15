/*
 * dsp.hpp
 *
 *  Created on: 10.10.2019
 *      Author: Can
 */

#ifndef INC_DSP_HPP_
#define INC_DSP_HPP_


#include "stm32h7xx_hal.h"

#include "../../modules/inc/tuner.hpp"
#include "../../modules/inc/biquad.hpp"
#include "../../modules/inc/delay.hpp"
#include "../../modules/inc/chorus.hpp"
#include "../../modules/inc/overdrive.hpp"
#include "../../modules/inc/reverb.hpp"
#include "../../modules/inc/tremolo.hpp"
#include "../../modules/inc/rotary.hpp"
#include "../../modules/inc/compressor.hpp"

//The bank positions
static const unsigned short c_dsp_bank=0;
static const unsigned short c_tuner_bank=1;
static const unsigned short c_EQ_bank=2;
static const unsigned short c_delay_bank=5;
static const unsigned short c_chorus_bank=6;
static const unsigned short c_overdrive_bank=7;
static const unsigned short c_reverb_bank=8;
static const unsigned short c_tremolo_bank=9;
static const unsigned short c_rotary_bank=10;
static const unsigned short c_compressor_bank=11;


class c_dsp{
	public:
		//Methods
		void init(void);
		int32_t process(int32_t* x);
		void start(void);
		void stop(void);
		void update_bank_states(uint32_t banks);

		uint32_t last_id=0;

		//Variables
		bool status;




		const static unsigned short int n_EQ=3;
		c_biquad biquad[n_EQ];
		c_delay delay;
		c_chorus chorus;
		c_overdrive overdrive;
		c_reverb reverb;
		c_tuner tuner;
		c_tremolo tremolo;
		c_rotary rotary;
		c_compressor compressor;

//	private:
};


#endif /* INC_DSP_HPP_ */
