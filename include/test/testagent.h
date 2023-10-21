/*
 * Created on Fri Oct 20 2023
 *
 * Filename: testagent.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: TestAgent is base class for agents in test scenerio,
 * generating action_space for derivative classes.
 */

#pragma once

#include "common.h"
#include "agent.h"

class TestAgent : public Agent
{
public:
    TestAgent() : Agent()
    {
        if(!initialized)
        {
            action_space_init();
            initialized = true;
            printf("TestAgent initialized.\n");
        }
    }

protected:
    static vector<Action> action_space;

    /**
     * @brief Calculate utilities for a joint strategy. I copied large pieces of
     * code from experiment.h and testexperiment.h, causing severe redundancy. But
     * this should be acceptable for such minor test code.
    */
    static vector<double> calc_utilities(vector<int> joint_strategy)
    {
        assert(joint_strategy.size() == N_USER);

        Action actions[N_USER];
        Feedback feedbacks[N_BS];
        
        for(int i = 0; i < N_USER; i++)
            actions[i] = action_space[joint_strategy[i]];
        
        // copy from experiment.h

        for(int i = 0; i < N_BS; i++)
        {
            vector<double> h(N_USER), p(N_USER);
            for(int j = 0; j < N_USER; j++)
                h[j] = channelgains_matrix[j][i],
                p[j] = actions[j].p[i];
            vector<double> r = calculate_transmition_rates(W_SUB, h, p);
            assert(r.size() == N_USER);
            for(int j = 0; j < N_USER; j++)
                feedbacks[j].r[i] = r[j];
        }

        // copy from testexperiment.h

        vector<double> u(N_USER);
        for(int i = 0; i < N_USER; i++)
        {
            double r = 0;   // total rate
            double p = 0;   // total power
            for(int j = 0; j < N_BS; j++)
                r += feedbacks[i].r[j],
                p += actions[i].p[j];
            double e = TTR * p; // energy
            double c = K_CPU * TTR * r; // cycle

            u[i] = WEIGHT_F*c - WEIGHT_E*e;
        }
        return u;
    }

private:
    static bool initialized;

    static double power_level[3];
    static double dfs_state[N_BS], sump, n_bs;

    static void action_space_dfs(int pos)
    {
        if(pos >= N_BS)
        {
            Action a;
            for(int i = 0; i < N_BS; i++) a.p[i] = dfs_state[i];
            action_space.push_back(a);
            return;
        }
        dfs_state[pos] = 0;
        action_space_dfs(pos + 1);
        if(n_bs == N_GROUP)
            return;
        n_bs++;
        for(int i = 0; i < 3; i++)
        {
            if(sump + power_level[i] <= 0.1)
            {
                sump += power_level[i];
                dfs_state[pos] = power_level[i];
                action_space_dfs(pos + 1);
                sump -= power_level[i];
            }
        }
        n_bs--;
    }

    static void action_space_init()
    {
        action_space.clear();
        memset(dfs_state, 0, sizeof(dfs_state));
        power_level[0] = 0.033;
        power_level[1] = 0.067;
        power_level[2] = 0.1;
        sump = 0, n_bs = 0;

        action_space_dfs(0);

        // for(auto a: action_space)
        // {
        //     for(int i = 0; i < N_BS; i++)
        //         printf("%.4lf ", a.p[i]);
        //     printf("\n");
        // }
    }
};