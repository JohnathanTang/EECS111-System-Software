#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

struct timeval t_global_start;


using namespace std;

// declare restroom 
Restroom room;
int main(int argc, char** argv)
{
	// initialize how many of each gender generated 
    int gender_count = 0; 
    
	// make sure user only inputs 1 additional argument to command line
    if(argc != 2){   
		printf("[ERROR] Expecting 1 argument, but got (%d).\n", argc - 1); 
        printf("[USAGE] p2_exec <number>\n") ; 
        return 0;
        
    }else{
		// pass user input to program routine
		gender_count = atoi(argv[1]);
		room.set_num(gender_count);
		room.set_stall(gender_count);
    }
    
	gettimeofday(&t_global_start, NULL);

	// we have three threads
	pthread_t       tid[2];
    
	// creation thread
    if(pthread_create(&tid[0], NULL, create, NULL)) {
        fprintf(stderr, "Error creating thread\n");		
    }
	// assignment thread
    if(pthread_create(&tid[1], NULL, assign, NULL)) {
        fprintf(stderr, "Error creating thread\n");		
    }
	// removal thread
    if(pthread_create(&tid[2], NULL, remove, NULL)) {
        fprintf(stderr, "Error creating thread\n");		
    }	  
     
    for(int i=0; i < 3; i++){
        if(pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Error joining thread\n");	
        }
    }  
    
	return 0;
    
}
// EOF 