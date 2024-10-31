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
    AdaptiveExperiment(int server_chosing_method) : BaseExperiment("test-AdaptiveAgent-basic-functionality")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method);
        tracker_setup();
    }
};

class AdaptiveExperiment2 : public BaseExperiment {
public:
    AdaptiveExperiment2(int server_chosing_method) : BaseExperiment("test-AdaptiveAgent-CyclicController")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method, AdaptiveAgent::config(N_LINK, 0.1, new CyclicController(N_LINK)));
        tracker_setup();
    }
};

class EGAdaptiveExperiment : public BaseExperiment {
public:
    EGAdaptiveExperiment(int server_chosing_method) : BaseExperiment("AdaptiveAgent-EpsGreedyMB")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        set_name("AdaptiveAgent-EpsGreedyMB_" + std::string(server_chosing_method_str));
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method, AdaptiveAgent::config(N_LINK, 0.1, new EpsGreedyMB(N_LINK, 0.1, 1e8 * 0.1)));
        tracker_setup();
    }
};

class GBAdaptiveExperiment : public BaseExperiment {
public:
    GBAdaptiveExperiment(int server_chosing_method) : BaseExperiment("AdaptiveAgent-GradientBandit")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        set_name("AdaptiveAgent-GradientBandit_" + std::string(server_chosing_method_str));
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method, AdaptiveAgent::config(N_LINK, 0.1, new GradientBandit(N_LINK, 1, 0.1)));
        tracker_setup();
    }
};

class TD1sAdaptiveExperiment : public BaseExperiment {
public:
    TD1sAdaptiveExperiment(int server_chosing_method, int n = 1) : BaseExperiment("")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        char buf[100];
        sprintf(buf, "AdaptiveAgent-single_state-TD-%dstep_", n);
        set_name(buf + std::string(server_chosing_method_str));
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new TemporalDifference(N_LINK, 1, 0, 0.1, 1e8 * 0.1 * 10, 0.1, 0.9, n),
                    new AdaptiveAgent::SingleState())
                );
        tracker_setup();
    }
};

class TD2sAdaptiveExperiment : public BaseExperiment {
public:
    TD2sAdaptiveExperiment(int server_chosing_method, int n = 1) : BaseExperiment("")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        char buf[100];
        sprintf(buf, "AdaptiveAgent-double_state-TD-%dstep_", n);
        set_name(buf + std::string(server_chosing_method_str));
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new TemporalDifference(N_LINK, 2, 0, 0.1, 1e8 * 0.1 * 10, 0.1, 0.9, n),
                    new AdaptiveAgent::DoubleState())
                );
        tracker_setup();
    }
};

class TDAvgLinkStatusExperiment : public BaseExperiment {
public:
    TDAvgLinkStatusExperiment(int server_chosing_method, int n = 1) : BaseExperiment("")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        char buf[100];
        sprintf(buf, "AdaptiveAgent-avg_link_status-TD-%dstep_", n);
        set_name(buf + std::string(server_chosing_method_str));
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new TemporalDifference(N_LINK, 3, 0, 0.1, 1e8 * 0.1 * 10, 0.1, 0.9, n),
                    new AdaptiveAgent::AvgLinkStatus3State(0.01))
                );
        tracker_setup();
    }
};

class Bandit2sExperiment : public BaseExperiment {
public:
    Bandit2sExperiment(int server_chosing_method, const char *method = "eps-greedy") : BaseExperiment("")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        char buf[100];
        sprintf(buf, "AdaptiveAgent-double_state-%s_", method);
        set_name(buf + std::string(server_chosing_method_str));
        for(int i = 0; i < N_USER; i++)
            agents[i] = new AdaptiveAgent(i, server_chosing_method, 
                    AdaptiveAgent::config(N_LINK, 0.1, 
                        new MultiStateBandit(N_LINK, 2, method, 0, 0.1, 0.1, 1e8 * 0.1, 0.1),
                    new AdaptiveAgent::DoubleState())
                );
        tracker_setup();
    }
};

class MixExperiment2 : public BaseExperiment {
public:
    MixExperiment2(int server_chosing_method) : BaseExperiment("Mix2-013greedy-2EGMB")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        set_name("Mix2-013greedy-2EGMB_" + std::string(server_chosing_method_str));
        assert(N_USER == 4);
        agents[0] = new GreedyAgent(0, server_chosing_method);
        agents[1] = new GreedyAgent(1, server_chosing_method);
        agents[2] = new AdaptiveAgent(2, server_chosing_method, AdaptiveAgent::config(N_LINK, 0.1, new EpsGreedyMB(N_LINK, 0.1, 1e8 * 0.1)));
        agents[3] = new GreedyAgent(3, server_chosing_method);
        tracker_setup();
    }
};

class MixExperiment3 : public BaseExperiment {
public:
    MixExperiment3(int server_chosing_method) : BaseExperiment("Mix2-013greedy-2GB")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        set_name("Mix2-013greedy-2GB_" + std::string(server_chosing_method_str));
        assert(N_USER == 4);
        agents[0] = new GreedyAgent(0, server_chosing_method);
        agents[1] = new GreedyAgent(1, server_chosing_method);
        agents[2] = new AdaptiveAgent(2, server_chosing_method, AdaptiveAgent::config(N_LINK, 0.1, new GradientBandit(N_LINK)));
        agents[3] = new GreedyAgent(3, server_chosing_method);
        tracker_setup();
    }
};