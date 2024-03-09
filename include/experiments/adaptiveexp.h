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
#include "agents/rl_controller/multiarmed_bandit.h"
#include "agents/greedyagent.h"
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
            agents[i] = new AdaptiveAgent(i, AdaptiveAgent::config(N_LINK, 0.1, new CyclicController(N_LINK)));
        tracker_setup();
    }
};

class EGAdaptiveExperiment : public BaseExperiment {
public:
    EGAdaptiveExperiment() : BaseExperiment("AdaptiveAgent-EpsGreedyMB")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, AdaptiveAgent::config(N_LINK, 0.1, new EpsGreedyMB(N_LINK, 0.1, 1e8 * 0.1)));
        tracker_setup();
    }
};

class GBAdaptiveExperiment : public BaseExperiment {
public:
    GBAdaptiveExperiment() : BaseExperiment("AdaptiveAgent-GradientBandit")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, AdaptiveAgent::config(N_LINK, 0.1, new GradientBandit(N_LINK)));
        tracker_setup();
    }
};

class MixExperiment2 : public BaseExperiment {
public:
    MixExperiment2() : BaseExperiment("Mix2-013greedy-2EGMB")
    {
        assert(N_USER == 4);
        agents[0] = new GreedyAgent(0);
        agents[1] = new GreedyAgent(1);
        agents[2] = new AdaptiveAgent(2, AdaptiveAgent::config(N_LINK, 0.1, new EpsGreedyMB(N_LINK, 0.1, 1e8 * 0.1)));
        agents[3] = new GreedyAgent(3);
        tracker_setup();
    }
};

class MixExperiment3 : public BaseExperiment {
public:
    MixExperiment3() : BaseExperiment("Mix2-013greedy-2GB")
    {
        assert(N_USER == 4);
        agents[0] = new GreedyAgent(0);
        agents[1] = new GreedyAgent(1);
        agents[2] = new AdaptiveAgent(2, AdaptiveAgent::config(N_LINK, 0.1, new GradientBandit(N_LINK)));
        agents[3] = new GreedyAgent(3);
        tracker_setup();
    }
};