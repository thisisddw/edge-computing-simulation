/*
 * Created on Fri Oct 20 2023
 *
 * Filename: test.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Test and compare with matlab version.
 */

#include "test/testagent.h"
#include "test/testexperiment1.h"
#include "test/testexperiment2.h"
#include "test/testexperiment3.h"
#include "progressbar.h"
#include "matplotlibcpp.h"

/* static members of TestAgent */

vector<Action> TestAgent::action_space;
bool TestAgent::initialized;
double TestAgent::power_level[3];
double TestAgent::dfs_state[N_BS], TestAgent::sump, TestAgent::n_bs;

bool CSITestAgent::strategy_computed;
vector<int> CSITestAgent::joint_strategy;
vector<int> QLTestAgent::joint_strategy, QLTestAgent::new_joint_strategy;

namespace plt = matplotlibcpp;

// extern vector<Experiment *> main_loop(vector<Experiment *> (*exp_generator)(), int repeat);
extern std::pair<std::vector<Experiment *>, std::vector<ExpSummary>> main_loop(vector<Experiment *> (*exp_generator)(), int repeat);

vector<Experiment *> perform_test()
{
    auto exp_generator = []() -> vector<Experiment *> {
        vector<Experiment *> exps = {
            new TestExperiment1(),
            new TestExperiment2(),
            new TestExperiment3(),
        };
        return exps;
    };

    global_initialize();

    ProgressBar pb(50);
    
    vector<Experiment *> experiments = exp_generator();
    server_initialize();
    // server_failure_initialize();
    // new_server_initialize();
    for(int i = 0; i < N_SLOT; i++)
    {
        current_time = i * TTR;
        channelgains_update();
    #ifdef FAILURE_ON
        server_state_update();
        // new_server_state_update();
    #endif
        for(auto e: experiments)
            e->step();
        pb.update((double)i / N_SLOT);
    }

    printf("\n");

    return experiments;
}