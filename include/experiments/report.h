/*
 * Created on Mon Oct 23 2023
 *
 * Filename: report.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Generate a report in markdown format.
 */

#pragma once

#include "common.h"
#include "experiment.h"

void report(const char *path, vector<Experiment *> exps);