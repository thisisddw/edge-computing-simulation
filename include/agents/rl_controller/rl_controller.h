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
 * to be used in AdaptiveAgent.
*/
class RlController {
protected:
    const int n_action;     // action space is [0, n_action)
    const int n_state;      // state space is [0, n_state)
    int state;              // current state
public:
    RlController(int action_space_size, int state_space_size, int init_state = 0)
         : n_action(action_space_size), n_state(state_space_size), state(init_state) {}
    virtual ~RlController() {};
    virtual int act() = 0;
    virtual void feedback(double reward, int next_state) = 0;
};

/**
 * @brief RlController with a single state, therefore only a double type 
 * reward is required in feedback. Notably, this is the base class for MultiarmedBandit.
*/
class SingleStateRlController : public RlController {
public:
    SingleStateRlController(int action_space_size) : RlController(action_space_size, 1) {}
    virtual void feedback(double reward) = 0;
    /**
     * @param next_state This parameter would be neglected.
    */
    virtual void feedback(double reward, int next_state) override final { return feedback(reward); }
};

/**
 * @brief Make constant action.
*/
class TrivialController : public SingleStateRlController {
public:
    TrivialController(int n_link) : SingleStateRlController(n_link) {}

    int act() override
    {
        return n_action - 1;
    }
    void feedback(double reward) override {}
};

class CyclicController : public SingleStateRlController {
    int last;
public:
    CyclicController(int n_link) : SingleStateRlController(n_link) { last = n_link - 1; }

    int act() override
    {
        last = (last + 1) % n_action;
        return last;
    }
    void feedback(double reward) override {}
};