/*
 * Created on Fri Jan 05 2024
 *
 * Filename: greedyagent.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Choose server with max recent transmission rate.
 */

#pragma once

#include "agents/sentientagent.h"
#include "agents/tracker.h"
#include "experiments/tracker.h"

class GreedyAgent : public SentientAgent {
public:
    GreedyAgent(int id, int server_chosing_method = SERVER_CHOSING_METHOD::BY_ESTIMATION,
     int n_link = N_LINK) : SentientAgent(id, server_chosing_method, n_link){}

    Action act() override
    {
        if(estimation_turns < 10) return make_action();
        while(sending.size() < n_link)
        {
            Instance *inst = job->get_available_instance();
            if(!inst) break;
            vector<int> server_list = get_available_servers();
            if(server_list.empty()) break;

            int choice = choose_server(server_list, inst->size);// the only difference with random agent

            inst->set_pending();                    // have to set state to pending manually
            sending.push_back({inst, choice, 0});   // inst_ptr, server_id, sent_bits
        }
        return make_action();
    }
    void feedback(Feedback fb) override
    {
        update(fb);
    }
};


/**
 * @brief A variety of GreedyAgent with epsilon-greedy strategy. Upon each decision,
 * it choose server randomly with a probability of epsilon, otherwise choose the one 
 * with highest estimated aggregate information.
*/
class EpsGreedyAgent : public GreedyAgent {
protected:
    const double epsilon;
    const int server_chosing_method;

    int choose_server_by_estimation(vector<int> server_list, int size)
    {
        if(uniform_real(0, 1) < epsilon)
            return server_list[rand() % server_list.size()];

        return SentientAgent::choose_server_by_estimation(server_list, size);
    }

    int choose_server_by_transmission(vector<int> server_list, int size)
    {
        if(uniform_real(0, 1) < epsilon)
            return server_list[rand() % server_list.size()];

        return SentientAgent::choose_server_by_estimation(server_list, size);
    }

    int choose_server_by_transmission_no_x(vector<int> server_list, int size) override
    {
        if(uniform_real(0, 1) < epsilon)
            return server_list[rand() % server_list.size()];

        return SentientAgent::choose_server_by_estimation(server_list, size);
    }

public:
    EpsGreedyAgent(int id, int server_chosing_method = SERVER_CHOSING_METHOD::BY_ESTIMATION, double eps = 0.1) : 
    GreedyAgent(id, server_chosing_method), epsilon(eps), server_chosing_method(server_chosing_method) {}
};