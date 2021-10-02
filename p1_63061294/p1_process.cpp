#include "p1_process.h"

using namespace std; 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string> 
#include <unistd.h>

#define FILE_COUNT 5

void get_statistics(string class_name[], int num_processes, int num_threads) {
	
	// Dynamically allocate memory based on number of processes from user
	pid_t *processes = (pid_t*)malloc(num_processes * sizeof(pid_t) );	
	int status; 
		
	// Create Child Processes
	for (int i = 0; i < num_processes ; i++ ){
		int pid = fork() ; 
		if(pid == 0) {
			// For every child created from fork, print and track pid 
			printf("Child process is created. (pid: %d)\n", getpid());
			processes[i] = getpid(); 
			int process_counter = i; 
			do{
				// Call main thread handler to handle process threading 
				do_thread( num_threads,class_name[process_counter]); 
				process_counter += num_processes; 
			}while(process_counter < FILE_COUNT ) ;
			exit(0); 
		}
	}
			
	// Terminate Child Processes
	for (int i = 0; i < num_processes; i++) {
		
		pid_t child_process = waitpid(processes[i], &status, 0 ) ; 
		printf("Child process is terminated. (pid: %d)\n", child_process); }
	// Free Processes 
	free(processes) ; 	
}
