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
#define FAILURE_ON      // comment out this line to disable server failure
// #define FAILURE_DETAIL  // whether or not to create a table of each failure event in report
// #define PLOT_AGENT      // set this flag to include detail chart of agents in report

/*environment settings*/

#define N_USER 2        // number of users
#define N_BS 4          // number of BS
#define RANGE 20        // users and BSs are scattered in [0,RANGE]*[0,RANGE]

#define CHANNEL_CHOOSING  "Rician"  //channel fast fading between each pair of edge node and UE follows Rician or Rayleigh distribution 
#define RICIAN_K 3.981  // K factor of Rician channel    10^0.6
#define Rayleigh_sigma 3  // sigma factor of Rayleigh channel
#define M_PI 3.14159265358979323846
#define N_0 1e-13       // noise
#define W_SUB 0.1e6     // bandwidth

/*agent settings*/

#define JOB_PATH ""     // data path
#define P_MAX 0.1       // max transmission power
#define N_LINK 6        // max number of established links of an agent

/*server settings*/

#define F_INTERVAL 20   // average interval between failures
#define F_DURATION 1.0  // the failure lasts F_DURATION seconds
#define F_INTERVAL_a 3
#define F_INTERVAL_b 17  // F_INTERVAL is in [F_INTERVAL_a, F_INTERVAL_b]

/*job settings*/
#define PP_ALPHA 1.16  //set to 1.16 to follow pareto principle 80/20 [1, 1.16, 2]

/*simulation settings*/

#define RANDOM_SEED 2011421         // set to (time(NULL)) to induce undeterministic behavior [42, 23659]
#define N_SLOT (100*60)            // number of time slots
#define TTR 1e-2                    // time of one transmit frame
#define RESULT_PATH "./result/"     // path to store any result files
#define REPEAT_NUMBER 50       //number of repeat 

/* only for test purpose */

#define K_CPU 1.9e3
#define WEIGHT_E 6e2        // parameter for utility
#define WEIGHT_F 2e-6       // parameter for utility
#define N_GROUP 2           // number of BS in a group
#define MAX_ITER (N_USER*8) // number of iteration in full-CSI algorithm
