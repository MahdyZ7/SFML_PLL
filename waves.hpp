
#ifndef WAVES_HPP
#define WAVES_HPP
#include <SFML/Graphics.hpp>
#include <complex.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <bitset>


#define screen_width 1080
#define screen_height 500
#define screen_center_x screen_width / 2
#define screen_center_y screen_height / 2
#define screen_third_x screen_width / 3
#define screen_quart screen_width / 4
#define screen_eigth screen_width / 8
#define screen_16th screen_width / 16
#define screen_6_16th screen_width / 16 * 6
#define screen_11_16th screen_width / 16 * 11
#define screen_fourth_y screen_height / 8
#define screen_3_fourth_y screen_height * 7 / 8
#define N (int)(screen_quart)

// struct state_vector
struct state_vector
{
	float phase_offset;   // carrier phase offset
	float frequency_offset;  // carrier frequency offset

	// Set global parameters for the sine wave
	float	time1;           
	float	amplitude;        
	float phi;   // input signal's initial phase
	float phi_hat;           // PLL's initial phase	
	int noise_flag;
};


void key_press(state_vector *st);
#endif