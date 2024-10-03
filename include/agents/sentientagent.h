/*
 * Created on Thu Mar 07 2024
 *
 * Filename: sentientagent.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: SentientAgent, derived from BaseAgent, is able to estimate
 * the state of links, base on their historical stats.
 */

#pragma once

#include "agents/baseagent.h"

/**
 * @brief This is intended to be base class for every non-trivial strategy,
 * with the necessary ability to estimate the state of links base on their 
 * historical stats.
*/
class SentientAgent : public BaseAgent {
protected:
    unsigned int estimation_turns;
    double estimated_I[N_BS];  // estimated aggregate information

    /**
     * @brief Update sending and pending instances with fb. Also update
     * estimated_I. This function should be called in every overriding
     * feedback function.
    */
    void update(Feedback fb)
    {
        BaseAgent::update(fb);
        const double A = 0.8;
        int current_slot = static_cast<int>(current_time / TTR);

        for(int i = 0; i < N_BS; i++)
        {
            #ifdef FAILURE_ON
                bool is_failure_slot = (std::find(server_failure_slot_data[i].cat_pred_fault_time_slots.begin(), 
                                    server_failure_slot_data[i].cat_pred_fault_time_slots.end(), 
                                    current_slot) != server_failure_slot_data[i].cat_pred_fault_time_slots.end());
                                    // (std::find(server_failure_slot_data[i].xgb_pred_fault_time_slots.begin(), 
                                    // server_failure_slot_data[i].xgb_pred_fault_time_slots.end(), 
                                    // current_slot) != server_failure_slot_data[i].xgb_pred_fault_time_slots.end()) ||
                                    // (std::find(server_failure_slot_data[i].lgb_pred_fault_time_slots.begin(), 
                                    // server_failure_slot_data[i].lgb_pred_fault_time_slots.end(), 
                                    // current_slot) != server_failure_slot_data[i].lgb_pred_fault_time_slots.end());
                double prob = is_failure_slot ? 1.0 : 0.0;
                // if(prob==1){
                //     printf("current_slot%dprob=1",current_slot);
                // }

                estimated_I[i] = A * estimated_I[i] + (1 - A) * fb.i[i] * (1 - prob);
            #else
                estimated_I[i] = A * estimated_I[i] + (1 - A) * fb.i[i];
            #endif
        }

        estimation_turns++;
    }
    
    // virtual Action make_action() override
    // {
    //     IF_DEBUG(integrity_check());

    //     Action a;
    //     for(auto s: sending)
    //         a.p[s.server_id] = P_MAX / sending.size();

    // #define RATE_INC(sid) (log2(1 + estimated_I[sid] * (a.p[sid] + P_MAX/n)) - log2(1 + estimated_I[sid] * a.p[sid])) 

    //     int n = sending.size() * 3;
    //     for (int i = 0; i < n; i++)
    //     {
    //         int id = 0, sid = sending[id].server_id;
    //         double val = RATE_INC(sid);
    //         for (int i = 0; i < (int)sending.size(); i++)
    //         {
    //             int t = sending[i].server_id;
    //             double inc = RATE_INC(t);
    //             if (inc > val)
    //                 id = i, sid = t, val = inc;
    //         }
    //         a.p[sid] += P_MAX / n;
    //     }

    // #undef RATE_INC

    //     return a;
    // }

public:
    SentientAgent(int id, int n_link = N_LINK) : BaseAgent(id, n_link)
    {
        estimation_turns = 0;
        memset(estimated_I, 0, sizeof(estimated_I));
    }
};