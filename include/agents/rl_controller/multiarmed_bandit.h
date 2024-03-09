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

class QbasedMultiarmedBandit : public SingleStateRlController {
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
    QbasedMultiarmedBandit(int action_space_size, double q_init_val = 1e8, double learning_rate = 0.1)
     : SingleStateRlController(action_space_size), lr(learning_rate)
    {
        last_action = -1;
        Q = new double[n_action];
        for(int i = 0; i < n_action; i++)
            Q[i] = q_init_val;
    }
    virtual ~QbasedMultiarmedBandit() { delete[] Q; }

    virtual void feedback(double reward) override
    {
        if(last_action == -1) return;
        Q[last_action] = Q[last_action] + lr * (reward - Q[last_action]);
    }
};

class EpsGreedyMB : public QbasedMultiarmedBandit {
    const double eps;
public:
    EpsGreedyMB(int action_space_size, double eps = 0.1, double q_init_val = 1e8, double learning_rate = 0.1)
     : QbasedMultiarmedBandit(action_space_size, q_init_val, learning_rate) , eps(eps) {}

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

class GradientBandit final : public SingleStateRlController {
    const double lr;
    double avg_r;
    double *h, *p;
    int last_action;

    int make_action(int a) { return last_action = a; }
    void compute_p()
    {
        double sum = 0;
        for(int i = 0; i < n_action; i++)
            p[i] = exp(h[i]), sum += p[i];
        for(int i = 0; i < n_action; i++)
            p[i] /= sum;
    }

public:
    GradientBandit(int action_space_size, double learning_rate = 0.1)
     : SingleStateRlController(action_space_size), lr(learning_rate)
    {
        last_action = -1;
        h = new double[n_action];
        for(int i = 0; i < n_action; i++)
            h[i] = 0;
        p = new double[n_action];
    }
    ~GradientBandit() { delete[] h; delete[] p; }

    int act() override
    {
        compute_p();
        for(int i = 1; i < n_action; i++)
            p[i] += p[i - 1];
        double r = uniform_real(0, 1);
        for(int i = 0; i < n_action; i++)
            if (r < p[i])
                return make_action(i);
        return make_action(n_action - 1);
    }
    void feedback(double reward) override
    {
        if (last_action == -1)
            avg_r = reward;
        else
            avg_r += 0.1 * (reward - avg_r);    // the ratio 0.1 may be changed
  
        compute_p();
  
        for (int i = 0; i < n_action; i++)
        {
            if (i == last_action)
                h[i] += lr * (reward - avg_r) * (1 - p[i]);
            else
                h[i] -= lr * (reward - avg_r) * p[i];
        }
    }
};