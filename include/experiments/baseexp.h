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
#include "experiments/tracker.h"
#include "matplotlibcpp.h"

class BaseExperiment : public Experiment {
protected:
    ExpTracker exp_tracker;
    AgentTracker tracker[N_USER];

    void track() override
    {
        exp_tracker.track();
        for(int i = 0; i < N_USER; i++)
            tracker[i].track(feedbacks[i]);
    }
    void tracker_setup()
    {
        for(int i = 0; i < N_USER; i++)
            tracker[i].set_agent(agents[i]);
    }
public:
    BaseExperiment(const char *name) : Experiment(name), exp_tracker(this) {}

    /**
     * @brief Returns a string representation of the server selection method.
    */
    const char* get_server_chosing_method_str(int server_chosing_method) {
        switch (server_chosing_method) {
            case 0: return "by_estimation";
            case 1: return "by_transmission";
            case 2: return "by_transmission_no_x";
            default: assert(false);
        }
    }

    /**
     * @brief Create a dummy BaseExperiment with data of input exps.
    */
    static BaseExperiment *average(vector<BaseExperiment *> exps)
    {
        BaseExperiment *ret = new BaseExperiment(exps.back()->get_name());
        for (int i = 0; i < N_USER; i++)
            ret->tracker[i] = exps.back()->tracker[i];
        vector<ExpTracker *> exp_trackers;
        for (auto it: exps)
            exp_trackers.push_back(&it->exp_tracker);
        ret->exp_tracker = ExpTracker::average(exp_trackers);
        return ret;
    }

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
    /**
     * @brief Plot and save overview stats of this experiment.
     * @return A string of image name.
    */
    string plot_save(const char *path)
    {
        std::string sname = std::string(get_name());
        exp_tracker.plot_save((path + sname + "-overview.png").c_str(), (sname + ": overview").c_str());
        return sname + "-overview.png";
    }

    /**
     * @brief Get statistiacal summary with AgentTracker::summarize().
     * @return A map of AgentSummaries, with assigned agent name as keys and also
     *          a special key "total" refer to the sums of each numbers.
    */
    ExpSummary summarize()
    {
        ExpSummary s;
        std::string sname = std::string(get_name());
        AgentSummary total = {};
        for(int i = 0; i < N_USER; i++)
        {
            string aname = "agent" + std::to_string(i);
            s[aname] = tracker[i].summarize();

            total.gross_trans += s[aname].gross_trans;
            total.actual_trans += s[aname].actual_trans;
            total.inst_done += s[aname].inst_done;
        }
        s["total"] = total;
        return s;
    }

    /**
     * @brief Get average statistiacal summary with AgentTracker::summarize().
     * @return A map of AgentSummaries, with assigned agent name as keys and also
     *          a special key "total" refer to the sums of each numbers.
    */
   static ExpSummary average_summary(vector<BaseExperiment *> exps)
    {
        ExpSummary avg_summary;
        AgentSummary total = {};
        int num_experiments = exps.size();
        // printf(">>>num_experiments:%d\n", num_experiments);

        for (int i = 0; i < N_USER; i++)
        {
            std::string aname = "agent" + std::to_string(i);
            AgentSummary agent_total = {};

            for (auto &exp : exps)
            {
                AgentSummary agent_summary = exp->summarize().at(aname);

                agent_total.gross_trans += agent_summary.gross_trans;
                agent_total.actual_trans += agent_summary.actual_trans;
                agent_total.inst_done += agent_summary.inst_done;

                // printf(">>>agent_total_exps: gross_trans: %lf, actual_trans: %lf, inst_done: %lf\n", agent_total.gross_trans, agent_total.actual_trans, agent_total.inst_done);
            }

            agent_total.gross_trans /= num_experiments;
            agent_total.actual_trans /= num_experiments;
            agent_total.inst_done /= num_experiments;

            // printf(">>>agent_avg_exps: gross_trans: %lf, actual_trans: %lf, inst_done: %lf\n\n", agent_total.gross_trans, agent_total.actual_trans, agent_total.inst_done);

            avg_summary[aname] = agent_total;
        }

        for (const auto &entry : avg_summary)
        {
            const AgentSummary &agent_summary = entry.second;
            total.gross_trans += agent_summary.gross_trans;
            total.actual_trans += agent_summary.actual_trans;
            total.inst_done += agent_summary.inst_done;

            // printf(">>>total_exps: gross_trans: %lf, actual_trans: %lf, inst_done: %lf\n\n", total.gross_trans, total.actual_trans, total.inst_done);
        }

        avg_summary["average"] = total;

        return avg_summary;
    }
    
};
