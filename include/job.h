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
#include <ctime>

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
    const double size;             // unit: bit

    // 3 kinds of instance state
    enum {
        A,  // available
        P,  // pending (being send or waiting server to finish)
        D   // done
    };

    Instance(double d, double size, class Task *parent) : state(A), parent(parent), duration(d), size(size) {}

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


double pareto(double x_m, double alpha);    // from utils.cpp

class JobLoader {
private:
    Job jobs[N_USER];

    // Job *create_fake_job()
    // {
    //     int n_instances = 4000*5, duration = 2, size = 2e6;
    //     Job *j = new Job();
    //     Task *t = new Task();
    //     while(n_instances--)
    //         t->add(new Instance(duration, size, t));
    //     j->add(t);
    //     return j;
    // }

    Job* create_fake_job() {
        // int n_instances = 4000 * 5, size = 2e6, duration = 2;
        int n_instances = 4000 * 5;
        double size, duration;
        Job* j = new Job();
        Task* t = new Task();

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        while (n_instances--) {
            // size = (std::rand() % 5000001) + 1000000; // [1e6, 15e6]
            // duration = (size / 1000000); // size=1e6 -> duration=1，size=2e6 -> duration=2...

            // Randomly decide whether to generate a small or large size
            // if ((std::rand() % 100) < 80) { // 80% chance for small size
            //     size = 1e6 + (std::rand() % static_cast<int>(2e6)); // Range: 1e6 to 2e6
            //     // size = 2e6;
            //     duration = size / 1e6; // Duration based on size
            // } else { // 20% chance for large size
            //     size = 3e6 + (std::rand() % static_cast<int>(10e6)); // Range: 3e6 to 5e6
            //     // size = 4e6;
            //     duration = size / 1e6; // Duration based on size
            // }

            size = pareto(1e6 , PP_ALPHA);
            duration = size / 1e6;

            t->add(new Instance(duration, size, t));
        }
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
        return create_fake_job();
    }
};