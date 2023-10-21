/*
 * Created on Fri Oct 20 2023
 *
 * Filename: testexperiment.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Base class for experiments in test scenerio. Keep
 * track with average utility.
 */

#pragma once

#include "experiment.h"

class TestExperiment : public Experiment {
public:
    vector<double> utility;
    TestExperiment(const char *name) : Experiment(name) {}

protected:
    /**
     * record average utility
    */
    void track() override
    {
        double u = 0;
        for(int i = 0; i < N_USER; i++)
        {
            double r = 0;   // total rate
            double p = 0;   // total power
            for(int j = 0; j < N_BS; j++)
                r += feedbacks[i].r[j],
                p += actions[i].p[j];
            double e = TTR * p; // energy
            double c = K_CPU * TTR * r; // cycle

            u += WEIGHT_F*c - WEIGHT_E*e;
        }
        u /= N_USER;
        if(!utility.empty())
            u = (u + (*utility.rbegin())*(utility.size())) / (utility.size() + 1);
        utility.push_back(u);
    }
};
