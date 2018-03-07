#include <string>
#include "type.h"

void encrypt(customer_details * customer) {
	int operator_id = find_operator(customer->acc_no);
	int key = operators[find_operator_position(operator_id)].password[0];
	char name[30] = "",phone_no[11] = "",address[30] = "",pass[10] = "";
	
	//Encrypt customer name
	for(int i=0; i<strlen(customer->customer_name); i++) 
		name[i] = customer->customer_name[i] + ((customer->passphrase[i])%strlen(customer->passphrase)) + key;
	strcpy(customer->customer_name,name);
	
	//Encrypt customer age
	customer->age = customer->age + customer->passphrase[0] + key;
	
	//Encrypt customer phone_no
	for(int i=0; i<strlen(customer->phone_no); i++) 
		phone_no[i] = customer->phone_no[i] + ((customer->passphrase[i])%strlen(customer->passphrase)) + key;
	strcpy(customer->phone_no,phone_no); 
	
	//Encrypt customer address
	for(int i=0; i<strlen(customer->address); i++) 
		address[i] = customer->address[i] + ((customer->passphrase[i])%strlen(customer->passphrase)) + key;
	strcpy(customer->address,address); 
	
	//Encrypt customer balance
	customer->balance = customer->balance + customer->passphrase[0] + key;
	
	//Encrypt customer passphrase
	for(int i=0; i<strlen(customer->passphrase); i++) 
		pass[i] = customer->passphrase[i] + key;
	strcpy(customer->passphrase,pass);
}


void decrypt(customer_details * customer) {
	int operator_id = find_operator(customer->acc_no);
	int key = operators[find_operator_position(operator_id)].password[0];
	char name[30] = "",phone_no[11] = "",address[30] = "",pass[10] = "";
	
	//Decrypt customer passphrase
	for(int i=0; i<strlen(customer->passphrase); i++) 
		pass[i] = customer->passphrase[i] - key;
	strcpy(customer->passphrase,pass);
	
	//Decrypt customer name
	for(int i=0; i<strlen(customer->customer_name); i++)
		name[i] = customer->customer_name[i] - ((customer->passphrase[i])%strlen(customer->passphrase)) - key;
	strcpy(customer->customer_name,name);
	
	//Decrypt customer age
	customer->age = customer->age - customer->passphrase[0] - key;
	
	//Decrypt customer phone_no
	for(int i=0; i<strlen(customer->phone_no); i++)
		phone_no[i] = customer->phone_no[i] - ((customer->passphrase[i])%strlen(customer->passphrase)) - key;
	strcpy(customer->phone_no,phone_no);  
	
	//Decrypt customer address
	for(int i=0; i<strlen(customer->address); i++) 
		address[i] = customer->address[i] - ((customer->passphrase[i])%strlen(customer->passphrase)) - key;
	strcpy(customer->address,address); 
	
	//Decrypt customer balance
	customer->balance = customer->balance - customer->passphrase[0] - key;
}