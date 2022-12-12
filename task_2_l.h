#ifndef _3_TASK_2_L_H
#define _3_TASK_2_L_H

#include <cstdio>
#include <random>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include <vector>

pthread_mutex_t mutex;
int ret_index = -1;
std::vector<int> ret_indx;

struct args
{
    std::vector<int> array;
    int search_value;
    int start;
    int end;
} ARG;

#endif //_3_TASK_2_L_H
