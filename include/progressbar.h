/*
 * Created on Sat Oct 21 2023
 *
 * Filename: progressbar.h
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Progress bar.
 */

#pragma once

#include "common.h"

class ProgressBar {
    #define GAP (CLOCKS_PER_SEC / 3)
    clock_t start_time, last_draw;
    const int width, length;

    std::string time_format(int sec)
    {
        static char buf[30];
        sprintf(buf, "%3dm%3ds", sec/60, sec%60);
        return std::string(buf);
    }

    void draw(double progress)
    {
        assert(progress >= 0. && progress <= 1.);

        int pos = width * progress;

        printf("[");
        for(int i = 0; i < width; i++)
        {
            if(i < pos) printf("=");
            if(i == pos) printf(">");
            if(i > pos) printf(" ");
        }
        printf("]");

        double elapsed = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        double remaining = elapsed / (progress + 1e-6) - elapsed;
        printf(" Elapsed: %s, Remaining: %s", time_format((int)(elapsed + 0.5)).c_str(),
         time_format((int)(remaining + 0.5)).c_str());
    }

    void erase()
    {
        printf("\r");
        for(int i = 0; i < length; i++)
            printf(" ");
        printf("\r");
    }

public:
    ProgressBar(int width) : width(width), length(width + 2 + 27 + 16) 
    {
        start_time = clock();
        last_draw = start_time - GAP;
    }
    void update(double progress)
    {
        if(clock() - last_draw < GAP) 
            return;
        last_draw = clock();
        erase();
        draw(progress);
    }
};