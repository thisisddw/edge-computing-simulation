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
    GreedyExperiment(int server_chosing_method, int n_link = 3) : BaseExperiment("greedy")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        
        set_name("greedy-" + std::to_string(n_link)  + "links" + std::string(server_chosing_method_str));
        for(int i = 0; i < N_USER; i++)
            agents[i] = new GreedyAgent(i, server_chosing_method, n_link);
        tracker_setup();
    }
    ~GreedyExperiment()
    {
        printf("\nMessage from exp %s\n", get_name());
    }
};

class EpsGreedyExperiment : public BaseExperiment {
public:
    EpsGreedyExperiment(int server_chosing_method, double eps = 0.1) : BaseExperiment("eps-greedy")
    { 
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        char tmp[100];
        sprintf(tmp, "eps-greedy-%.2lf", eps);
        set_name(tmp + std::string(server_chosing_method_str));

        for(int i = 0; i < N_USER; i++)
            agents[i] = new EpsGreedyAgent(i, server_chosing_method, eps);
        tracker_setup();
    }
};

class MixExperiment1 : public BaseExperiment {
public:
    MixExperiment1(int server_chosing_method) : BaseExperiment("mix-013-gready-2-eps-greedy")
    {
        const char* server_chosing_method_str = get_server_chosing_method_str(server_chosing_method);
        set_name("mix-013-gready-2-eps-greedy_" + std::string(server_chosing_method_str));
        assert(N_USER == 4);
        agents[0] = new GreedyAgent(0, server_chosing_method);
        agents[1] = new GreedyAgent(1, server_chosing_method);
        agents[2] = new GreedyAgent(2, server_chosing_method, 1);
        agents[3] = new GreedyAgent(3, server_chosing_method);

        tracker_setup();
    }
};