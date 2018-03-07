#include <string>
#include <iostream>

using namespace std;

//Checks if the string is a valid name
bool is_valid_name(string name) {
	for(int i = 0;i < name.length();i++) {
		if(isalpha(name[i])==0)
			return false;
	}
	return true;
}

//Checks if the string is a valid phone number
bool is_valid_no(string phone_no) {
	for(int i = 0;i < 4;i++) {
		if(isdigit(phone_no[i])==0)
			return false;
	} 	
	return true;
}

