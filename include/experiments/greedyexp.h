/*
 * Created on Fri Jan 05 2024
 *
 * Filename: greedyexp.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: 
 */

#pragma once

#include "agents/greedyagent.h"
#include "experiments/baseexp.h"

class GreedyExperiment : public BaseExperiment {
public:
    GreedyExperiment() : BaseExperiment("greedy")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new GreedyAgent(i);
        tracker_setup();
    }
};

class EpsGreedyExperiment : public BaseExperiment {
public:
    EpsGreedyExperiment(double eps = 0.1) : BaseExperiment("eps-greedy")
    {
        char tmp[100];
        sprintf(tmp, "eps-greedy-%.2lf", eps);
        set_name(tmp);

        for(int i = 0; i < N_USER; i++)
            agents[i] = new EpsGreedyAgent(i, eps);
        tracker_setup();
    }
};