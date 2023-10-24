/*
 * Created on Fri Oct 20 2023
 *
 * Filename: experiment.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: One instance of experiment contains a particular settings
 * of agents. Multiple experiments sharing the same global parameters can 
 * be done simultaneously.
 */

#pragma once

#include "common.h"
#include "agent.h"

class Experiment {
private:
    const char *name;

protected:
    Agent *agents[N_USER];

    /*informations exchanged between agents and env in the latest slot.*/

    Action actions[N_USER];
    Feedback feedbacks[N_USER];

    /**
     * @brief Can be overrided to keep track with statistics.
    */
    virtual void track() {};

public:
    Experiment(const char *name) : name(name) {}
    virtual ~Experiment() {}

    const char *get_name() { return name; }

    /**
     * @brief Interact with agents under current global state. Should be
     * called exactly once each time slot.
    */
    void step()
    {
        for(int i = 0; i < N_USER; i++)
            actions[i] = agents[i]->act();
        for(int i = 0; i < N_BS; i++)
        {
            vector<double> h(N_USER), p(N_USER);
            for(int j = 0; j < N_USER; j++)
                h[j] = channelgains_matrix[j][i],
                p[j] = actions[j].p[i];
            vector<std::pair<double, double>> r = calculate_transmition_rates(W_SUB, h, p);
            assert(r.size() == N_USER);
            for(int j = 0; j < N_USER; j++)
                feedbacks[j].r[i] = r[j].first,
                feedbacks[j].i[i] = r[j].second,
                feedbacks[j].a[i] = server_available[i];
        }
        for(int i = 0; i < N_USER; i++)
            agents[i]->feedback(feedbacks[i]);
        
        track();
    }
};