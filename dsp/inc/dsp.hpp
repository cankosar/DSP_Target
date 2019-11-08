/*
 * dsp.hpp
 *
 *  Created on: 10.10.2019
 *      Author: Can
 */

#ifndef INC_DSP_HPP_
#define INC_DSP_HPP_


#include "stm32h7xx_hal.h"

#include "../../../common/common.hpp"

#include "../../modules/inc/tuner.hpp"
#include "../../modules/inc/biquad.hpp"
#include "../../modules/inc/delay.hpp"
#include "../../modules/inc/chorus.hpp"
#include "../../modules/inc/overdrive.hpp"
#include "../../modules/inc/reverb.hpp"
#include "../../modules/inc/tremolo.hpp"
#include "../../modules/inc/rotary.hpp"
#include "../../modules/inc/compressor.hpp"
#include "../../modules/inc/flanger.hpp"
#include "../../modules/inc/autowah.hpp"
#include "../../modules/inc/wahwah.hpp"
#include "../../modules/inc/volume.hpp"

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

		c_biquad lowshelf;
		c_biquad lowmid;
		c_biquad mid;
		c_biquad highmid;
		c_biquad highshelf;

		c_delay delay;
		c_chorus chorus;
		c_overdrive overdrive;
		c_reverb reverb;
		c_tuner tuner;
		c_tremolo tremolo;
		c_rotary rotary;
		c_compressor compressor;
		c_flanger flanger;
		c_autowah autowah;
		c_wahwah wahwah;
		c_volume volume;

//	private:
};


#endif /* INC_DSP_HPP_ */
