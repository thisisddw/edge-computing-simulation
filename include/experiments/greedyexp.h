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

class MixExperiment1 : public BaseExperiment {
public:
    MixExperiment1() : BaseExperiment("mix-013-gready-2-eps-greedy")
    {
        assert(N_USER == 4);
        agents[0] = new GreedyAgent(0);
        agents[1] = new GreedyAgent(1);
        agents[2] = new GreedyAgent(2, 1);
        agents[3] = new GreedyAgent(3);

        tracker_setup();
    }
};