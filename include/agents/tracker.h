/*
 * Created on Mon Oct 23 2023
 *
 * Filename: tracker.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Track statistics of BaseAgent.
 */

#pragma once

#include "baseagent.h"
#include "matplotlibcpp.h"

class AgentTracker {
    BaseAgent *agent;
public:
    vector<int> n_inst_done, n_inst_sending, n_inst_executing;
    vector<double> trans_rate;

    AgentTracker() { agent = NULL; }

    void set_agent(Agent *ptr)
    {
        agent = (BaseAgent *)ptr;
    }
    void track(Feedback fb)
    {
        n_inst_done.push_back(agent->job.number_instances_done());
        n_inst_sending.push_back(agent->sending.size());
        n_inst_executing.push_back(agent->pending.size());
        double r = 0;
        for(int i = 0; i < N_BS; i++)
            r += fb.r[i];
        trans_rate.push_back(r);
    }

    void plot(const char *title = "Default Title")
    {
        namespace plt = matplotlibcpp;

        plt::suptitle(title);

        plt::subplot2grid(2, 1, 0, 0);
        plt::named_plot("transmission rate", trans_rate);
        plt::legend();

        plt::subplot2grid(2, 1, 1, 0);
        plt::named_plot("sending", n_inst_sending);
        plt::named_plot("executing", n_inst_executing);
        plt::named_plot("done", n_inst_done);
        plt::legend();

        plt::show();
    }
    void plot_save(const char *path, const char *title = "Default Title")
    {
        namespace plt = matplotlibcpp;

        plt::suptitle(title);

        plt::subplot2grid(2, 1, 0, 0);
        plt::named_plot("transmission rate", trans_rate);
        plt::legend();

        plt::subplot2grid(2, 1, 1, 0);
        plt::named_plot("sending", n_inst_sending);
        plt::named_plot("executing", n_inst_executing);
        plt::named_plot("done", n_inst_done);
        plt::legend();

        plt::save(path);
    }
};