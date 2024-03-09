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
#include "assert.h"

#ifdef DEBUG
    #define IF_DEBUG(x) x
#else
    #define IF_DEBUG(x)
#endif

using std::vector;

class Instance {
private:
    int state;
    unsigned int location;      // this == parent->instances[location]
public:
    class Task * const parent;
    const double duration;      // unit: s
    const int size;             // unit: bit

    // 3 kinds of instance state
    enum {
        A,  // available
        P,  // pending (being send or waiting server to finish)
        D   // done
    };

    Instance(int d, int size, class Task *parent) : state(A), parent(parent), duration(d), size(size) {}

    bool is_available() { return state == A; }
    bool is_pending() { return state == P; }
    bool is_done() { return state == D; }

    void set_available();
    void set_pending();
    void set_done();

    friend class Task;
};

class Task {
private:
    unsigned int cnt_a, cnt_p, cnt_d;
    vector<Instance *> instances;

    unsigned int first_available;    // should be maintained in Task::add() and Instance::set_xxx()

public:
    const int priority;

    #define task_integrity_check()\
        do {\
            for (Instance *i: instances)\
            {\
                assert(i->parent == this && instances[i->location] == i);\
                assert(!i->is_available() || i->location >= first_available);\
            }\
            assert(first_available == size() || instances[first_available]->is_available());\
            assert(cnt_a + cnt_p + cnt_d == instances.size());\
        } while(0)

    /**
     * @param priority Tasks with higher priority would be executed first
    */
    Task(int priority = 0) : priority(priority) { cnt_a = cnt_p = cnt_d = 0; first_available = 0; }

    /**
     * @brief delete all instances while destructing
    */
    ~Task() { for (Instance *i: instances) delete i; }

    bool is_done() const
    {
        IF_DEBUG(task_integrity_check());
        return cnt_d == instances.size();
    }
    unsigned int size() const { return instances.size(); }
    unsigned int count_instances_done() const { return cnt_d; }
    
    void add(Instance *i);
    const vector<Instance *> &get_instances() { return instances; }

    Instance *get_available_instance()
    {
        IF_DEBUG(task_integrity_check());
        return first_available < instances.size() ? instances[first_available] : NULL;
    }

    friend class Instance;
};

/**
 * @brief Tasks would be delete within destruction process
 * 
 * The cost of every operation depends on the number of Tasks.
 * To keep performance high, do not add too many Tasks to a Job.
*/
class Job {
    /**
     * tasks should be arranged in descending priority order
    */
    vector<Task *> tasks;

public:
    ~Job() { for (Task *t: tasks) delete t; }

    void add(Task *t) { tasks.push_back(t); }
    const vector<Task *> &get_tasks() { return tasks; }

    /**
     * @brief Pick an instance that is available from a task whose prerequisites are met.
    */
    Instance *get_available_instance()
    {
        int first_undone = -1;
        for(unsigned i = 0; i < tasks.size(); i++)
        {
            if(tasks[i]->is_done())
                continue;
            if(first_undone == -1)
                first_undone = i;
            if(tasks[i]->priority < tasks[first_undone]->priority)
                break;
            Instance *ret = tasks[i]->get_available_instance();
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
        for(Task *t: tasks)
            cnt += t->size();
        return cnt;
    }

    /**
     * @brief Number of instances that have been deploied and finished.
    */
    int number_instances_done()
    {
        int cnt = 0;
        for(Task *t: tasks)
            cnt += t->count_instances_done();
        return cnt;
    }
};

class JobLoader {
private:
    Job jobs[N_USER];

    Job *creat_fake_job()
    {
        int n_instances = 400, duration = 2, size = 2e6;
        Job *j = new Job();
        Task *t = new Task();
        while(n_instances--)
            t->add(new Instance(duration, size, t));
        j->add(t);
        return j;
    }

public:
    JobLoader(const char *path) {}

    /**
     * @brief Agents should call this function to get their job.
    */
    Job *get_job(int id)
    {
        // should load jobs from a config file instead
        return creat_fake_job();
    }
};