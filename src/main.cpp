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
#include <cstdint> 
#include <ctime>  

namespace plt = matplotlibcpp;

extern std::pair<std::vector<Experiment *>, std::vector<ExpSummary>> main_loop(vector<Experiment *> (*exp_generator)(), int repeat); // mainloop.cpp
// extern void perform_test(); // test.cpp

int main()
{
    // perform_test();
    // return 0;

    auto exp_generator = []() -> vector<Experiment *> {
        vector<Experiment *> exps = {
            new RandomExperiment(),

            new GreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 1),
            new GreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 1),
            new GreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 1),

            new GreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 2),
            new GreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 2),
            new GreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 2),

            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 0.05),
            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 0.05),
            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 0.05),

            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 0.1),
            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 0.1),
            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 0.1),

            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 0.2),
            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 0.2),
            new EpsGreedyExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 0.2),

            // new MixExperiment1(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION),
            // new MixExperiment1(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION),
            // new MixExperiment1(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X),

            // new AdaptiveExperiment(),
            // new AdaptiveExperiment2(),

            new EGAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION),
            new EGAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION),
            new EGAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X),

            new GBAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION),
            new GBAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION),
            new GBAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X),

            new TD1sAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 1),
            new TD1sAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 1),
            new TD1sAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 1),

            new TD2sAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 1),
            new TD2sAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 1),
            new TD2sAdaptiveExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 1),

            new TDAvgLinkStatusExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, 1),
            new TDAvgLinkStatusExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, 1),
            new TDAvgLinkStatusExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, 1),

            new Bandit2sExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, "eps-greedy"),
            new Bandit2sExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, "eps-greedy"),
            new Bandit2sExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, "eps-greedy"),

            new Bandit2sExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION, "gradient"),
            new Bandit2sExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION, "gradient"),
            new Bandit2sExperiment(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X, "gradient"),

            // new MixExperiment2(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION),
            // new MixExperiment2(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION),
            // new MixExperiment2(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X),

            // new MixExperiment3(SentientAgent::SERVER_CHOSING_METHOD::BY_ESTIMATION),
            // new MixExperiment3(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION),
            // new MixExperiment3(SentientAgent::SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X),
        };
        return exps;
    };
 
    // vector<Experiment *> exps = main_loop(exp_generator, REPEAT_NUMBER);
    auto exps = main_loop(exp_generator, REPEAT_NUMBER);
    vector<Experiment *> exps_1 = exps.first;
    vector<ExpSummary> summary = exps.second;

    // fprintf(exps);
 
    report(RESULT_PATH, exps_1, summary);

    // for(Experiment *p: exps)
    //     delete p;

    return 0;
}