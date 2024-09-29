/*
 * Created on Thu Oct 19 2023
 *
 * Filename: utils.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Util functions for calculation.
 */

#include <complex>
#include "common.h"
#include "message.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

static std::default_random_engine generator(RANDOM_SEED);

/**
 * @brief Generate random number following Ricean distribution with parameter K.
 * 
 * @details
 * I adopt this method from https://dsp.stackexchange.com/questions/84493/how-to-code-rician-fading-channel-gains-from-k-factor
 * A nice article for Rician fading: https://ma-mimo.ellintech.se/2020/03/02/rician-fading-a-channel-model-often-misunderstood/
*/
double rician_fading(double K)
{
    std::normal_distribution<double> distribution (0.0,1.0);
    std::complex<double> cn(distribution(generator), distribution(generator));
    auto h = sqrt(K/(K+1)) + sqrt(1/(2*(K+1)))*cn;
    return abs(h);
}

/**
 * @param a lower bound
 * @param b upper bound
*/
double uniform_real(double a, double b)
{
    std::uniform_real_distribution<double> distribution(a, b);
    return distribution(generator);
}

/**
 * @param lambda Expected value is lambda^-1
*/
double exponential(double lambda)
{
    std::exponential_distribution<double> exponentialDist(lambda);
    return exponentialDist(generator);
}

/**
 * @brief Update channelgains_matrix according to path loss and rician fading.
*/
void channelgains_update()
{
    for(int i = 0; i < N_USER; i++)
        for(int j = 0; j < N_BS; j++)
            channelgains_matrix[i][j] = path_loss[i][j] * pow(10, rician_fading(RICIAN_K)/10);
}

/**
 * @brief this function affects server_available, server_recover_time, server_next_error
*/
void server_state_update()
{
    for(int i = 0; i < N_BS; i++)
    {
        if(server_available[i] && server_next_error[i] <= current_time)
        {
            // float F_DURATION = 1.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(5.0 - 1.0)));
            server_available[i] = false;
            server_recover_time[i] = current_time + F_DURATION;
            server_next_error[i] = server_recover_time[i] + exponential(1.0/F_INTERVAL);
            server_failure_histroy.push_back({i, current_time, server_recover_time[i], server_recover_time[i] - current_time});
            IF_DEBUG(printf("\rBS %d failure: [%.3lf, %.3lf] \n", i, current_time, server_recover_time[i]);)
        }
        if(!server_available[i] && server_recover_time[i] <= current_time)
        {
            server_available[i] = true;
            server_recover_time[i] = 0;
        }
    }
}

void new_server_state_update()
{
    for(int i = 0; i < N_BS; i++)
    {
        if(server_available[i] && server_next_error[i] <= current_time)
        {
            server_available[i] = false;
            server_recover_time[i] = current_time + F_DURATION;

            std::ifstream file(filenames[i]);
            std::string line;
            // std::cout << "Attempting to open file: " << filenames[i] << std::endl;

            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << filenames[i] << std::endl;
                continue;
            }

            int found_one_index = -1;
            printf("current_time:%d\n", current_time);
            int n_slot =  current_time/TTR;
            printf("n_slot:%d\n", n_slot);
            int line_index = n_slot; 
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
            // printf("found_one_index:%d\n", found_one_index);
            file.close();

            // server_next_error[i] = server_recover_time[i] + exponential(1.0/F_INTERVAL);
            server_next_error[i] = server_recover_time[i] + (found_one_index - n_slot)  * TTR + TTR;
            server_failure_histroy.push_back({i, current_time, server_recover_time[i], server_recover_time[i] - current_time});
            IF_DEBUG(printf("\rBS %d failure: [%.3lf, %.3lf] \n", i, current_time, server_recover_time[i]);)
        }
        if(!server_available[i] && server_recover_time[i] <= current_time)
        {
            server_available[i] = true;
            server_recover_time[i] = 0;
        }
    }
}

/**
 * @brief Calculate transmission rate of each user towards a particular edge node.
 * @param w brandwith
 * @param h channel gain vector of users
 * @param p transmission power of each user
 * @return A vector of pair<double, double>, the first is transmition rate, the second
 * is aggregate information.
*/
vector<std::pair<double, double>> calculate_transmition_rates(double w, vector<double> h, vector<double> p)
{
    assert(h.size() == p.size());

    int n = h.size();
    double tmp = 0;
    for(int i = 0; i < n; i++)
        tmp += h[i] * p[i];
    vector<std::pair<double, double>> rate(n);
    for(int i = 0; i < n; i++)
        rate[i] = {w * log2(1 + (h[i] * p[i]) / (tmp - h[i] * p[i] + N_0)),
        h[i] / (tmp - h[i] * p[i] + N_0)};
    return rate;
}