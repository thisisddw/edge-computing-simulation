/*
 * Created on Fri Oct 20 2023
 *
 * Filename: mainloop.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Perform experiments in the main loop.
 */

#include "common.h"
#include "experiment.h"
#include "progressbar.h"
#include "experiments/baseexp.h"

/**
 * @brief Perform experiments with settings in parameter.h.
 * @param experiments Experiment instances to be performed.
 * @param repeat If repeat > 1, every experiment would be perform that times,
 *  and returned exps would be created with BaseExperiment::average. That means,
 *  if repeat > 1, every input exps should be a BaseExperiment.
*/
vector<Experiment *> main_loop(vector<Experiment *> (*exp_generator)(), int repeat)
{
    assert(repeat >= 1);

    global_initialize();

    ProgressBar pb(50);
    vector<vector<Experiment *>> exp_histroy;

    for (int j = 0; j < repeat; j++)
    {
        vector<Experiment *> experiments = exp_generator();
        server_initialize();
        for(int i = 0; i < N_SLOT; i++)
        {
            current_time = i * TTR;
            channelgains_update();
        #ifdef FAILURE_ON
            server_state_update();
        #endif
            for(auto e: experiments)
                e->step();
            pb.update((double)(i + j*N_SLOT) / (N_SLOT * repeat));
        }
        exp_histroy.push_back(experiments);
    }
    printf("\n");

    if (repeat == 1)
        return exp_histroy[0];

    vector<Experiment *> ret;
    for (unsigned int i = 0; i < exp_histroy[0].size(); i++)
    {
        vector<BaseExperiment *> samples;
        for (auto &it: exp_histroy)
            samples.push_back((BaseExperiment *)(it[i]));
        ret.push_back(BaseExperiment::average(samples));
    }
    return ret;
}
