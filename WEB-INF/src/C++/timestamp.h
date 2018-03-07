#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

//Checks if the time entered by user is valid
bool is_time_valid(int hour,int min){
	if(hour >= 0 && hour < 24 && min >= 0 && min < 60)
		return true;
	return false;
}

//Gets time from user
void get_time(int *hour,int *min) {
	while(true) {
		scanf("%d %d",hour,min);
		if(is_time_valid(*hour,*min))
			return;
		cout<<"Enter the time in 24 hour format correctly hh(0-23) mm(0-59)\n";
	}
}

//Gets the current timestamp value
const tm get_timestamp() {
    time_t now = time(0);
    struct tm tstruct; 
    tstruct = *localtime(&now);
	return tstruct;
}

//Returns the timestamp as string
string get_timestamp_string(struct tm tstruct) {
	char buf[80];
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;
}

//Checks if the timestamp is in between the start and stop times
bool is_timestamp_in_range(int start_hour,int start_min,int stop_hour,int stop_min,tm timestamp) {
	if(timestamp.tm_hour >= start_hour && timestamp.tm_hour <= stop_hour){
		if(timestamp.tm_hour == start_hour && timestamp.tm_min < start_min)
			return false;
		if(timestamp.tm_hour == stop_hour && timestamp.tm_min >= stop_min)
			return false;
		return true;
	}
	return false;
}

//Checks if first time is less than or equal to second time
bool is_time_less_than_equal(int start_hour, int start_min, int stop_hour, int stop_min) {
	if(start_hour <= stop_hour) {
		if(start_hour == stop_hour) 
			if(start_min > stop_min)
				return false;
		return true;
	}
	return false;
}

//Checks if first time is less than second time
bool is_time_less_than(int start_hour, int start_min, int stop_hour, int stop_min) {
	if(start_hour <= stop_hour) {
		if(start_hour == stop_hour) 
			if(start_min >= stop_min)
				return false;
		return true;
	}
	return false;
}

//Checks if first timestamp is less than second
bool is_timestamp_less_than(tm time1,tm time2) {
	return is_time_less_than(time1.tm_hour,time1.tm_min,time2.tm_hour,time2.tm_min);
}

//Checks if first timestamp is equal to second
bool is_timestamp_equal(tm time1,tm time2) {
	return ((time1.tm_hour == time2.tm_hour) && (time1.tm_min == time2.tm_min ));
}

bool is_timestamp_less_than_equal(tm time1,tm time2) {
	return (is_timestamp_equal(time1,time2) || is_timestamp_less_than(time1,time2));
}

tm add_minutes_to_timestamp(tm time,int minutes) {
	int mins = time.tm_min;
	time.tm_hour += ((mins + minutes)/60);
	time.tm_min = (mins + minutes)%60;
	return time;
}


tm get_timestamp_from_string(string time_str) {
	time_t timee = time(0);
	tm time_struct = *localtime(&timee);
	time_struct.tm_sec = 0;
	size_t pos = 0;
	string delimiter = ":";
	string time_string = time_str;
	pos = time_string.find(delimiter);
	time_struct.tm_hour = stoi(time_string.substr(0, pos));
	time_string.erase(0, pos + delimiter.length());
	pos = time_string.find(delimiter);
	time_struct.tm_min = stoi(time_string.substr(0, pos));
	time_string.erase(0, pos + delimiter.length());
	pos = time_string.find(delimiter);
	time_struct.tm_sec = stoi(time_string.substr(0, pos));
	return time_struct;
}	