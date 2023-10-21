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
 * @brief Update channelgains_matrix according to path loss and rician fading.
*/
void channelgains_update()
{
    for(int i = 0; i < N_USER; i++)
        for(int j = 0; j < N_BS; j++)
            channelgains_matrix[i][j] = path_loss[i][j] * pow(10, rician_fading(RICIAN_K)/10);
}

/**
 * @brief Calculate transmission rate of each user towards a particular edge node.
 * @param w brandwith
 * @param h channel gain vector of users
 * @param p transmission power of each user
*/
vector<double> calculate_transmition_rates(double w, vector<double> h, vector<double> p)
{
    assert(h.size() == p.size());

    int n = h.size();
    double tmp = 0;
    for(int i = 0; i < n; i++)
        tmp += h[i] * p[i];
    vector<double> rate(n);
    for(int i = 0; i < n; i++)
        rate[i] = w * log2(1 + (h[i] * p[i]) / (tmp - h[i] * p[i] + N_0));
    return rate;
}