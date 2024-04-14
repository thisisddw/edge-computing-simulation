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
    class StatePolicy {
    public:
        virtual ~StatePolicy() {};
        virtual int get_state(int, double *, Feedback) = 0;
    };

    class config {
    public:    
        int init_number_link;           // to initialize BaseAgent::n_link
        double ctl_interval;            // interval between two controller actions
        RlController *controller;      // to control BaseAgent::n_link
        /**
         * state_policy is called to determine the current state.
         * It accepts 3 params: int n_link, double *estimated_I, Feedback fb
        */
        StatePolicy *state_policy;
    
        config( int init_number_link = N_LINK, 
                double ctl_interval = 0.1, 
                RlController *controller = NULL, 
                StatePolicy *state_policy = new AdaptiveAgent::SingleState())
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
        delete cfg.state_policy;
    }

    /**
     * only state 0 is used
    */
    class SingleState : public StatePolicy { 
        public: int get_state(int n_link, double *estimated_I, Feedback fb) override { return 0; } 
    };
    /**
     * two states are used, state 0 when every server works normally, state 1 when some server failure
     * is taking place
    */
    class DoubleState : public StatePolicy { 
    public: 
        int get_state(int n_link, double *estimated_I, Feedback fb) override
        {
            for (int i = 0; i < N_BS; i++)
                if (!fb.a[i])
                    return 1;   // some server fails
            return 0;           // every server works well
        }
    };
    /**
     * 
    */
    class AvgLinkStatus3State : public StatePolicy { 
        const double lr;
        bool initialized;
        double mean;
        double s2;

        int cnt[3];

        #define SQ(x) ((x)*(x))

    public: 
        // perhaps it's better to set a lower lr than the controller
        AvgLinkStatus3State(double lr) : StatePolicy(), lr(lr), initialized(false) 
        { 
            cnt[0] = cnt[1] = cnt[2] = 0;
        }
        ~AvgLinkStatus3State()
        {
            fprintf(stderr, "~AvgLinkStatus3State(): cnt[medium, bad, good] = [%d, %d, %d]\n", cnt[0], cnt[1], cnt[2]);
        }
        
        int get_state(int n_link, double *estimated_I, Feedback fb) override
        {
            double val = 0;
            for (int i = 0; i < N_BS; i++)
                // val += estimated_I[i];
                if (fb.a[i])
                    val = std::max(val, estimated_I[i]);
            // val /= N_BS;
            
            if (!initialized)
            {
                mean = val;
                s2 = SQ(0.5 * mean);
                initialized = true;
            }

            double s = sqrt(s2), t = 0.3 * s;
            int state = val > mean - t && val < mean + t ? 0      // medium link status
                                        : val <= mean - t ? 1     // bad link status
                                                          : 2;    // good link status
            cnt[state]++;

            // update estimation
            s2 += lr * (SQ(val - mean) - s2);
            mean += lr * (val - mean);

            return state;
        }

        #undef SQ
    };

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
                cfg.state_policy->get_state(n_link, estimated_I, fb));
            n_link = controller->act() + 1;

            last_ctl_upd = current_time;
            accumulated_reward = 0;
        }
    }
};