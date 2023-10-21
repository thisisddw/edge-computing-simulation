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
#include "progressbar.h"

/**
 * @brief Perform experiments with settings in parameter.h.
 * @param experiments Experiment instances to be performed.
*/
void main_loop(vector<Experiment *> experiments)
{
    global_initialize();

    ProgressBar pb(50);

    for(int i = 0; i < N_SLOT; i++)
    {
        channelgains_update();
        for(auto e: experiments)
            e->step();
        pb.update((double)i / N_SLOT);
    }
    printf("\n");
}
