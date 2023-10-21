/*
 * Created on Thu Oct 19 2023
 *
 * Filename: parameters.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 * 
 * Description: Simulation parameters should be defined here.
 */

#pragma once

#define RANDOM_SEED 2011421//(time(NULL))

#define N_USER 6        // number of users
#define N_BS 10         // number of BS
#define RANGE 20        // users and BSs are scattered in [0,RANGE]*[0,RANGE]

#define RICIAN_K 3.981  // K factor of Rician channel    10^0.6
#define N_0 1e-13       // noise
#define W_SUB 0.1e6     // bandwidth

#define N_SLOT 200      // number of time slots
#define TTR 1e-3        // time of one transmit frame

/* only for test purpose */

#define K_CPU 1.9e3
#define WEIGHT_E 6e2    // parameter for utility
#define WEIGHT_F 2e-6   // parameter for utility
#define N_GROUP 2       // number of BS in a group
#define MAX_ITER (N_USER*8) // number of iteration in full-CSI algorithm
