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
#include "experiment.h"
#include "experiments/baseexp.h"
#include <memory>

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
        // int current_slot = static_cast<int>(current_time / TTR);

        for(int i = 0; i < N_BS; i++)
        {
            #ifdef FAILURE_ON
                // bool is_failure_slot = (std::find(server_failure_slot_data[i].cat_pred_fault_time_slots.begin(), 
                //                     server_failure_slot_data[i].cat_pred_fault_time_slots.end(), 
                //                     current_slot) != server_failure_slot_data[i].cat_pred_fault_time_slots.end());
                //                     // (std::find(server_failure_slot_data[i].xgb_pred_fault_time_slots.begin(), 
                //                     // server_failure_slot_data[i].xgb_pred_fault_time_slots.end(), 
                //                     // current_slot) != server_failure_slot_data[i].xgb_pred_fault_time_slots.end()) ||
                //                     // (std::find(server_failure_slot_data[i].lgb_pred_fault_time_slots.begin(), 
                //                     // server_failure_slot_data[i].lgb_pred_fault_time_slots.end(), 
                //                     // current_slot) != server_failure_slot_data[i].lgb_pred_fault_time_slots.end());
                // double prob = is_failure_slot ? 1.0 : 0.0;
                // // if(prob==1){
                // //     printf("current_slot%dprob=1",current_slot);
                // // }

                // estimated_I[i] = A * estimated_I[i] + (1 - A) * fb.i[i] * (1 - prob);
                estimated_I[i] = A * estimated_I[i] + (1 - A) * fb.i[i];

            #else
                estimated_I[i] = A * estimated_I[i] + (1 - A) * fb.i[i];
            #endif
        }

        estimation_turns++;
    }

    const int server_chosing_method;

    virtual int choose_server_by_estimation(vector<int> server_list, int size)
    {

        int server_id = -1;

        assert(!server_list.empty());
        
        for(int s : server_list){

                if (server_id == -1 || estimated_I[s] > estimated_I[server_id])
                    server_id = s; 

        }
        return server_id;
    }

    virtual int choose_server_by_transmission(vector<int> server_list, int size)
    {
        assert(!server_list.empty());

        double total_transfer_time;
        int total_number_inst_done;

        total_transfer_time = get_total_time_spent();
        total_number_inst_done = get_total_number_inst_done();

        double E_transmission_time[N_BS];
        int server_id = -1;
        double estimated_R[N_BS];

        for(int i = 0; i < N_BS; i++){
            estimated_R[i] = W_SUB * log2(1 + P_MAX / (sending.size() + 1) * estimated_I[i]);
        }

        assert(!server_list.empty());

        
        for(int s : server_list){

            #ifdef FAILURE_ON
                double prob = exp(-size / (server_F_INTERVAL[s] * estimated_R[s])) + 1e-6;
                E_transmission_time[s] = (1 - prob) * (server_F_INTERVAL[s] + total_transfer_time / (total_number_inst_done+1));

                if (server_id == -1 || E_transmission_time[s] < E_transmission_time[server_id])
                    server_id = s; 
                // printf(">>>>E_transmission_time[s]%lf, estimated_R[s]%lf, prob%lf",E_transmission_time[s], estimated_R[s], prob);

            #else
                if (server_id == -1 || estimated_I[s] > estimated_I[server_id])
                    server_id = s; 
            #endif

        }
        return server_id;
    }

    virtual int choose_server_by_transmission_no_x(vector<int> server_list, int size)
    {
        assert(!server_list.empty());

        double E_transmission_time[N_BS];
        int server_id = -1;
        double estimated_R[N_BS];

        for(int i = 0; i < N_BS; i++){
            estimated_R[i] = W_SUB * log2(1 + P_MAX / (sending.size() + 1) * estimated_I[i]);
        }

        assert(!server_list.empty());

        
        for(int s : server_list){

            #ifdef FAILURE_ON
                double prob = exp(-size / (server_F_INTERVAL[s] * estimated_R[s])) + 1e-6;
                E_transmission_time[s] = server_F_INTERVAL[s] * (1 - prob) / prob;
                // printf(">>>>E_transmission_time[s]%lf, estimated_R[s]%lf, prob%lf",E_transmission_time[s], estimated_R[s], prob);

                if (server_id == -1 || E_transmission_time[s] < E_transmission_time[server_id])
                    server_id = s; 

            #else
                if (server_id == -1 || estimated_I[s] > estimated_I[server_id])
                    server_id = s; 
            #endif

        }
        return server_id;
    }

    virtual int choose_server(vector<int> server_list, int size){
        int ret = 0;
        if (server_chosing_method == SERVER_CHOSING_METHOD::BY_ESTIMATION)
            ret = choose_server_by_estimation(server_list, size);
        else if (server_chosing_method == SERVER_CHOSING_METHOD::BY_TRANSMISSION)
            ret = choose_server_by_transmission(server_list, size);
        else if (server_chosing_method == SERVER_CHOSING_METHOD::BY_TRANSMISSION_NO_X)
            ret = choose_server_by_transmission_no_x(server_list, size);
        else
            assert(false);

        return ret;
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
    enum SERVER_CHOSING_METHOD {
        BY_ESTIMATION,
        BY_TRANSMISSION,
        BY_TRANSMISSION_NO_X
    };

    SentientAgent(int id, int server_chosing_method = SERVER_CHOSING_METHOD::BY_ESTIMATION, int n_link = N_LINK) : BaseAgent(id, n_link), server_chosing_method(server_chosing_method)
    {
        estimation_turns = 0;
        memset(estimated_I, 0, sizeof(estimated_I));
    }
};