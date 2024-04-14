/*
 * Created on Fri Mar 22 2024
 *
 * Filename: tracker.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Track stats of Experiment.
 */

#pragma once

#include "experiment.h"
#include "matplotlibcpp.h"


class ExpTracker {
    Experiment *exp;

public:
    vector<int> n_links, n_working_server;
    vector<double> tot_trans_rate;

    ExpTracker(Experiment *exp) : exp(exp) {}

    void track()
    {
        int nl = 0, nws = 0;
        double ttr = 0;
        for (int i = 0; i < N_USER; i++)
            for (int j = 0; j < N_BS; j++)
                nl += exp->actions[i].p[j] > 0,
                ttr += exp->feedbacks[i].r[j];
        for (int j = 0; j < N_BS; j++)
            nws += exp->feedbacks[0].a[j];
        n_links.push_back(nl);
        n_working_server.push_back(nws);
        tot_trans_rate.push_back(ttr);
    }
    void plot_save(const char *path, const char *title = "Default Title")
    {
        namespace plt = matplotlibcpp;

        plt::suptitle(title);

        plt::subplot2grid(2, 1, 0, 0);
        plt::named_plot("total rate", tot_trans_rate);
        plt::legend();

        plt::subplot2grid(2, 1, 1, 0);
        plt::named_plot("links", n_links);
        plt::named_plot("working servers", n_working_server);
        plt::legend();

        plt::save(path);
    }
};