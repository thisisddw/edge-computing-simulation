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

void report(const char *path, vector<Experiment *> exps)
{
    char buf[100];
    sprintf(buf, "%sreport", path);
    FILE *f = fopen(buf, "w");
    
    for(Experiment *e: exps)
    {
        auto imgs = ((BaseExperiment *)e)->plot_save_agents(path);
        fprintf(f, "## %s\n\n", e->get_name());
        for(auto img: imgs)
        {
            fprintf(f, "![](%s)\n\n", img);
        }
    }

    fclose(f);
}