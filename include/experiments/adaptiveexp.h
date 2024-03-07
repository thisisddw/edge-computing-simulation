/*
 * Created on Thu Mar 07 2024
 *
 * Filename: adaptiveexp.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Experements for adaptive agents with RL controller. 
 */

#pragma once

#include "agents/adaptiveagent.h"
#include "experiments/baseexp.h"

class AdaptiveExperiment : public BaseExperiment {
public:
    AdaptiveExperiment() : BaseExperiment("test-AdaptiveAgent-basic-functionality")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i);
        tracker_setup();
    }
};

class AdaptiveExperiment2 : public BaseExperiment {
public:
    AdaptiveExperiment2() : BaseExperiment("test-AdaptiveAgent-CyclicController")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, AdaptiveAgent::config(N_LINK, 10, new CyclicController(N_LINK)));
        tracker_setup();
    }
};