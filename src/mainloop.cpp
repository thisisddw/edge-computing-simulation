/*
 * Created on Fri Oct 20 2023
 *
 * Filename: mainloop.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Perform experiments in the main loop.
 */

#include "common.h"
#include "experiment.h"

/**
 * @brief Perform experiments with settings in parameter.h.
 * @param experiments Experiment instances to be performed.
*/
void main_loop(vector<Experiment *> experiments)
{
    global_initialize();
    for(int i = 0; i < N_SLOT; i++)
    {
        channelgains_update();
        for(auto e: experiments)
            e->step();
        // if(i % (N_SLOT/100) == 0)
        //     printf("%d\n", i);
    }
}