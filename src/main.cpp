/*
 * Created on Thu Oct 19 2023
 *
 * Filename: main.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 */

#include "common.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

extern void perform_test();

int main()
{
    perform_test();
    return 0;

    global_initialize();
    channelgains_update();

    vector<double> x(N_USER), y(N_USER);
    for(int i = 0; i < N_USER; i++)
        x[i] = user_location[i].x, y[i] = user_location[i].y;
    plt::scatter(x, y, 5, {{"c","r"}});
    x.resize(N_BS), y.resize(N_BS);
    for(int i = 0; i < N_BS; i++)
        x[i] = bs_location[i].x, y[i] = bs_location[i].y;
    plt::scatter(x, y, 5, {{"c","b"}});
    plt::show();

    return 0;
}