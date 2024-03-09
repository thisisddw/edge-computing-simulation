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
    const double lr, clip;
    double avg_r, var_r;
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

    int cnt_clip, cnt_upd;
    double avg_g, avg_dif;

public:
    GradientBandit(int action_space_size, double learning_rate = 1, double clip = 0.1)
     : SingleStateRlController(action_space_size), lr(learning_rate), clip(clip)
    {
        last_action = -1;
        h = new double[n_action];
        for(int i = 0; i < n_action; i++)
            h[i] = 0;
        p = new double[n_action];

        cnt_clip = cnt_upd = 0;
        avg_g = 0;
    }
    ~GradientBandit() { delete[] h; delete[] p; 
        // printf("\rGradientBandit: cnt_clip %d cnt_upd %d avg_g %lf avg_dif %lf var_r %lf \n",
        //  cnt_clip, cnt_upd / n_action, avg_g, avg_dif, var_r);
    }

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
            avg_r = reward, var_r = 1, avg_dif = 0;
        else
        {
            var_r += 0.1 * ((reward - avg_r) * (reward - avg_r) - var_r);
            avg_r += 0.1 * (reward - avg_r);    // the ratio 0.1 may be changed
        }
  
        double dif = (reward - avg_r) / sqrt(var_r);
        vector<double> g(n_action, 0);

        compute_p();
  
        for (int i = 0; i < n_action; i++)
        {
            if (i == last_action)
                g[i] = lr * dif * (1 - p[i]);
            else
                g[i] = -lr * dif * p[i];

            avg_g += (g[i] - avg_g) / (++cnt_upd);
            avg_dif += (abs(dif) - avg_dif) / cnt_upd;
        }

        double max_g = 0;
        for (int i = 0; i < n_action; i++)
            max_g = std::max(max_g, abs(g[i]));
        double t = std::min(1., clip / max_g); if (t < 1) cnt_clip++;
        for (int i = 0; i < n_action; i++)
            h[i] += g[i] * t;
    }
};