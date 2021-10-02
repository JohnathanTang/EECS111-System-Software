#include "types_p2.h"
#include "utils.h"

using namespace std;

// set and get order of person in queue 
void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

// set and get gender of person 
void Person::set_gender(int data) { gender = data; }
int Person::get_gender(void)      { return gender; }

// set and get elapsed time of person 
void Person::set_time(long data)        { time_to_stay_ms = data; }
unsigned long Person::get_time(void)    { return time_to_stay_ms; }

// set and get order of person in restroom
void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

// set and get number of male and/or female in restroom
void Restroom::set_num(int data) { num = data; }
int Restroom::get_num(void)      { return num; }

// start and complete ( person in restroom ) 
void Person::start(void)    { gettimeofday(&t_start, NULL);}
void Person::complete(void) { gettimeofday(&t_end, NULL);}

// set and get current status of restroom (MenPresent, WomenPresent, or Empty) 
void Restroom::set_status(int data) { status = data; }
int Restroom::get_status(void)     { return status; }

void Restroom::set_stall(int data) { stall = data; }
int Restroom::get_stall(void)      { return stall; }



Person::Person() {
	gettimeofday(&t_create, NULL);
}


int Person::person_leave(void) 
{
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);
	int good = 1 ; 
	int not_good = 0; 

	long time = get_elasped_time(t_start, t_curr); 
	if (time_to_stay_ms <= time) { 
		return good; 
	}else { 
		return not_good; }
}

void Person::woman_enter()
{    
	// get gender of person entering, print accordingly 
	string g = ""; 
	if (gender == 0){
		g = "Man";
	}else{
		g = "Woman" ;
	}
    cout << "[Input] A person ("  << g <<  ") goes into the queue\n";
}

void Person::man_enter(){
    // get gender of person entering , print accordingly 
    string g = ""; 
	if (gender == 0){
		g = "Man";
	}else{
		g = "Woman" ;
	}
    cout << "[Input] A person ("  << g <<  ") goes into the queue\n";
}

void Restroom::print_status(void) 
{
	// print status of restroom , 0 = empty, 1 = womenpresent, 2 = menpresent
	string stat = ""; 
	switch (status) {
		case 0: 
			stat = "Empty\n" ; 
			break; 
		case 1: 
			stat = "WomenPresent\n"; 
			break; 
		case 2: 
			stat = "MenPresent\n" ; 
			break;
		default: 
			break; 
	}
	// print status 
    cout << stat; 
}


void Restroom::add_person(Person& p) 
{
	// incremenet and append person to restroom 
	in_restroom.push_back(p);
    in_restroom[in_restroom.size()-1].start();
    
	// if person is female, increment number of women in restroom
    if(p.get_gender() == FEMALE){
        num_women++;
    }
	// if person is male, increment number of men in restroom 
    else{
        num_men++;
    }
	// increment total number of people in restroom, regardless of gender
    total_people++;
	// update status 
    change_status(p);
}
   

void Restroom::change_status(Person& p)
{
    int temp_status = status;

    if(status == EMPTY) {
        if(p.get_gender() == MALE){
            status = MENPRESENT;
        } else if (p.get_gender() == FEMALE){
            status = WOMENPRESENT;
        }
    }
	if(in_restroom.size() == 0){
        status = EMPTY;
    }
    
    // if previous status doesn't equal updated, something happened
	// update status 
    if (temp_status != status){
        status_changed = true;
    }else{
	status_changed = false;}
}


void Restroom::remove_person(int i)
{
	// if person leaving restroom is female, decrement count of women in restroom 
    if(in_restroom[i].get_gender() == FEMALE){
		num_women--;}
	// if person leaving restroom is male, decrement coutn of men in restroom
    else{
		num_men--;}
	// man or woman, we need to decrement total people in restroom 
    total_people--;
	// remove person from restroom and update status accordingly 
    in_restroom.erase(in_restroom.begin() + i);
    change_status(in_restroom[i]);
}

