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

extern vector<Experiment *> main_loop(vector<Experiment *> (*exp_generator)(), int repeat); // mainloop.cpp
extern void perform_test(); // test.cpp

int main()
{
    // perform_test();
    // return 0;

    auto exp_generator = []() -> vector<Experiment *> {
        vector<Experiment *> exps = {
            // new RandomExperiment(),
            // new GreedyExperiment(1),
            // new GreedyExperiment(2),
            new GreedyExperiment(3),
            // new EpsGreedyExperiment(0.05),
            // new EpsGreedyExperiment(0.1),
            // new EpsGreedyExperiment(0.2),
            // new MixExperiment1(),
            // new AdaptiveExperiment(),
            // new AdaptiveExperiment2(),
            new EGAdaptiveExperiment(),
            new GBAdaptiveExperiment(),
            // new TD1sAdaptiveExperiment(1),
            // new TD2sAdaptiveExperiment(1),
            // new TDAvgLinkStatusExperiment(1),
            // new Bandit2sExperiment("eps-greedy"),
            // new Bandit2sExperiment("gradient"),
            // new MixExperiment2(),
            // new MixExperiment3(),
        };
        return exps;
    };

    vector<Experiment *> exps = main_loop(exp_generator, 10);
    
    report(RESULT_PATH, exps);

    // for(Experiment *p: exps)
    //     delete p;

    return 0;
}