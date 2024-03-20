/*
 * Created on Thu Mar 07 2024
 *
 * Filename: adaptiveagent.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: 
 */

#pragma once

#include "agents/sentientagent.h"
#include "agents/rl_controller/rl_controller.h"

/**
 * @brief Agent controled by rl_controller. It should behave exactly same as GreedyAgent with
 * default argument values of constructer. The controller would be destructed during destruction.
*/
class AdaptiveAgent : public SentientAgent {
public:
    class config {
    public:    
        int init_number_link;           // to initialize BaseAgent::n_link
        double ctl_interval;            // interval between two controller actions
        RlController *controller;      // to control BaseAgent::n_link
        /**
         * state_policy is called to determine the current state.
         * It accepts 3 params: int n_link, double *estimated_I, Feedback fb
        */
        int (*state_policy)(int, double *, Feedback);
    
        config( int init_number_link = N_LINK, 
                double ctl_interval = 0.1, 
                RlController *controller = NULL, 
                int (*state_policy)(int, double *, Feedback) = AdaptiveAgent::single_state)
        {
            this->init_number_link = init_number_link;
            this->ctl_interval = ctl_interval;
            this->controller = controller;
            this->state_policy = state_policy;
        }
    };

private:
    double get_reward(Feedback fb)
    {
        double ret = 0;
        for(auto si: sending)
            ret += fb.r[si.server_id] * TTR;
        return ret;
    }

protected:
    const config cfg;

    RlController *controller;
    double last_ctl_upd;
    double accumulated_reward;

    virtual int choose_server(vector<int> server_list)
    {
        assert(!server_list.empty());
        int id = -1;
        for(int s: server_list)
            if(id == -1 || estimated_I[s] > estimated_I[id])
                id = s;
        return id;
    }

public:
    AdaptiveAgent(int id, config cfg = config())
     : SentientAgent(id, cfg.init_number_link), cfg(cfg), controller(cfg.controller)
    {
        if (!cfg.controller)
            this->controller = new TrivialController(n_link);

        last_ctl_upd = current_time;
        accumulated_reward = 0;
    }
    ~AdaptiveAgent()
    {
        delete controller;
    }

    /**
     * only state 0 is used
    */
    static int single_state(int n_link, double *estimated_I, Feedback fb) { return 0; }
    /**
     * two states are used, state 0 when every server works normally, state 1 when some server failure
     * is taking place
    */
    static int double_state(int n_link, double *estimated_I, Feedback fb)
    {
        for (int i = 0; i < N_BS; i++)
            if (!fb.a[i])
                return 1;   // some server fails
        return 0;           // every server works well
    }

    Action act() override
    {
        if(estimation_turns < 10) return make_action();

        // deal with the case where sending.size() is greater than n_link
        // remove the instance with lowest estimated_I
        while(sending.size() > n_link)
        {
            auto to_erase = sending.begin();
            for(auto it = sending.begin(); it != sending.end(); it++)
                if(estimated_I[it->server_id] < estimated_I[to_erase->server_id])
                    to_erase = it;
            to_erase->instance_ptr->set_available();
            sending.erase(to_erase);
        }

        // copied from GreedyAgent
        while(sending.size() < n_link)
        {
            Instance *inst = job->get_available_instance();
            if(!inst) break;
            vector<int> server_list = get_available_servers();
            if(server_list.empty()) break;

            int choice = choose_server(server_list);// the only difference with random agent

            inst->set_pending();                    // have to set state to pending manually
            sending.push_back({inst, choice, 0});   // inst_ptr, server_id, sent_bits
        }
        return make_action();
    }
    void feedback(Feedback fb) override
    {
        accumulated_reward += get_reward(fb);
        update(fb);

        if (last_ctl_upd + cfg.ctl_interval < current_time)
        {
            controller->feedback(accumulated_reward / (current_time - last_ctl_upd) * cfg.ctl_interval, 
                cfg.state_policy(n_link, estimated_I, fb));
            n_link = controller->act() + 1;

            last_ctl_upd = current_time;
            accumulated_reward = 0;
        }
    }
};