/*
 * Created on Sat Mar 09 2024
 *
 * Filename: job.cpp
 * Author: David Du
 * Email: thisisddw@gmail.com
 *
 * Description: Implementation of job.h
 */

#include "job.h"

void Instance::set_available() 
{ 
    assert(is_available() || is_pending()); 
    if(is_pending()) parent->cnt_p--, parent->cnt_a++;
    if(parent->first_available > location) parent->first_available = location;
    state = A; 
}

void Instance::set_pending() 
{ 
    assert(is_available() || is_pending());
    if(is_available()) parent->cnt_a--, parent->cnt_p++;
    state = P; 
    while(parent->first_available < parent->size() && !parent->instances[parent->first_available]->is_available())
        parent->first_available++;
}

void Instance::set_done() 
{ 
    assert(is_pending() || is_done());
    if(is_pending()) parent->cnt_p--, parent->cnt_d++;
    state = D; 
}

void Task::add(Instance *i)
{
    IF_DEBUG(task_integrity_check());
    assert(i->parent == this);

    if (first_available == size() && !i->is_available())
        first_available++;

    instances.push_back(i);
    i->location = instances.size() - 1;
    if (i->is_available()) cnt_a++;
    if (i->is_pending()) cnt_p++;
    if (i->is_done()) cnt_d++;
}