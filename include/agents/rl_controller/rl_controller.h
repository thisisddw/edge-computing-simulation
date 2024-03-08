/*
 * Created on Thu Mar 07 2024
 *
 * Filename: rl_controller.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Interface of reinforement learning controller, which
 * is intended to be used in AdaptiveAgent.
 */

#pragma once

#include "common.h"

/**
 * @brief Interface of reinforement learning controller, which is intended 
 * to be used in AdaptiveAgent. It is supposed to have discrete action space
 * and a single state, therefore only a double type reward is required in feedback.
*/
class RlController {
protected:
    const int n_action;     // action space is [0, n_action)
public:
    RlController(int action_space_size) : n_action(action_space_size) {}
    virtual ~RlController() {};
    virtual int act() = 0;
    virtual void feedback(double reward) = 0;
};

/**
 * @brief Make constant action.
*/
class TrivialController : public RlController {
public:
    TrivialController(int n_link) : RlController(n_link) {}

    int act() override
    {
        return n_action - 1;
    }
    void feedback(double reward) override {}
};

class CyclicController : public RlController {
    int last;
public:
    CyclicController(int n_link) : RlController(n_link) { last = n_link - 1; }

    int act() override
    {
        last = (last + 1) % n_action;
        return last;
    }
    void feedback(double reward) override {}
};