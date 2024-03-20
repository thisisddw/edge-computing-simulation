/*
 * Created on Wed Feb 28 2024
 *
 * Filename: basictypes.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Definitions of simple types.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::pair;

/**
 * @brief Serve as coordinates of user and BS.
*/
struct Point2D {
    double x;
    double y;
};

/**
 * @brief Record entry of server failure.
*/
struct FailureRecord {
    int sid;
    double start;
    double end;
    double duration;
};

/**
 * @brief Statistical summary of agents
*/
struct AgentSummary {
    double gross_trans;       // average transmission rate
    double actual_trans;    // (total size of executed instances) / time
    double inst_done;       // number of executed instances
};

/**
 * @brief Summary of experiments, including summary of each agent and total numbers
*/
typedef map<string, AgentSummary> ExpSummary;