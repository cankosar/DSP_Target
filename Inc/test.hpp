/*
 * test.hpp
 *
 *  Created on: 19.09.2019
 *      Author: Can
 */

#ifndef TEST_HPP_
#define TEST_HPP_



class construct {
public:
    int a, b;

    // Default Constructor
    construct()
    {
        a = 10;
        b = 20;
    };

    void increment(void);

};

class dyna{

	public:
		static const unsigned a_size=2;
		//uint32_t* buf = new uint32_t[a_size];
		uint32_t buf[a_size];
		void init_array(void);

};

#endif /* TEST_HPP_ */
