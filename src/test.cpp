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
#include "matplotlibcpp.h"


/* static members of TestAgent */

vector<Action> TestAgent::action_space;
bool TestAgent::initialized;
double TestAgent::power_level[3];
double TestAgent::dfs_state[N_BS], TestAgent::sump, TestAgent::n_bs;


namespace plt = matplotlibcpp;

extern void main_loop(vector<Experiment *> experiments);

void perform_test()
{
    vector<Experiment *> exps = {
        new TestExperiment1(),
        new TestExperiment2(),
    };
    main_loop(exps);

    for(int i = 0; i < (int)exps.size(); i++)
    {
        TestExperiment *e = (TestExperiment *)exps[i];
        plt::plot(e->utility, {{"label", e->get_name()}});
        // for(auto d: e->utility)
        //     printf("%.2lf ", d);
        // printf("\n");
    }
    plt::legend();
    plt::show();
}