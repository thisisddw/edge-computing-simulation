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

/*flags*/

// #define DEBUG           // could make considerable difference in performance
// #define FAILURE_ON      // comment out this line to disable server failure
// #define FAILURE_DETAIL  // whether or not to create a table of each failure event in report
// #define PLOT_AGENT      // set this flag to include detail chart of agents in report

/*environment settings*/

#define N_USER 4        // number of users
#define N_BS 6          // number of BS
#define RANGE 20        // users and BSs are scattered in [0,RANGE]*[0,RANGE]

#define RICIAN_K 3.981  // K factor of Rician channel    10^0.6
#define N_0 1e-13       // noise
#define W_SUB 0.1e6     // bandwidth

/*agent settings*/

#define JOB_PATH ""     // data path
#define P_MAX 0.1       // max transmission power
#define N_LINK 3        // max number of established links of an agent

/*server settings*/

#define F_INTERVAL 20   // average interval between failures
#define F_DURATION 1.0  // the failure lasts F_DURATION seconds

/*simulation settings*/

#define RANDOM_SEED 2011421         // set to (time(NULL)) to induce undeterministic behavior
#define N_SLOT (60000*3)            // number of time slots
#define TTR 1e-3                    // time of one transmit frame
#define RESULT_PATH "./result/"     // path to store any result files

/* only for test purpose */

#define K_CPU 1.9e3
#define WEIGHT_E 6e2        // parameter for utility
#define WEIGHT_F 2e-6       // parameter for utility
#define N_GROUP 2           // number of BS in a group
#define MAX_ITER (N_USER*8) // number of iteration in full-CSI algorithm
