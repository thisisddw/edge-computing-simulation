/*
 * Created on Fri Oct 20 2023
 *
 * Filename: agent.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Base class for agents.
 */

#pragma once

#include "message.h"

class Agent {
public:
    virtual ~Agent() {}

    virtual Action act() = 0;
    virtual void feedback(Feedback fb) = 0;
};