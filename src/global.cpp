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
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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

    current_time = 0;
}

/**
 * @brief Initialize server state and set first failure time.
*/
void server_initialize()
{
    // printf("N_SLOT: %d\n", N_SLOT);
    // printf("TTR: %f\n", TTR);
    for(int i = 0; i < N_BS; i++)
        server_available[i] = true,
        server_recover_time[i] = 0,
        server_next_error[i] = N_SLOT * TTR + TTR;
        // printf("server_available[%d]: %s\n", i, server_available[i] ? "true" : "false"),
        // printf("server_recover_time[%d]: %d\n", i, server_recover_time[i]),
        // printf("server_next_error[%d]: %f\n", i, server_next_error[i]);

    // set first failure time of each server
    for(int i = 0; i < N_BS; i++)
        server_next_error[i] = exponential(1.0/F_INTERVAL);
        // printf("server_next_error[%d]: %f\n", i, server_next_error[i]);
}

/**
 * @brief Initialize server state and set failure time.
*/
std::string filenames[N_BS] = {
        "D:/File/Projects/edge-computing-simulation/data/new_bs_1_label.csv",
        "D:/File/Projects/edge-computing-simulation/data/new_bs_2_label.csv",
        "D:/File/Projects/edge-computing-simulation/data/new_bs_3_label.csv",
        "D:/File/Projects/edge-computing-simulation/data/new_bs_4_label.csv",
        "D:/File/Projects/edge-computing-simulation/data/new_bs_5_label.csv",
        "D:/File/Projects/edge-computing-simulation/data/new_bs_6_label.csv"
    };

void new_server_initialize() {
    for (int i = 0; i < N_BS; i++) {
        server_available[i] = true;
        server_recover_time[i] = 0;
        server_next_error[i] = -1; 
    }

    for (int i = 0; i < N_BS; i++) {
        std::ifstream file(filenames[i]);
        std::string line;
        std::cout << "Attempting to open file: " << filenames[i] << std::endl;

        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filenames[i] << std::endl;
            continue;
        }

        int found_one_index = -1; 
        int line_index = 0; 

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            int column_index = 0; 

            while (std::getline(ss, value, ',')) {
                int val = std::stoi(value); 
                if (val == 1) {
                    found_one_index = line_index; 
                    break; 
                }
                column_index++;
            }
            if (found_one_index != -1) {
                break; 
            }
            line_index++;
        }
        printf("found_one_index:%d\n", found_one_index);
        server_next_error[i] = found_one_index  * TTR + TTR; 
        file.close();
    }
    for(int i = 0; i < N_BS; i++)
        printf("server_available[%d]: %s\n", i, server_available[i] ? "true" : "false"),
        printf("server_recover_time[%d]: %d\n", i, server_recover_time[i]),
        printf("server_next_error[%d]: %f\n", i, server_next_error[i]);
}