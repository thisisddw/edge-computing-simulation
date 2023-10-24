/*
 * Created on Mon Oct 23 2023
 *
 * Filename: baseexp.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Base class for experiments, in charge of keep statistics.
 */

#pragma once

#include "experiment.h"
#include "agents/tracker.h"
#include "matplotlibcpp.h"

class BaseExperiment : public Experiment {
protected:
    AgentTracker tracker[N_USER];

    void track() override
    {
        for(int i = 0; i < N_USER; i++)
            tracker[i].track(feedbacks[i]);
    }
public:
    BaseExperiment(const char *name) : Experiment(name) {}

    /**
     * @brief Plot each agent with AgentTracker::plot().
    */
    void plot_agents()
    {
        for(int i = 0; i < N_USER; i++)
            tracker[i].plot((std::string(get_name()) + ": agent" + std::to_string(i)).c_str());
    }
    /**
     * @brief Plot and save each agent with AgentTracker::plot_save().
     * @return A list of image names.
    */
    vector<std::string> plot_save_agents(const char *path)
    {
        std::string sname = std::string(get_name());
        vector<std::string> ret;
        for(int i = 0; i < N_USER; i++)
        {
            tracker[i].plot_save((path + sname + "-agent" + std::to_string(i) + ".png").c_str(),
             (sname + ": agent" + std::to_string(i)).c_str());
            ret.push_back(sname + "-agent" + std::to_string(i) + ".png");
        }
        return ret;
    }
};
