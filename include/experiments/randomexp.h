/*
 * Created on Mon Oct 23 2023
 *
 * Filename: randomexp.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Simulate a group of random agents.
 */

#pragma once

#include "agents/randomagent.h"
#include "experiments/baseexp.h"

class RandomExperiment : public BaseExperiment {
public:
    RandomExperiment() : BaseExperiment("random") 
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new RandomAgent(i),
            tracker[i].set_agent(agents[i]);
    }
};