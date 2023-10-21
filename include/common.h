/*
 * Created on Thu Oct 19 2023
 *
 * Filename: common.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 * 
 * Description: Including parameters, global variables, util functions 
 * and frequently used header files.
 */

#pragma once

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <random>
#include <iostream>
#include "parameters.h"

using std::vector;

struct Point2D {
    double x;
    double y;
};
extern struct Point2D user_location[N_USER], bs_location[N_BS];

extern double path_loss[N_USER][N_BS];
extern double channelgains_matrix[N_USER][N_BS];

/*math functions*/

double uniform_real(double a, double b);
double rician_fading(double K);
vector<double> calculate_transmition_rates(double w, vector<double> h, vector<double> p);

/*util functions for evironment*/

void global_initialize();
void channelgains_update();
