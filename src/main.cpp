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
#include "experiments/greedyexp.h"
#include "experiments/adaptiveexp.h"
#include "experiments/report.h"

namespace plt = matplotlibcpp;

extern void main_loop(vector<Experiment *> experiments);    // mainloop.cpp
extern void perform_test();                                 // test.cpp

int main()
{
    // perform_test();
    // return 0;

    vector<Experiment *> exps = {
        // new RandomExperiment(),
        new GreedyExperiment(),
        // new AdaptiveExperiment(),
        new AdaptiveExperiment2(),
        // new EpsGreedyExperiment(0.05),
        // new EpsGreedyExperiment(0.1),
        // new EpsGreedyExperiment(0.2),
        // new MixExperiment1(),
    };

    main_loop(exps);

    // for(Experiment *p: exps)
    //     ((BaseExperiment *)p)->plot_save_agents(RESULT_PATH);
    
    report(RESULT_PATH, exps);

    return 0;
}