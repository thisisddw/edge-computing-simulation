/*
 * Created on Thu Mar 07 2024
 *
 * Filename: multiarmed_bandit.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Implement the multi-armed bandit problem model
 */

#pragma once

#include "agents/rl_controller/rl_controller.h"

class MultiarmedBandit : public SingleStateRlController {
private:
    int last_action;
protected:
    const double lr;
    double *Q;

    /**
     * @brief Descendant classes must call this function in return statements of act().
    */
    int make_action(int a)
    {
        return last_action = a;
    }

public:
    MultiarmedBandit(int action_space_size, double q_init_val = 1e8, double learning_rate = 0.1)
     : SingleStateRlController(action_space_size), lr(learning_rate)
    {
        last_action = -1;
        Q = new double[n_action];
        for(int i = 0; i < n_action; i++)
            Q[i] = q_init_val;
    }
    virtual ~MultiarmedBandit() { delete[] Q; }

    virtual void feedback(double reward) override
    {
        if(last_action == -1) return;
        Q[last_action] = Q[last_action] + lr * (reward - Q[last_action]);
    }
};

class EpsGreedyMB : public MultiarmedBandit {
    const double eps;
public:
    EpsGreedyMB(int action_space_size, double eps = 0.1, double q_init_val = 1e8, double learning_rate = 0.1)
     : MultiarmedBandit(action_space_size, q_init_val, learning_rate) , eps(eps) {}

    int act() override
    {
        if(uniform_real(0, 1) < eps)
            return make_action(rand() % n_action);
        int a = 0;
        for(int i = 0; i < n_action; i++)
            if(Q[i] > Q[a])
                a = i;
        return make_action(a);
    }
};