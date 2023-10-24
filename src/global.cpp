/*
 * Created on Thu Oct 19 2023
 *
 * Filename: global.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Define and initialize global variables.
 */


#include "parameters.h"
#include "job.h"
#include <math.h>

#define SQ(x) (x)*(x)
extern double uniform_real(double a, double b);

struct Point2D {
    double x;
    double y;
};
Point2D user_location[N_USER], bs_location[N_BS];
JobLoader job_loader(JOB_PATH);
bool server_available[N_BS];
double server_recover_time[N_BS], server_next_error[N_BS];

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

    // in case only one failure happens
    server_next_error[F_ID] = F_TIME;

    current_time = 0;
}