bool Restroom::person_allowed(Person& p)
{
	// check if person , based on gender is allowed in the restroom 
	int capacity = stall ; 
    if( in_restroom.size() == capacity ){
        return false; }

	// if the person entering is man and there is a man present, person is allowed
	if (p.get_gender() == 0 && status == MENPRESENT) {
		return (p.get_gender() == 0 && status == MENPRESENT) ;
	// if the person entering is woman and there is a woman present, person is allowed
	} else if (p.get_gender() == 1 && status == WOMENPRESENT) {
		return (p.get_gender() == 1 && status == WOMENPRESENT) ;
	// if the restroom if empty, man or woman is allowed 
	} else {
		return (status == EMPTY ); 
	} 
          
}


void Restroom::print_send(Person &p){
    
	// Get gender of who is sent to restroom 
	// set gender string accordingly 
	string gender = ""; 
	if (p.get_gender() == 0 ){
		gender = "Man"; 
	}else {
		gender = "Woman" ; 
	}
	
	// Print who is sent to restroom (man or woman ) and status of queue) 
    cout << "[Queue] Send (" << gender  <<  ") into the restroom" << " (stay " << p.get_time() << " ms), Status:" ;
    print_total(total_people, num_men, num_women); 
}

void Restroom::person_enter(Person &p){
		
	// get gender of person (man or woman ) entering the restroom 
	string gender = ""; 
	if (p.get_gender() == 0 ){
		gender = "Man"; 
	}else {
		gender = "Woman" ;
	}
	// print who is entering the restroom (man or woman) and status of restroom 
    cout << "[Restroom] (" << gender  << ") goes into the  restroom (Stay " << p.get_time() 
            << "ms), ";
    print_total(total_people, num_men, num_women); 
}

void Restroom::man_leave(Person &p)
{    
   // get gender of person leaving 
	// set gender string accordingly 

    string gender = ""; 
	if (p.get_gender() == 0 ){
		gender = "Man"; 
	}else {
		gender = "Woman" ; 
	}
	
	// get status of restroom (Man Present, Woman Present, Empty)
	// set status string accordingly 
    string status_string = ""; 
    
    if(status == MENPRESENT)
        status_string = "ManPresent";
    else if (status == WOMENPRESENT)
        status_string = "WomanPresent";
    else
        status_string = "Empty";
	
	 // Print who has left the restroom (Man or Woman ) 
    
    cout << "[Restroom] (" << gender  << ") left the restroom. ";
    
	// If status_changed is false, print same status 
	// else update and print status of restroom 
    if(status_changed == false){
		cout << "State is (" << status_string << "): ";}        
    else{
        cout << "Status is changed, Status is (" << status_string << "): ";}
		
    print_total(total_people, num_men, num_women); 
}  

void Restroom::woman_leave(Person &p)
{
	// get gender of person leaving 
	// set gender string accordingly 

    string gender = ""; 
	if (p.get_gender() == 0 ){
		gender = "Man"; 
	}else {
		gender = "Woman" ; 
	}
	
	// get status of restroom (Man Present, Woman Present, Empty)
	// set status string accordingly 
    string status_string = ""; 
    
    if(status == MENPRESENT)
        status_string = "ManPresent";
    else if (status == WOMENPRESENT)
        status_string = "WomanPresent";
    else
        status_string = "Empty";
	
	 // Print who has left the restroom (Man or Woman ) 
    
    cout << "[Restroom] (" << gender  << ") left the restroom. ";
    
	// If status_changed is false, print same status 
	// else update and print status of restroom 
    if(status_changed == false){
		cout << "State is (" << status_string << "): ";}        
    else{
        cout << "Status is changed, Status is (" << status_string << "): ";}
		
    print_total(total_people, num_men, num_women); 
}    

// print statistic of people in restroom (total, number of men, and number of women ) 
void print_total(int total_people, int num_men, int num_women){
	cout << "Total: " << total_people << " (Men: " << num_men << " Women: " << num_women << ")\n";
}

// EOF
