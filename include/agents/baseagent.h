/*
 * Created on Sun Oct 22 2023
 *
 * Filename: baseagent.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: BaseAgent have access to its job.
 */

#pragma once

#include "common.h"
#include "agent.h"
#include "job.h"

class AgentTracker;

class BaseAgent : public Agent {
protected:
    const int id;
    Job * const job;
    unsigned int n_link;
    double total_time_spent;
    int total_number_inst_done;
    long long total_inst_size;

    struct SendingInstance {
        Instance *instance_ptr;
        int server_id;
        double sent_bits;
        double start_time;
        double end_time;
    };
    struct PendingInstance {
        Instance *instance_ptr;
        int server_id;
        double finish_time;
    };

    vector<SendingInstance> sending;
    vector<PendingInstance> pending;

    bool server_available[N_BS];

    /**
     * @brief Get all servers that have not been occupied by sending instances.
    */
    vector<int> get_available_servers()
    {
        vector<int> ret;
        for(int i = 0; i < N_BS; i++)
        {
            bool used = false;
            for(auto s: sending)
                if(s.server_id == i)
                    { used = true; break; }
            if(!used && server_available[i])
                ret.push_back(i);
        }
        return ret;
    }

    #define integrity_check()\
        assert(sending.size() <= n_link);\
        for(auto s: sending)\
            assert(s.instance_ptr->is_pending());\
        for(auto p: pending)\
            assert(p.instance_ptr->is_pending());

    /**
     * @brief Make action according to sending instances.
    */
    virtual Action make_action()
    {
        IF_DEBUG(integrity_check());

        Action a;
        for(auto s: sending)
            a.p[s.server_id] = P_MAX / sending.size();

        return a;
    }

    /**
     * @brief Update sending and pending instances with fb. This function
     * should be called in every overriding feedback function.
    */
    void update(Feedback fb)
    {
        IF_DEBUG(integrity_check());

        memcpy(server_available, fb.a, sizeof(server_available));

        // Update sent_bits, then move all transmission finished instances to pending.
        for(auto it = sending.begin(); it != sending.end();)
        {
            // fprintf(stderr, "Checking sending list: size = %zu\n", sending.size());
            it->sent_bits += fb.r[it->server_id] * TTR;
            if (it->start_time == 0.0) {
                it->start_time = current_time;
            }
            // fprintf(stderr, "_____________________________");
            // fprintf(stderr, "start_time = %f\n", it->start_time);
            if(!fb.a[it->server_id])    // in case the server is broken
            {
                // IF_DEBUG(fprintf(stderr, "\rAgent%d sending instance interrupt due to failure on server%d   \n", id, it->server_id););
                it->instance_ptr->set_available();
                it = sending.erase(it);
            }
            else if(it->sent_bits >= it->instance_ptr->size)
            {
                it->end_time = current_time;
                total_time_spent += (it->end_time - it->start_time);
                // fprintf(stderr, "start_time = %f, end_time = %f, sent_bits = %f\n", it->start_time, it->end_time, it->sent_bits);
                pending.push_back({it->instance_ptr, it->server_id, current_time + it->instance_ptr->duration});
                it = sending.erase(it);
            }
            else
                it++;

        }
        // Remove all finished pending instances.
        for(auto it = pending.begin(); it != pending.end();)
        {
            if(!fb.a[it->server_id])    // in case the server is broken
            {
                // IF_DEBUG(fprintf(stderr, "\rAgent%d executing instance interrupt due to failure on server%d   \n", id, it->server_id););
                it->instance_ptr->set_available();
                it = pending.erase(it);
            }
            else if(it->finish_time <= current_time)
            {
                it->instance_ptr->set_done();
                total_number_inst_done++;
                total_inst_size += static_cast<long long>(it->instance_ptr->size);
                // printf(">>>static_cast<size_t>(it->instance_ptr->size):%lld", static_cast<long long>(it->instance_ptr->size));
                it = pending.erase(it);
            }
            else
                it++;
        }
    }

    double get_total_time_spent() const {
        return total_time_spent;
    }

    double get_total_number_inst_done() const {
        return total_number_inst_done;
    }

    double get_total_inst_size() const {
        return total_inst_size;
    }

    #undef integrity_check
    
public:
    BaseAgent(int id, int n_link = N_LINK) : id(id), job(job_loader.get_job(id)), n_link(n_link)
    {
        for(int i = 0; i < N_BS; i++)
            server_available[i] = true;
        total_number_inst_done = 0;
        total_time_spent = 0;
        total_inst_size = 0;
    }
    virtual ~BaseAgent() { delete job; }

    friend class AgentTracker;
};
