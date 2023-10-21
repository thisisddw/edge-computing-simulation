/*
 * Created on Fri Oct 20 2023
 *
 * Filename: testexperiment1.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Test random strategy.
 */

#pragma once

#include "test/testagent.h"
#include "test/testexperiment.h"

class RandomTestAgent : public TestAgent {
public:
    Action act() override
    {
        return action_space[(rand()*100 + rand()) % action_space.size()];
    }
    void feedback(Feedback fb) override
    {

    }
};

class TestExperiment1 : public TestExperiment {
public:
    TestExperiment1() : TestExperiment("test-random")
    {
        for(int i = 0; i < N_USER; i++)
            agents[i] = new RandomTestAgent();
    }
};