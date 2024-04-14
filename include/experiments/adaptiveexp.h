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
#include "agents/rl_controller/tabular.h"
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
            agents[i] = new AdaptiveAgent(i, AdaptiveAgent::config(N_LINK, 0.1, new GradientBandit(N_LINK, 1, 0.1)));
        tracker_setup();
    }
};

class TD1sAdaptiveExperiment : public BaseExperiment {
public:
    TD1sAdaptiveExperiment(int n = 1) : BaseExperiment("")
    {
        char buf[100];
        sprintf(buf, "AdaptiveAgent-single_state-TD-%dstep", n);
        set_name(buf);
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new TemporalDifference(N_LINK, 1, 0, 0.1, 1e8 * 0.1 * 10, 0.1, 0.9, n),
                    new AdaptiveAgent::SingleState())
                );
        tracker_setup();
    }
};

class TD2sAdaptiveExperiment : public BaseExperiment {
public:
    TD2sAdaptiveExperiment(int n = 1) : BaseExperiment("")
    {
        char buf[100];
        sprintf(buf, "AdaptiveAgent-double_state-TD-%dstep", n);
        set_name(buf);
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new TemporalDifference(N_LINK, 2, 0, 0.1, 1e8 * 0.1 * 10, 0.1, 0.9, n),
                    new AdaptiveAgent::DoubleState())
                );
        tracker_setup();
    }
};

class TDAvgLinkStatusExperiment : public BaseExperiment {
public:
    TDAvgLinkStatusExperiment(int n = 1) : BaseExperiment("")
    {
        char buf[100];
        sprintf(buf, "AdaptiveAgent-avg_link_status-TD-%dstep", n);
        set_name(buf);
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new TemporalDifference(N_LINK, 3, 0, 0.1, 1e8 * 0.1 * 10, 0.1, 0.9, n),
                    new AdaptiveAgent::AvgLinkStatus3State(0.01))
                );
        tracker_setup();
    }
};

class Bandit2sExperiment : public BaseExperiment {
public:
    Bandit2sExperiment(const char *method = "eps-greedy") : BaseExperiment("")
    {
        char buf[100];
        sprintf(buf, "AdaptiveAgent-double_state-%s", method);
        set_name(buf);    
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new MultiStateBandit(N_LINK, 2, method, 0, 0.1, 0.1, 1e8 * 0.1, 0.1),
                    new AdaptiveAgent::DoubleState())
                );
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