#include "p2_threads.h"
#include "utils.h"
#include <assert.h>
#include <string>
extern Restroom room;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
extern struct timeval t_global_start;

using namespace std;


void *create(void *parm)
{
    int male_count = room.get_num();
    int female_count = room.get_num();
    struct timeval t_current;
    int total = 2 * room.get_num();
    
    
    srand( time(NULL) );
    
    int counter = 0; 
	while (total > counter) {
		
		// create new person 
        Person* p = new Person();
		// set random time in restroom
		int random_time = rand() % 8 + 3 ;
        p->set_time(random_time);
		// put in queue 
        p->set_order(counter+1);
        
		int gender_rand = rand() % 2 ; 
		// if there are no men
        if( ( male_count == 0) ||  (gender_rand == FEMALE && female_count > 0) ){
            female_count--; 
			p->set_gender(FEMALE);}
		
		// else decrement number of male and set person to male
        else{
			male_count--; 
            p->set_gender(MALE);
        }
        pthread_mutex_lock(&mutex_1);
		
		// push to wait list for the restroom 
        room.queue.push_back(*p);   
        pthread_mutex_unlock(&mutex_1);
		
		//sleep for interval between 1ms to 5ms 
        usleep(MSEC((rand()%5+1)));
		counter ++; 
    }
	// finish thread 
    pthread_exit(0);
    
}


void *assign(void *parm)
{
	int added = 0;
	vector<Person> temp;
    struct timeval t_current;
   
	int total = room.get_num();
    int add_total = total * 2  ;
    
      
    srand(time(NULL));
    
	// clear queue and send to temp vector 
	
    while(added != add_total){
        int counter =0 ; 
        while (counter < room.queue.size() ) {    
            pthread_mutex_lock(&mutex_1);
            temp.push_back(room.queue[0]);
            pthread_mutex_unlock(&mutex_1);
            
            gettimeofday(&t_current, NULL);
			long time = get_elasped_time(t_global_start, t_current) ; 
			string print_time ;
			print_time = '[' + time + " ms]" ; 
			cout << "[" << time << " ms]" ; 
			// if male enters 
            if(room.queue[0].get_gender() == MALE){
				room.queue[0].man_enter();}
			// else female enters 
			else if (room.queue[0].get_gender() == FEMALE){
                room.queue[0].woman_enter();
            }else{
				cout << "ENTER ERROR \n" ; }
            room.queue.erase(room.queue.begin());
			counter ++ ; 
        }
		// append person to list of people in the restroom 
		if (room.in_restroom.size() != room.get_stall() || temp.size() == 0) {
            while( temp.size() != 0 && room.person_allowed(temp[0]) ){
                
                pthread_mutex_lock(&mutex_2);
                gettimeofday(&t_current, NULL);
				string print_elasped ;
				long my_time =  get_elasped_time(t_global_start,t_current);
				cout << "[" << my_time  << " ms]";
				room.print_send(temp[0]); 
				room.add_person(temp[0]);	
				long my_time_2 = get_elasped_time(t_global_start,t_current );
				cout << "[" << my_time_2  << " ms]";
				room.person_enter(temp[0]);
				pthread_mutex_unlock(&mutex_2);     
				temp.erase( temp.begin() );
				added++;
				
            }
        } 
    }
  
    pthread_exit(0);
}

void *remove(void *parm)
{   
    struct timeval t_current;
 
    int total_remove = 2 * room.get_num();
    int removed = 0;
    
    while(removed != total_remove ){
        pthread_mutex_lock(&mutex_2);
		int counter = 0; 
		while (counter < room.in_restroom.size()) {
            if(room.in_restroom[counter].person_leave() ){
				Person dummy = room.in_restroom[counter];
                int gender_temp = room.in_restroom[counter].get_gender();
                gettimeofday(&t_current, NULL);
				long my_time = get_elasped_time(t_global_start, t_current) ;
                cout << "[" << my_time << " ms]";
                room.remove_person(counter);
                if(gender_temp == MALE){
					room.man_leave(dummy);}
                else if (gender_temp == FEMALE){
					room.woman_leave(dummy);}
				else{
					printf(" REMOVE ERROR\n"); 
				}
				counter--;
                removed++;
            }
			counter ++; 
        }
        pthread_mutex_unlock(&mutex_2);  
        
        if(room.in_restroom.size() == 0){    
            room.set_status(EMPTY);
        }
    }
    pthread_exit(0);
}

//EOF 