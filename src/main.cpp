/*
 * Created on Thu Oct 19 2023
 *
 * Filename: main.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 */

#include "common.h"
#include "matplotlibcpp.h"
#include "experiments/randomexp.h"

namespace plt = matplotlibcpp;

extern void main_loop(vector<Experiment *> experiments);    // mainloop.cpp
extern void perform_test();                                 // test.cpp

int main()
{
    // perform_test();
    // return 0;

    vector<Experiment *> exps = {
        new RandomExperiment(),
    };

    main_loop(exps);

    for(Experiment *p: exps)
        ((BaseExperiment *)p)->plot_agents();

    return 0;
}