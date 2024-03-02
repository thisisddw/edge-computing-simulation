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

#include "agents/baseagent.h"

class GreedyAgent : public BaseAgent {
protected:
    unsigned int estimation_turns;
    double estimated_I[N_BS];  // estimated aggregate information

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
    GreedyAgent(int id) : BaseAgent(id) 
    {
        estimation_turns = 0;
        memset(estimated_I, 0, sizeof(estimated_I));
    }

    Action act() override
    {
        if(estimation_turns < 10) return make_action();
        while(sending.size() < N_LINK)
        {
            Instance *inst = job.get_available_instance();
            if(!inst) break;
            vector<int> server_list = get_available_servers();
            if(server_list.empty()) break;

            int choice = choose_server(server_list);// the only difference with random agent

            inst->state = Instance::P;              // have to set state to pending manually
            sending.push_back({inst, choice, 0});   // inst_ptr, server_id, sent_bits
        }
        return make_action();
    }
    void feedback(Feedback fb) override
    {
        update(fb);

        const int A = 0.8;
        for(int i = 0; i < N_BS; i++)
            estimated_I[i] = A * estimated_I[i] + (1 - A) * fb.i[i];

        estimation_turns++;
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

    virtual int choose_server(vector<int> server_list) override
    {
        assert(!server_list.empty());

        if(uniform_real(0, 1) < epsilon)
            return server_list[rand() % server_list.size()];

        int id = -1;
        for(int s: server_list)
            if(id == -1 || estimated_I[s] > estimated_I[id])
                id = s;
        return id;
    }

public:
    EpsGreedyAgent(int id, double eps) : GreedyAgent(id), epsilon(eps) {}
};