/*
 * Created on Thu Oct 19 2023
 *
 * Filename: global.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Define and initialize global variables.
 */


#include <math.h>
#include <vector>
#include "basictypes.h"
#include "parameters.h"
#include "job.h"

#define SQ(x) (x)*(x)
extern double uniform_real(double a, double b);
extern double exponential(double lambda);

Point2D user_location[N_USER], bs_location[N_BS];
JobLoader job_loader(JOB_PATH);

bool server_available[N_BS];
double server_recover_time[N_BS], server_next_error[N_BS];
std::vector<FailureRecord> server_failure_histroy;

double path_loss[N_USER][N_BS];
double channelgains_matrix[N_USER][N_BS];

double current_time;

/**
 * @brief Randomly generate locations for users and BSs.
*/
void global_initialize()
{
    srand(RANDOM_SEED);

    for(int i = 0; i < N_USER; i++)
        user_location[i].x = uniform_real(0, 1) * RANGE,
        user_location[i].y = uniform_real(0, 1) * RANGE;
    for(int i = 0; i < N_BS; i++)
        bs_location[i].x = uniform_real(0, 1) * RANGE,
        bs_location[i].y = uniform_real(0, 1) * RANGE;
    for(int i = 0; i < N_USER; i++)
        for(int j = 0; j < N_BS; j++)
            path_loss[i][j] = pow(sqrt(SQ(user_location[i].x - bs_location[j].x)
                + SQ(user_location[i].y - bs_location[j].y)), -3.5);

    for(int i = 0; i < N_BS; i++)
        server_available[i] = true,
        server_recover_time[i] = 0,
        server_next_error[i] = N_SLOT * TTR + TTR;

    // set first failure time of each server
    for(int i = 0; i < N_BS; i++)
        server_next_error[i] = exponential(1.0/F_INTERVAL);

    current_time = 0;
}
