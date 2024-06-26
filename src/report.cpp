/*
 * Created on Mon Oct 23 2023
 *
 * Filename: report.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Generate a report in markdown format.
 */

#include "common.h"
#include "experiment.h"
#include "experiments/baseexp.h"
#include "matplotlibcpp.h"

#define EXPAND_AND_STRINGIZE(x) #x
#define MAKE_ROW(P, DESC) fprintf(f, "| %s | %s | %s |\n", #P, EXPAND_AND_STRINGIZE(P), DESC)

void report(const char *path, vector<Experiment *> exps)
{
    char buf[100];
    sprintf(buf, "%sreport.md", path);
    FILE *f = fopen(buf, "w");
    
    namespace plt = matplotlibcpp;

    // generate map of user and bs locations

    vector<double> x, y;
    for(int i = 0; i < N_USER; i++)
    {
        x.push_back(user_location[i].x), y.push_back(user_location[i].y);
        plt::text(x.back() + RANGE/50., y.back() + RANGE/50., "agent" + std::to_string(i));
    }
    plt::scatter(x, y, 10, {{"label", "user"}});
    x.clear(), y.clear();
    for(int i = 0; i < N_BS; i++)
    {
        x.push_back(bs_location[i].x), y.push_back(bs_location[i].y);
        plt::text(x.back() + RANGE/50., y.back() + RANGE/50., "BS" + std::to_string(i));
    }
    plt::scatter(x, y, 10, {{"label", "base station"}});
    plt::xlim(-RANGE*0.1, RANGE*1.1);
    plt::ylim(-RANGE*0.1, RANGE*1.1);
    plt::legend();
    sprintf(buf, "%slocation.png", path);
    plt::save(buf);

    // title
    fprintf(f, "# Simulation Report\n\n");
    
    time_t now = time(NULL);
    tm * lt = localtime(&now);

    fprintf(f, "Created on %d/%d/%d, %d:%d\n\n",
        lt->tm_mon + 1, lt->tm_mday, lt->tm_year + 1900, lt->tm_hour, lt->tm_min);

    //section 0
    fprintf(f, "## Flags\n\n");
#ifdef DEBUG
    fprintf(f, " - DEBUG\n");
#endif
#ifdef FAILURE_ON
    fprintf(f, " - FAILURE_ON\n");
#endif
#ifdef FAILURE_DETAIL
    fprintf(f, " - FAILURE_DETAIL\n");
#endif
#ifdef PLOT_AGENT
    fprintf(f, " - PLOT_AGENT\n");
#endif
    fprintf(f, "\n");

    // section 1
    fprintf(f, "## Environment Settings\n\n");

    fprintf(f, "### parameters\n\n");
    // make a table
    fprintf(f, "| parameter | value | description |\n");
    fprintf(f, "|-----------|-------|-------------|\n");
    MAKE_ROW(N_USER, "number of users");
    MAKE_ROW(N_BS, "number of BS");
    MAKE_ROW(RANGE, "users and BSs are scattered in [0,RANGE]*[0,RANGE]");
    MAKE_ROW(RICIAN_K, "K factor of Rician channel    10^0.6");
    MAKE_ROW(N_0, "noise");
    MAKE_ROW(W_SUB, "bandwidth");
    MAKE_ROW(P_MAX, "max transmission power");
    MAKE_ROW(N_LINK, "max number of established links of an agent");
    MAKE_ROW(F_INTERVAL, "average interval between failures");
    MAKE_ROW(F_DURATION, "the failure lasts F_DURATION seconds");
    MAKE_ROW(N_SLOT, "number of time slots");
    MAKE_ROW(TTR, "time of one transmit frame");
    MAKE_ROW(RANDOM_SEED, "random seed");
    fprintf(f, "\n");

    fprintf(f, "### locations of users and base stations\n\n");
    fprintf(f, "![location](location.png)\n\n");

    fprintf(f, "### server failures\n\n");

#ifdef FAILURE_DETAIL
    fprintf(f, "| server id | start (s) | end (s) | duration (s) |\n");
    fprintf(f, "|-----------|-----------|---------|--------------|\n");
    std::sort(server_failure_histroy.begin(), server_failure_histroy.end(), 
    [](FailureRecord x, FailureRecord y)->bool {
        if(x.sid != y.sid) return x.sid < y.sid;
        return x.start < y.start;
    });
    for(FailureRecord r: server_failure_histroy)
        fprintf(f, "|%d|%.3lf|%.3lf|%.3lf|\n", r.sid, r.start, r.end, r.duration);
    fprintf(f, "\n");
#endif

    // section 2
    fprintf(f, "## Experiment Results\n\n");

    // fprintf(f, "|terms|description|\n");
    // fprintf(f, "|-----|-----------|\n");
    // fprintf(f, "|GTR (gross transmission rate)| average transmission rate|\n");
    // fprintf(f, "|ATR (actual transmission rate)| (total size of executed instances) / time|\n");
    // fprintf(f, "|NEI (number of executed instances)| number of executed instances |\n\n");

    // each experiment has a subsection
    for(Experiment *e: exps)
    {
        fprintf(f, "### %s\n\n", e->get_name());

        ExpSummary s = ((BaseExperiment *)e)->summarize();
        fprintf(f, "|   |gross trans rate|actual trans rate|instances executed|\n");
        fprintf(f, "|---|----------------|-----------------|------------------|\n");
        for(auto &it: s)
        {
            fprintf(f, "|%s|%.2lf M|%.2lf M|%.0lf|\n", it.first.c_str(), 
                it.second.gross_trans / 1e6, it.second.actual_trans / 1e6, it.second.inst_done);
        }
        fprintf(f, "\n");

        auto img1 = ((BaseExperiment *)e)->plot_save(path);
        fprintf(f, "![](%s)\n\n", img1.c_str());

    #ifdef PLOT_AGENT
        auto imgs = ((BaseExperiment *)e)->plot_save_agents(path);
        for(auto img: imgs)
        {
            fprintf(f, "![](%s)\n", img.c_str());
        }
        fprintf(f, "\n");
    #endif
    }

    fclose(f);
}