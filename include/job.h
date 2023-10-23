/*
 * Created on Sun Oct 22 2023
 *
 * Filename: job.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Agents have jobs to do. One job is consist of multiple
 * tasks, while a task is consist of multiple instances. Instances are
 * inseperable, which means one instance should be deployed to exactly
 * one server.
 */

#pragma once

#include <vector>
#include "parameters.h"

using std::vector;

class Instance {
public:
    double duration;    // unit: s
    int size;           // unit: bit
    int state;

    // 3 kinds of instance state
    enum {
        A,  // available
        P,  // pending (being send or waiting server to finish)
        D   // done
    };

    Instance() : duration(0), size(0), state(A) {}
    Instance(int d, int size) : duration(d), size(size), state(A) {}

    bool is_available() { return state == A; }
    bool is_pending() { return state == P; }
    bool is_done() { return state == D; }
};

class Task {
public:
    vector<Instance> instances;
    int priority;

    bool is_done()
    {
        for(Instance &i: instances)
            if(!i.is_done())
                return false;
        return true;
    }
    Instance *get_available_instance()
    {
        for(unsigned int i = 0; i < instances.size(); i++)
            if(instances[i].is_available())
                return &instances[i];
        return NULL;
    }
};

class Job {
public:
    /**
     * tasks should be arranged in descending priority order
    */
    vector<Task> tasks;

    /**
     * @brief Pick an instance that is available from a task whose prerequisites are met.
    */
    Instance *get_available_instance()
    {
        int first_undone = -1;
        for(unsigned i = 0; i < tasks.size(); i++)
        {
            if(tasks[i].is_done())
                continue;
            if(first_undone == -1)
                first_undone = i;
            if(tasks[i].priority < tasks[first_undone].priority)
                break;
            Instance *ret = tasks[i].get_available_instance();
            if(ret)
                return ret;
        }
        return NULL;
    }

    /**
     * @brief Number of instances in all tasks.
    */
    int number_instances()
    {
        int cnt = 0;
        for(Task &t: tasks)
            cnt += (int)t.instances.size();
        return cnt;
    }

    /**
     * @brief Number of instances that have been deploied and finished.
    */
    int number_instances_done()
    {
        int cnt = 0;
        for(Task &t: tasks)
            for(Instance &i: t.instances)
                if(i.is_done())
                    cnt++;
        return cnt;
    }
};

class JobLoader {
    Job jobs[N_USER];
public:
    JobLoader(const char *path) 
    {
        // should load jobs from a config file instead

        int n_instances = 10, duration = 2, size = 2e6;
        Task t;
        t.instances.push_back(Instance(duration, size));
        Job j;
        while(n_instances--)
            j.tasks.push_back(t);
        for(int i = 0; i < N_USER; i++)
            jobs[i] = j;
    }
    /**
     * @brief Agents should call this function to get their job.
    */
    Job get_job(int id)
    {
        return jobs[id];
    }
};