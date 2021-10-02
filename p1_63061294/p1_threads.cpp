#include "p1_threads.h"

using namespace std; 


/* 
Create and write to output files "_sort" and "_stats" after vectors are sorted and 
values are computed.  
*/
void create_output( vector<student_struct> student_vector, double avg_sum, int num_threads, string file_name ){
	
	// Declare file streams for output 
	fstream f_out_sorted, f_out_stats; 
	string file_sorted = "output/" + file_name + "_sorted"+".csv";
	string file_stat = "output/" + file_name + "_stats"+".csv";
	cout << "\nCREATED OUTPUT FILE NAMES\n"; 
	// Sort student vectors 
	cout << "\nSORTING\n"; 
	sort(&student_vector,student_vector.size(), num_threads);
	cout << "\nSORTED" ; 
	f_out_sorted.open(file_sorted.c_str(), ios::out);
	cout << "\nCREATING _SORTED \n"; 
	if(f_out_sorted.fail()){
		cout << "Failed to Create: " << file_sorted << "\n";}
	else{
		// Write to "_sort" files 
		cout << "\nWRITING TO SORT FILE\n" ; 
		f_out_sorted << "Rank,Student ID,Grade\n";
		for(int i = 1; i <= student_vector.size() ; i++){
			long int student_id = student_vector[i].id ; 
			string student_grade = student_vector[i].grade_string ; 
			string comma = "," ; 
			//string sorted_string = "," + student_id + "," + student_grade + "\n";
			string sorted_string = "" ; 
			sorted_string += comma; 
			sorted_string += student_id ; 
			sorted_string += comma; 
			sorted_string += student_grade; 
			sorted_string += "\n" ; 
			f_out_sorted << (i+1) << sorted_string;}
		}	
	// Compute values for stats 
	double avg = avg_compute(student_vector.size(), avg_sum) ; 
	double std_dev = variance_compute(student_vector, avg) ; 
	double median = median_compute(student_vector) ; 
	
	// Write to "_stats" files 
	f_out_stats.open(file_stat.c_str(), ios::out);
	if(f_out_stats.fail()){
		cout << "Failed to Create:  " << file_stat << "\n";}
	else{
	f_out_stats << "Average,Median,Std. Dev\n";
	f_out_stats << setprecision(3) << fixed << avg << "," << median << "," << std_dev; }
}

/*
Merge vectors handled by threads 
*/

void *merge_thread(void *arg) {
	
	thread_struct *my_thread = (thread_struct*)arg;
	vector<student_struct> *students;
	int lower = my_thread->lower;
	int upper = my_thread->upper;
	students = my_thread->students;

	if(upper > lower) {
		merge_sort(lower, (lower + (upper - lower) / 2), students);
		merge_sort((lower + (upper - lower) / 2) + 1, upper,students);
		merge(lower, (lower + (upper - lower) / 2), upper, students);}
}

/* 
Handle input files and read data line by line
Push data to student vector 
*/
void do_thread(int num_threads, string file_name){
	
	// Declare file streams for input files 
	fstream file_in; 
	file_in << setprecision(15);
	string input_file = "input/" + file_name + ".csv";
	
	file_in.open(input_file.c_str(), ios::in);
	if(file_in.fail()){
		cout << "Failed to Open: " << file_name << endl; }
	else{
		// Declare student vectors to store data from files 
		vector< student_struct> student_vector;
		string data, line ;  
		double avg_sum = 0 ;
		vector<string> row;

		// Read file line by line and push to student_vector
		while( getline(file_in, line) ) { 
			row.clear();
			stringstream s(line);
			s << setprecision(15);
			while(getline(s,data,',')) {
				row.push_back(data);}
				
			const char* student_grade_conv = row[1].c_str() ;
			const char* student_id_conv = row[0].c_str() ; 			
			double student_grade = atof(student_grade_conv);	
			long student_id = atol(student_id_conv);
			
			// accumulate grade to find average
			avg_sum += student_grade;
			// create new temp student entry before pushing to vector 
			student_struct temp_student = student_struct(student_id, student_grade, row[1]);
			student_vector.push_back(temp_student);
		}
		// Create output files 
		create_output(student_vector, avg_sum, num_threads, file_name); 
	}
}

