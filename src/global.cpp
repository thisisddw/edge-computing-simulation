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
#include <common.h>
#include <nlohmann/json.hpp>

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
std::vector<Server_failure_slot> server_failure_slot_data;
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
 * @brief Initialize server failure time and failure prediction results.
*/

void server_failure_initialize() {
    const std::string filename = "D:/File/Projects/edge-computing-simulation/data/server_fault_information.json";
    std::ifstream file(filename);
    nlohmann::json j;
    file >> j;

    for (const auto& item : j) {
        Server_failure_slot group_data;
        group_data.server_id = item["server_id"].get<int>();
        group_data.fault_time_slots = item["fault_time_slots"].get<std::vector<int>>();
        group_data.cat_pred_fault_time_slots = item["cat_pred_fault_time_slots"].get<std::vector<int>>();
        group_data.xgb_pred_fault_time_slots = item["xgb_pred_fault_time_slots"].get<std::vector<int>>();
        group_data.lgb_pred_fault_time_slots = item["lgb_pred_fault_time_slots"].get<std::vector<int>>();

        server_failure_slot_data.push_back(group_data);
    }

    // for (const auto& group_data : server_failure_slot_data) {
    //     std::cout << "Server ID: " << group_data.server_id << "\n";
    //     std::cout << "Fault Time Slots: ";
    //     for (const auto& id : group_data.fault_time_slots) {
    //         std::cout << id << " ";
    //     }
    //     std::cout << "\nCat Pred Fault Time Slots: ";
    //     for (const auto& id : group_data.cat_pred_fault_time_slots) {
    //         std::cout << id << " ";
    //     }
    //     std::cout << "\nXGB Pred Fault Time Slots: ";
    //     for (const auto& id : group_data.xgb_pred_fault_time_slots) {
    //         std::cout << id << " ";
    //     }
    //     std::cout << "\nLGB Pred Fault Time Slots: ";
    //     for (const auto& id : group_data.lgb_pred_fault_time_slots) {
    //         std::cout << id << " ";
    //     }
    //     std::cout << "\n\n";
    // }
}

/**
 * @brief Initialize server state and set failure time.
*/

void new_server_initialize()
{
    for(int i = 0; i < N_BS; i++)
        server_available[i] = true,
        server_recover_time[i] = 0,
        server_next_error[i] = N_SLOT * TTR + TTR;

    // set first failure time of each server
    for (int i = 0; i < N_BS; i++) {
        if (i < static_cast<int>(server_failure_slot_data.size())) {
            const auto& fault_time_slots = server_failure_slot_data[i].fault_time_slots;

            if (!fault_time_slots.empty()) {
                server_next_error[i] = static_cast<int>(*std::min_element(fault_time_slots.begin(), fault_time_slots.end()) * TTR);
            } else {
                server_next_error[i] = N_SLOT * TTR + TTR;
            }
        } else {
            server_next_error[i] = N_SLOT * TTR + TTR;
        }
    }
}