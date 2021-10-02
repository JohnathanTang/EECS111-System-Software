#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cfloat>
#include <string> 
#include <string.h> 

using namespace std; 

/* 
Student Struct
Students will have an ID and Grade 
*/ 
struct student_struct{
	long id;		
	double grade;	
	string grade_string;
	student_struct(long x, double y, string s){
		id = x;
		grade = y;
		grade_string = s;}
};

/*
Thread Struct 
Thread to handle sorting process 
*/
struct thread_struct{
	int thread_num; 	
	int lower; 		
	int upper;		
	vector <student_struct> *students; 
};

//****************** Function Declarations****************************

// **********************Handle IO files******************************
void do_thread(int num_threads, string file_name);
void create_output( vector<student_struct> student_vector, double avg_sum, int num_threads, string file_name );

//********************** Merge and sort threads****************************
void sort(vector<student_struct> *students, int array_size, int num_threads);
void *merge_thread(void *arg);

// *********************Merge Sort Algorithm ******************************
void merge(int lower, int mid, int upper, vector<student_struct> *students);
void merge_sort(int lower, int upper, vector<student_struct> *students);

// **********************Compare two student entries ***********************
int compare_students(student_struct s1, student_struct s2);

// **********************Compute stat values ********************************* 
double std_compute(double variance);
double variance_compute(vector<student_struct> array, double avg) ;
double avg_compute(double vector_size , double avg_sum );
double median_compute(vector<student_struct> student_vector);

#endif