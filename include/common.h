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
#include <utility>
#include <algorithm>
#include "basictypes.h"
#include "parameters.h"
#include "job.h"

#ifdef DEBUG
    #define IF_DEBUG(x) x
#else
    #define IF_DEBUG(x)
#endif

extern struct Point2D user_location[N_USER], bs_location[N_BS];
extern JobLoader job_loader;

extern bool server_available[N_BS];
extern double server_recover_time[N_BS], server_next_error[N_BS];
extern vector<FailureRecord> server_failure_histroy;

extern double path_loss[N_USER][N_BS];
extern double channelgains_matrix[N_USER][N_BS];

extern double current_time;

/*math functions*/

double uniform_real(double a, double b);
double rician_fading(double K);
double exponential(double lambda);
vector<std::pair<double, double>> calculate_transmition_rates(double w, vector<double> h, vector<double> p);

/*util functions for evironment*/

void global_initialize();
void server_initialize();
void channelgains_update();
void server_state_update();