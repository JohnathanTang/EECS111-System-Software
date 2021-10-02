#ifndef __P1_PROCESS
#define __P1_PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include "p1_threads.h"
#include <sys/wait.h>

using namespace std;

void get_statistics(string class_name[], int num_processes, int num_threads);

#endif