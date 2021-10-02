#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <assert.h>

#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2

using namespace std;

// For this program we will need two essential classes 
// Person and Restroom 
// Each person will have a gender, place in queue, and time needed to use restroom
// Restroom data structure holds statistics including number of each gender in restroom 
// while managing status (MenPresent, WomenPresent, or Empty) 

// Person class 
class Person
{
	// Declare "person" data structure
	// including gender, order, and time stay in restroom 
	string str_gender;	
	unsigned long order, use_order; 
	int gender;
	long time_to_stay_ms;
	struct timeval t_create; struct timeval t_start; struct timeval t_end; 
	
	
	//Person class with prototype functions 
public:
	Person();

	// Set and get gender of person 
	void set_gender(int data);
	int get_gender(void);

	// Set and Get oder of person in queue 
	void set_order(unsigned long data);
	unsigned long get_order(void);

	// Set and get order of person in restroom 
	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	// set and get elapsed time of person 
	void set_time(long data);
    unsigned long get_time(void);
    
	//start and complete in restroom 
	void start(void);
	void complete(void);
	
	//Ready_to_leave 
	int person_leave(void);
	
	//Women or Men enter restroom 
	void woman_enter();
    void man_enter();
    
    
};

// Restroom clas
class Restroom {
	bool status_changed; 
    int status, num, stall, num_men, num_women, total_people; 
	
public:
	Restroom(){
		// Restroom has status , number of men and women, total number of people 
		num_men = 0;
        num_women = 0;
        total_people = 0;
        status_changed = false;
		status = EMPTY;
        
	}
	
	//Person enter or leaves restroom
    void woman_leave(Person &p);
	void man_leave(Person &p);
    void person_enter(Person &p);
    
	// Number of people in restroom
    void set_num(int num);
    int get_num(void);
    
	// Queue of restroom
    void set_stall(int stall);
    int get_stall(void);
    
	// Status of restroom
    void set_status(int status);
    int get_status(void);
    
	// Print status of restroom (MenPresent, WomenPresent, Empty) 
	void print_status(void);

	// Change status of restroom after update 
	void change_status(Person& p);
    
	// Allow person to restroom 
    void add_person(Person& p);

	// Person leaves restroom
    void remove_person(int i);
    
	// Is person allowed to restroom based on gender of previous person
    bool person_allowed(Person& p);
    
	// If person is of same gender, send next 
    void print_send(Person &p);
    
	// Two vectors to store people in queue and keep track of who is in restroom 
	vector<Person> queue;
    vector<Person> in_restroom;
	
};

// Enum data structure for gender for easier programming
// 0 = Male and 1 = Female 
enum Gender{
    MALE, FEMALE
};

// Print statistics of people in restroom 
void print_total(int total_people, int num_men, int num_women);

#endif

