/*
 * Created on Thu Oct 19 2023
 *
 * Filename: message.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Message entities passed between agents and evironment.
 */

#pragma once

#include "common.h"

/**
 * @brief Agents make an action at the begining of each slot.
*/
struct Action
{
    double p[N_BS]; // power to each BS

    Action() { memset(p, 0, sizeof(p)); }
};

/**
 * @brief Agents receive a feedback at the end of each slot.
*/
struct Feedback
{
    double r[N_BS]; // transmit rate to each BS
    double i[N_BS]; // aggregate information
    bool a[N_BS];   // whether BSs are available

    Feedback() { memset(this, 0, sizeof(Feedback)); }
};