/*
create and merge new vectors left and right 
use MergeSort algorithm 
*/
void merge(int lower, int mid, int upper, vector<student_struct> *students){
	int num_1 = mid - lower + 1;
	int num_2 = upper - mid;
	
	// Declare indexes for sub-vectors 
	int k = lower;
	int x = 0;
	int y = 0;
	
	vector<student_struct> left;
	for(int i = 1; i <= num_1 ; i++){
		left.push_back((*students)[i+lower]);}
		
	vector<student_struct> right;
	
	for(int i = 1; i <= num_2 ; i++){
		right.push_back((*students)[i+mid+1]);}

	while( num_1 > x  && num_2 > y){
		if( compare_students(left[x],right[y]) == 1) {
			(*students)[k] = left[x];
			x++;
			k++;
		}else {
			(*students)[k] = right[y];
			k++;
			y++;}
	// Push remaining elements of "left" vector		
	}while( num_1 > x ) {
		(*students)[k] = left[x];
		x++;
		k++;
	// Push remaining elements of "right" vector 
	}while( num_2 > y ) {
		(*students)[k] = right[y];
		k++;
		y++;}
}

/*
Create threads and assign to sorting algorithm 
*/

void sort(vector<student_struct> *students, int array_size, int num_threads){

	thread_struct *my_thread;

	pthread_t *thread_array;
	thread_struct *thread_struct_array ;

	int l = array_size / num_threads;
	int remainder = array_size - l * num_threads;

	int lower = 0;
	for(int i = 1 ; i <= num_threads ; i++){
		my_thread = &thread_struct_array[i];
		my_thread->thread_num = i;
		if(i == num_threads - 1){
			my_thread->students = students;
			my_thread->lower = i * l;
			my_thread->upper = (i+1) * l - 1 + remainder;
			
		}else{
			my_thread->students = students;
			my_thread->lower = i * l;
			my_thread->upper = (i+1) * l - 1;}
	}

	for(int i = 1 ; i <= num_threads ; i++){
		my_thread = &thread_struct_array[i];
		pthread_create(&thread_array[i], NULL, merge_thread, my_thread);}

	for(int i = 1 ; i <= num_threads ; i++) {
		pthread_join(thread_array[i], NULL);}
        
	thread_struct *temp_thread_1 = &thread_struct_array[0];
	for(int i = 1; i < num_threads ; i++){
		thread_struct *temp_thread_2 = &thread_struct_array[i];
		merge(temp_thread_1->lower, temp_thread_2->lower - 1, temp_thread_2->upper, students);}
}

/*
Merge sorted vectors ( first and second halves ) 
*/
void merge_sort(int lower, int upper, vector<student_struct> *students){
	if(upper > lower) {
		merge_sort(lower, (lower + (upper - lower) / 2), students);
		merge_sort((lower + (upper - lower) / 2)+1, upper,students);
		merge(lower, (lower + (upper - lower) / 2), upper,students);
	}
}

/*
Compare two student entries
If student 1's grade is less than student 2's return 1 
else return 0 
*/
int compare_students(student_struct student1, student_struct student2){
	if(student2.grade < student1.grade){
		return 1;
	}else if ( student2.grade == student1.grade ){
		return 1; 
	}else{ 
		return 0;}
}

/*
Compute standard deviation 
*/
double std_compute (double variance) {
	return sqrt(variance) ; 
}

/*
Compute variance 
*/
double variance_compute(vector<student_struct> student_vector, double avg) {
	double variance = 0 ; 
	for (int i = 1; i <= student_vector.size() ; i++) {
	variance += pow(student_vector[i].grade - avg, (double)2); }
	variance = variance / student_vector.size() ; 
	double std = std_compute(variance) ; 
	return std; 
}

/*
Compute median 
*/
double median_compute(vector<student_struct> student_vector){
 	if(student_vector.size() / 2 != 0){
		return (student_vector[ student_vector.size() / 2 ].grade) ;
	}else{
		return ((student_vector[ student_vector.size() / 2].grade + student_vector[ student_vector.size() / 2 - 1].grade) / 2);}
}

/*
Compute average grade 
*/ 
double avg_compute(double vector_size , double avg_sum ) {
	return (avg_sum / vector_size) ;
}	