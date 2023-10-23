/*
 * Created on Sun Oct 22 2023
 *
 * Filename: randomagent.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Randomly choose an available server to deploy instance if possible.
 */

#pragma once

#include "baseagent.h"

class RandomAgent : public BaseAgent {
public:
    RandomAgent(int id) : BaseAgent(id) {}

    Action act() override
    {
        while(sending.size() < N_LINK)
        {
            Instance *inst = job.get_available_instance();
            if(!inst) break;
            vector<int> server_list = get_available_servers();
            if(server_list.empty()) break;

            int choice = server_list[rand() % server_list.size()];
            inst->state = Instance::P;              // have to set state to pending manually
            sending.push_back({inst, choice, 0});   // inst_ptr, server_id, sent_bits
        }
        return make_action();
    }
    void feedback(Feedback fb) override
    {
        update(fb);
    }
};