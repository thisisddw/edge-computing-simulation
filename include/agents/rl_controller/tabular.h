/*
 * Created on Thu Mar 14 2024
 *
 * Filename: tabular.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Implementation of tabular RL methods.
 */

#pragma once

#include <list>
#include "agents/rl_controller/rl_controller.h"

/**
 * @brief This could be base class for most tabular RL methods. With act() and feedback()
 * already implemented here, derived classes should override get_state_action_target() to 
 * provide informations needed when update Q values in feedback().
*/
class QbasedTabular : public RlController {
protected:
    const double eps;
    const double lr;
    double **Q;
    int last_action;
public:
    QbasedTabular(int action_space_size, int state_space_size, int init_state = 0, 
        double eps = 0.1, double q_init_val = 0, double learning_rate = 0.1)
        : RlController(action_space_size, state_space_size, init_state), eps(eps), lr(learning_rate)
    {
        Q = new double *[n_state];
        for (int i = 0; i < n_state; i++)
            Q[i] = new double[n_action];
        for (int s = 0; s < n_state; s++)
            for (int a = 0; a < n_action; a++)
                Q[s][a] = q_init_val;
        last_action = -1;
    }
    virtual ~QbasedTabular() override
    {
        for (int i = 0; i < n_state; i++)
            delete[] Q[i];
        delete Q;
    }
    int act() override final
    {
        if (uniform_real(0, 1) < eps)
            return rand() % n_action;
        int a = 0;
        for (int i = 0; i < n_action; i++)
            if (Q[state][i] > Q[state][a])
                a = i;
        return last_action = a;
    }
    void feedback(double reward, int next_state) override final
    {
        if (last_action != -1)
            update(state, last_action, reward);
        state = next_state;

        auto sa_t = get_state_action_target();
        pair<int, int> sa = sa_t.first;
        int s = sa.first, a = sa.second;
        if (s == -1) return;
        Q[s][a] = Q[s][a] + lr * (sa_t.second - Q[s][a]);
    }

    /**
     * @brief Override this function to receive state-action-reward update.
    */
    virtual void update(int state, int action, double reward) = 0;
    /**
     * @brief Get the state and action and target to update Q. Would be invoked
     * in QbasedTabular::feedback(). If there is no state-action pair to update, 
     * returned state should be -1.
    */
    virtual pair<pair<int, int>, double> get_state_action_target() = 0;
};

class TemporalDifference final : public QbasedTabular {
    const double decay;
    const unsigned int n_step;
    std::list<pair<pair<int, int>, double>> sar_histroy;

public:
    TemporalDifference(int action_space_size, int state_space_size, int init_state = 0,
        double eps = 0.1, double q_init_val = 0, double learning_rate = 0.1, double decay = 0.9, unsigned int n_step = 1)
        : QbasedTabular(action_space_size, state_space_size, init_state, eps, q_init_val, learning_rate), 
        decay(decay), n_step(n_step) 
    { }

    void update(int state, int action, double reward) override
    {
        assert(sar_histroy.size() <= n_step + 1);
        if (sar_histroy.size() == n_step + 1)
            sar_histroy.pop_front();
        sar_histroy.push_back({{state, action}, reward});
    }

    pair<pair<int, int>, double> get_state_action_target() override
    {
        assert(sar_histroy.size() <= n_step + 1);
        if (sar_histroy.size() < n_step + 1)
            return {{-1, 0}, 0};
        double t = 0, d = 1;
        unsigned cnt = 0;
        for (auto it = sar_histroy.begin(); cnt < n_step; it++)
            t += d * it->second, d *= decay, cnt++;
        // t += d * Q[sar_histroy.back().first.first][sar_histroy.back().first.second];

        int last_state = sar_histroy.back().first.first;
        int best_a = 0;
        for (int i = 0; i < n_action; i++)
            if (Q[last_state][i] > Q[last_state][best_a])
                best_a = i;
        double tmp = 0;
        for (int i = 0; i < n_action; i++)
        {
            tmp += Q[last_state][i] * (eps / n_action);
            if (i == best_a)
                tmp += Q[last_state][i] * (1 - eps);
        }
        t += d * tmp;

        return {sar_histroy.front().first, t};
    }
};