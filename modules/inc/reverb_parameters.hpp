/*
 * reverb_parameters.hpp
 *
 *  Created on: 30.09.2019
 *      Author: KOS2ST
 */

#ifndef INC_MODULES_REVERB_PARAMETERS_HPP_
#define INC_MODULES_REVERB_PARAMETERS_HPP_


//Allpass
static const unsigned n_allpass=4u;

const int sbuf_ap_1	= 556;
const int sbuf_ap_2	= 441;
const int sbuf_ap_3	= 341;
const int sbuf_ap_4	= 225;

//Comb
static const unsigned n_comb=8u;

const unsigned sbuf_comb_1		= 1116;
const unsigned sbuf_comb_2		= 1188;
const unsigned sbuf_comb_3		= 1277;
const unsigned sbuf_comb_4		= 1356;
const unsigned sbuf_comb_5		= 1422;
const unsigned sbuf_comb_6		= 1491;
const unsigned sbuf_comb_7		= 1557;
const unsigned sbuf_comb_8		= 1617;

#endif /* INC_MODULES_REVERB_PARAMETERS_HPP_ */
