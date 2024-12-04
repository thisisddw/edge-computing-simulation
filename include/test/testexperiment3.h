/*
 * Created on Wed Dec 04 2024
 *
 * Filename: testexperiment3.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
  * Description: Test Q-Learning Better-Response Process With Inertia.
 */

#include "test/testagent.h"
#include "test/testexperiment.h"


class QLTestAgent : public TestAgent
{
public:
    static vector<int> joint_strategy, new_joint_strategy;

private:
    const int id;

    inline int action_size()
    {
        return action_space.size();
    }
    inline int joint_strategy_size()
    {
        int t = 1;
        for (int i = 0; i < N_USER; i++)
            t *= action_size();
        return t;
    }
    inline int encode_joint_strategy(vector<int> js)
    {
        int t = 0;
        for (int i = 0; i < N_USER; i++)
            t = t * action_size() + js[i];
        return t;
    }

    double eps, eta, alpha, q_init;
    vector<double> Q;   // Q table for joint strategy

public:
    QLTestAgent(int id) : TestAgent(), id(id) 
    {
        eps = 0.1;
        eta = 0.5;
        alpha = 0.9;
        q_init = 1e7;
     
        // initialize q value
        Q.resize(joint_strategy_size(), 0);
    }

    Action act() override
    {
        if (uniform_real(0, 1) < eps)
            new_joint_strategy[id] = (rand() * 1000 + rand()) % action_size();
        else if (uniform_real(0, 1) < eta)
            new_joint_strategy[id] = joint_strategy[id];
        else
        {
            vector<int> better_response_set;
            for (int a = 0; a < action_size(); a++)
            {
                vector<int> virtual_strategy = joint_strategy;
                virtual_strategy[id] = a;
                int js_id = encode_joint_strategy(joint_strategy);
                int vjs_id = encode_joint_strategy(virtual_strategy);
                if (Q[vjs_id] > Q[js_id])
                    better_response_set.push_back(a);
            }
            if (better_response_set.empty())
                new_joint_strategy[id] = joint_strategy[id];
            else
                new_joint_strategy[id] = better_response_set[(rand() * 1000 + rand()) % better_response_set.size()];
        }
        return action_space[new_joint_strategy[id]];
    }
    void feedback(Feedback fb) override
    {
        joint_strategy = new_joint_strategy;
        int js_id = encode_joint_strategy(joint_strategy);
        double u = calc_utilities(joint_strategy)[id];
        Q[js_id] += (1 - alpha) * (u - Q[js_id]);
    }
};

class TestExperiment3 : public TestExperiment {
public:
    TestExperiment3() : TestExperiment("Q-Learning_Better-Response")
    {
        QLTestAgent::joint_strategy.resize(N_USER, 0);
        QLTestAgent::new_joint_strategy.resize(N_USER, 0);
        for(int i = 0; i < N_USER; i++)
            agents[i] = new QLTestAgent(i);
    }
};
