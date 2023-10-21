/*
 * Created on Fri Oct 20 2023
 *
 * Filename: testexperiment2.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Test full CSI best reply.
 */

#include "test/testagent.h"
#include "test/testexperiment.h"

bool strategy_computed;
vector<int> joint_strategy;

class CSITestAgent : public TestAgent
{
    const int id;
public:
    CSITestAgent(int id) : TestAgent(), id(id) {}
 
    Action act() override
    {
        if(!strategy_computed)
        {
            for(int i = 0; i < N_USER; i++)
                joint_strategy[i] = (rand()*100 + rand()) % action_space.size();
            for(int i = 0; i < MAX_ITER; i++)
            {
                int rand_id = rand() % N_USER;
                double utility = calc_utilities(joint_strategy)[rand_id];
                for(int j = 0; j < (int)action_space.size(); j++)
                {
                    auto virtual_strategy = joint_strategy;
                    virtual_strategy[rand_id] = j;
                    auto virtual_utilites = calc_utilities(virtual_strategy);
                    if(virtual_utilites[rand_id] > utility)
                        joint_strategy[rand_id] = j,
                        utility = virtual_utilites[rand_id];
                }
            }
            strategy_computed = true;
        }
        return action_space[joint_strategy[id]];
    }
    void feedback(Feedback fb) override
    {
        strategy_computed = false;
    }
};

class TestExperiment2 : public TestExperiment {
public:
    TestExperiment2() : TestExperiment("test-full-CSI")
    {
        strategy_computed = false;
        joint_strategy.resize(N_USER);
        for(int i = 0; i < N_USER; i++)
            agents[i] = new CSITestAgent(i);
    }
};
