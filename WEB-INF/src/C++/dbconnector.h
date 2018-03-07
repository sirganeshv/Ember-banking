#include <mysql.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
//#ifndef TYPE_H
//#define TYPE_H
//#endif
#include "type.h"
using namespace std;
MYSQL_RES *result;
MYSQL *con;
MYSQL_ROW row;
MYSQL_FIELD *field;

//Display the error message and exit
void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

//Find the customer who is least frequent
int find_least_frequent_customer() {
	int least_frequency = customer_frequency[0];
	int position = 0;
	for(int i =0;i < customer_list.size();i++) {
		if(customer_frequency[i] < least_frequency)
			position = i;
			least_frequency = customer_frequency[i];
	}
	return position;
}

//Initializes connection to DB
void init_db() {
	con = mysql_init(NULL);
	if (con == NULL) {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
	}
	if (mysql_real_connect(con, "localhost", "root", "","banking", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }  
}

//Closes connection with DB
void close_db() {
	mysql_close(con);
}

//Write customer info into DB and returns the account number
int write_customer_into_db(customer_details customer) {
	char query[150];
	time_t nowtime;
	tm time;
	char milli[4];
	nowtime = (time_t)customer.last_accessed_time.tv_sec;
	time = *localtime(&nowtime);
	long mill = customer.last_accessed_time.tv_usec;
	sprintf(milli,"%3d",customer.last_accessed_time.tv_usec);
	char time_string[50];
	sprintf(time_string,"%02d:%02d:%02d:%s",time.tm_hour,time.tm_min,time.tm_sec,milli);
	sprintf(query,"INSERT INTO customers VALUES(%d,'%s',%d,'%s','%s',%d,'%s',%d,'%s','%s','%s','%s')",
	0,customer.customer_name,customer.age,customer.phone_no,customer.address,customer.balance,customer.passphrase,
	customer.wrong_attempts,customer.is_active?"true":"false",customer.security_question,customer.security_answer,time_string);
	if (mysql_query(con,query)){
      finish_with_error(con);
	}
	int acc_no = mysql_insert_id(con);
	return acc_no++;
}

//Reads a specific customer from DB provided account number
bool read_customer_from_db(int acc_no) {
	char query[150];
	sprintf(query,"SELECT * FROM Customers where acc_no = %d and is_active = 'true'",acc_no);
	if (mysql_query(con,query)) {
      finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	customer_details customer;
	time_t timee = time(0);
	tm time_struct = *localtime(&timee);
	//timevall time_val;
	if ((row = mysql_fetch_row(result))) { 
		customer.acc_no = atoi(row[0]);
		strcpy(customer.customer_name,row[1]);
		customer.age = atoi(row[2]);
		strcpy(customer.phone_no,row[3]);
		strcpy(customer.address,row[4]);
		customer.balance = atoi(row[5]);
		strcpy(customer.passphrase,row[6]);
		customer.wrong_attempts = atoi(row[7]);
		if(!strcmp(row[8],"true"))
			customer.is_active = true;
		else
			customer.is_active = false;
		strcpy(customer.security_question,row[9]);
		strcpy(customer.security_answer,row[10]);
		char time_str[50];
		string delimiter = ":";
		strcpy(time_str,row[11]);
		size_t pos = 0;
		string time_string = time_str;
		pos = time_string.find(delimiter);
		time_struct.tm_hour = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		time_struct.tm_min = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		time_struct.tm_sec = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		timee = mktime(&time_struct);
		customer.last_accessed_time.tv_sec = static_cast<long> (timee);
		customer.last_accessed_time.tv_usec = (long)stoi(time_string.substr(0));
	}
	if(!row) {
		return false;
	}
	if(customer.is_active == true) {
		if(customer_list.size() == 3) {
			int position = find_least_frequent_customer();
			customer_list.erase(customer_list.begin() + position);
			customer_frequency.erase(customer_frequency.begin() + position);
		}
		customer_list.push_back(customer);
		customer_frequency.push_back(0);
		return true;
	}
	return false;
}

//Updates a specific customer in DB provided the customer info
void update_customer_in_db(customer_details customer) {
	char query[150];
	time_t nowtime;
	tm time;
	char milli[9];
	nowtime = (time_t)customer.last_accessed_time.tv_sec;
	time = *localtime(&nowtime);
	long mill = customer.last_accessed_time.tv_usec;
	sprintf(milli,"%3d",customer.last_accessed_time.tv_usec);
	char time_string[50];
	sprintf(time_string,"%02d:%02d:%02d:%s",time.tm_hour,time.tm_min,time.tm_sec,milli);
	string times = string(time_string);
	sprintf(query,"Update customers set phone_no = '%s',address = '%s',balance = %d,passphrase = '%s',wrong_attempts = %d,is_active = '%s' where acc_no = %d",
		customer.phone_no,customer.address,customer.balance,customer.passphrase,customer.wrong_attempts,customer.is_active?"true":"false",customer.acc_no);
	
	if (mysql_query(con,query)){
      finish_with_error(con);
	}
	sprintf(query,"update customers set last_time = '%s' where acc_no = %d",times.c_str(),customer.acc_no);
	if (mysql_query(con,query)){
      finish_with_error(con);
	}
	int acc_no = mysql_insert_id(con);
}

//Loads all customer info from DB
void load_customers_from_db() {
	if (mysql_query(con, "SELECT * FROM Customers where is_active = 'true'")) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	customer_details customer;
	time_t timee = time(0);
	tm time_struct = *localtime(&timee);
	//timevall time_val;
	while ((row = mysql_fetch_row(result))) { 
		customer.acc_no = atoi(row[0]);
		strcpy(customer.customer_name,row[1]);
		customer.age = atoi(row[2]);
		strcpy(customer.phone_no,row[3]);
		strcpy(customer.address,row[4]);
		customer.balance = atoi(row[5]);
		strcpy(customer.passphrase,row[6]);
		customer.wrong_attempts = atoi(row[7]);
		if(!strcmp(row[8],"true"))
			customer.is_active = true;
		else
			customer.is_active = false;
		strcpy(customer.security_question,row[9]);
		strcpy(customer.security_answer,row[10]);
		char time_str[50];
		string delimiter = ":";
		strcpy(time_str,row[11]);
		size_t pos = 0;
		string time_string = time_str;
		pos = time_string.find(delimiter);
		time_struct.tm_hour = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		time_struct.tm_min = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		time_struct.tm_sec = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		timee = mktime(&time_struct);
		customer.last_accessed_time.tv_sec = static_cast<long> (timee);
		customer.last_accessed_time.tv_usec = (long)stoi(time_string.substr(0));
		customer_list.push_back(customer);
	}
	mysql_free_result(result);
}

//Returns a list of customer details linked with a particular phone number
vector<customer_details> get_accounts_by_phone_no_from_db(char* phone_no) {
	vector<customer_details> customers;
	char query[150];
	sprintf(query,"SELECT * FROM Customers where phone_no = %s",phone_no);
	if (mysql_query(con, query)) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	customer_details customer;
	time_t timee = time(0);
	tm time_struct = *localtime(&timee);
	//timevall time_val;
	while ((row = mysql_fetch_row(result))) { 
		customer.acc_no = atoi(row[0]);
		strcpy(customer.customer_name,row[1]);
		customer.age = atoi(row[2]);
		strcpy(customer.phone_no,row[3]);
		strcpy(customer.address,row[4]);
		customer.balance = atoi(row[5]);
		strcpy(customer.passphrase,row[6]);
		customer.wrong_attempts = atoi(row[7]);
		if(!strcmp(row[8],"true"))
			customer.is_active = true;
		else
			customer.is_active = false;
		strcpy(customer.security_question,row[9]);
		strcpy(customer.security_answer,row[10]);
		char time_str[50];
		string delimiter = ":";
		strcpy(time_str,row[11]);
		size_t pos = 0;
		string time_string = time_str;
		pos = time_string.find(delimiter);
		time_struct.tm_hour = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		time_struct.tm_min = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		time_struct.tm_sec = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		timee = mktime(&time_struct);
		customer.last_accessed_time.tv_sec = static_cast<long> (timee);
		customer.last_accessed_time.tv_usec = (long)stoi(time_string.substr(0));
		customers.push_back(customer);
	}
	mysql_free_result(result);
	return customers;
}

//Write transactions to DB
void write_transactions_to_db() {
	char query[150];
	//tm time;
	char time_string[50];
	if (mysql_query(con, "delete FROM transactions")) {
		finish_with_error(con);
	}
	for(int i = 0;i < transactions.size();i++) {
		sprintf(time_string,"%02d:%02d:%02d",transactions[i].timestamp.tm_hour,transactions[i].timestamp.tm_min,transactions[i].timestamp.tm_sec);
		sprintf(query,"INSERT INTO transactions VALUES(%d,'%s','%s',%d,%d,'%s','%s')",transactions[i].acc_no,transactions[i].deposit,
		transactions[i].withdraw,transactions[i].balance,transactions[i].period,transactions[i].is_completed?"true":"false",time_string);
		if (mysql_query(con, query)) {
			finish_with_error(con);
		}
	}
}

//Load transactions from DB
void load_transactions_from_db() {
	if (mysql_query(con, "SELECT * FROM transactions")) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	transaction trans;
	while ((row = mysql_fetch_row(result))) { 
		trans.acc_no = atoi(row[0]);
		strcpy(trans.deposit,row[1]);
		strcpy(trans.withdraw,row[2]);
		trans.balance = atoi(row[3]);
		trans.period =atoi(row[4]);
		if(!strcmp(row[5],"true"))
			trans.is_completed = true;
		else
			trans.is_completed = false;
		char time_str[50];
		strcpy(time_str,row[6]);
		trans.timestamp = get_timestamp_from_string(time_str);
		transactions.push_back(trans);
	}
}

//Write pending_transactions to DB
void write_pending_transactions_to_db() {
	char query[150];
	//tm time;
	char time_string[50];
	if (mysql_query(con, "delete FROM pending_transactions")) {
		finish_with_error(con);
	}
	for(int i = 0;i < pending_transactions.size();i++) {
		sprintf(time_string,"%02d:%02d",pending_transactions[i].timestamp.tm_hour,pending_transactions[i].timestamp.tm_min);
		sprintf(query,"INSERT INTO pending_transactions VALUES(%d,'%s','%s',%d,%d,'%s','%s')",pending_transactions[i].acc_no,pending_transactions[i].deposit,
		pending_transactions[i].withdraw,pending_transactions[i].balance,pending_transactions[i].period,pending_transactions[i].is_completed?"true":"false",time_string);
		if (mysql_query(con, query)) {
			finish_with_error(con);
		}
	}
}

//Load pending_transactions from DB
void load_pending_transactions_from_db() {
	if (mysql_query(con, "SELECT * FROM pending_transactions")) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	transaction trans;
	time_t timee = time(0);
	tm time_struct = *localtime(&timee);
	time_struct.tm_sec = 0;
	while ((row = mysql_fetch_row(result))) { 
		trans.acc_no = atoi(row[0]);
		strcpy(trans.deposit,row[1]);
		strcpy(trans.withdraw,row[2]);
		trans.balance = atoi(row[3]);
		trans.period =atoi(row[4]);
		if(!strcmp(row[5],"true"))
			trans.is_completed = true;
		else
			trans.is_completed = false;
		size_t pos = 0;
		char time_str[50];
		string delimiter = ":";
		strcpy(time_str,row[6]);
		string time_string = time_str;
		pos = time_string.find(delimiter);
		time_struct.tm_hour = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		time_struct.tm_min = stoi(time_string.substr(0, pos));
		time_string.erase(0, pos + delimiter.length());
		pos = time_string.find(delimiter);
		//time_struct.tm_sec = stoi(time_string.substr(0, pos));
		trans.timestamp = time_struct;
		pending_transactions.push_back(trans);
	}
	mysql_free_result(result);
}

//Write customer information to DB
void write_customer_info_to_db() {
	char query[150];
	if (mysql_query(con, "delete FROM customer_info")) {
		finish_with_error(con);
	}
	for(int i = 0;i < customer_infos.size();i++) {
		sprintf(query,"INSERT INTO customer_info VALUES(%d,%d,%d)",customer_infos[i].acc_no,
		customer_infos[i].amount_withdrawn,customer_infos[i].no_of_transactions);
		if (mysql_query(con, query)) {
			finish_with_error(con);
		}
	}
}

//Load customer information from DB
void load_customer_info_from_db() {
	if (mysql_query(con, "SELECT * FROM customer_info")) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	customer_info info;
	while ((row = mysql_fetch_row(result))) { 
		info.acc_no = atoi(row[0]);
		info.amount_withdrawn = atoi(row[1]);
		info.no_of_transactions = atoi(row[2]);
		customer_infos.push_back(info);
	}
	mysql_free_result(result);
}

//Write fixed deposits to DB
void write_fixed_deposits_to_db() {
	char query[150];
	char start_time_string[50];
	char maturity_time_string[50];
	if (mysql_query(con, "delete FROM fixed_deposits")) {
		finish_with_error(con);
	}
	for(int i = 0;i < deposits.size();i++) {
		sprintf(start_time_string,"%02d:%02d:%02d",deposits[i].start_time.tm_hour,deposits[i].start_time.tm_min,deposits[i].start_time.tm_sec);
		sprintf(maturity_time_string,"%02d:%02d:%02d",deposits[i].maturity_time.tm_hour,deposits[i].maturity_time.tm_min,deposits[i].maturity_time.tm_sec);
		sprintf(query,"INSERT INTO fixed_deposits VALUES(%d,%d,%d,%d,'%s','%s',%d)",deposits[i].acc_no,deposits[i].duration,
		deposits[i].rate_of_interest,deposits[i].principal,start_time_string,maturity_time_string,deposits[i].amount);
		if (mysql_query(con, query)) {
			finish_with_error(con);
		}
	}
}

//Load fixed Deposits from DB
void load_fixed_deposits_from_db() {
	if (mysql_query(con, "SELECT * FROM fixed_deposits")) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	time_t timee = time(0);
	tm time_struct = *localtime(&timee);
	time_struct.tm_sec = 0;
	fixed_deposit fd;
	while ((row = mysql_fetch_row(result))) { 
		fd.acc_no = atoi(row[0]);
		fd.duration = atoi(row[1]);
		fd.rate_of_interest = atoi(row[2]);
		fd.principal = atoi(row[3]);
		char time_str[50];
		strcpy(time_str,row[4]);
		fd.start_time = get_timestamp_from_string(time_str);
		strcpy(time_str,row[5]);
		fd.maturity_time = get_timestamp_from_string(time_str);
		fd.amount = atoi(row[6]);
		deposits.push_back(fd);
	}
	mysql_free_result(result);
}

//Write last_accessed_time and next_fd_time to DB
void write_times_to_db() {
	char query[150];
	char last_clearing_time_string[50];
	char next_fd_time_string[50];
	if (mysql_query(con, "delete FROM times")) {
		finish_with_error(con);
	}
	sprintf(last_clearing_time_string,"%02d:%02d:%02d",last_clearing_time.tm_hour,last_clearing_time.tm_min,last_clearing_time.tm_sec);
	sprintf(next_fd_time_string,"%d:%d:%d",next_fd_time.tm_hour,next_fd_time.tm_min,next_fd_time.tm_sec);
	sprintf(query,"INSERT INTO times VALUES('%s','%s')",last_clearing_time_string,next_fd_time_string);
	if (mysql_query(con, query)) {
		finish_with_error(con);
	}
}

//Read last_accessed_time and next_fd_time from DB
void load_times_from_db() {
	if (mysql_query(con, "SELECT * FROM times")) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	time_t timee = time(0);
	tm time_struct = *localtime(&timee);
	time_struct.tm_sec = 0;
	while ((row = mysql_fetch_row(result))) {
		char time_str[50];
		strcpy(time_str,row[0]);
		last_clearing_time = get_timestamp_from_string(time_str);
		strcpy(time_str,row[1]);
		next_fd_time = get_timestamp_from_string(time_str);
		
	}
	mysql_free_result(result);
}

//Write operator details to DB
void write_operator_to_db() {
	char query[150];
	if (mysql_query(con, "delete FROM accounts_under_operator")) {
		finish_with_error(con);
	}
	if (mysql_query(con, "delete FROM operators")) {
		finish_with_error(con);
	}
	for(int i = 0;i < operators.size();i++) {
		sprintf(query,"INSERT INTO operators VALUES('%s',%d,'%s',%d,'%s')",operators[i].name,operators[i].id,
		operators[i].password,operators[i].no_of_customers,operators[i].is_admin ? "true" : "false");
		if (mysql_query(con, query)) {
			finish_with_error(con);
		}
	}
	for(int i = 0;i < operators.size();i++) {
		for(int j = 0;j < operators[i].no_of_customers;j++) {
			sprintf(query,"INSERT INTO accounts_under_operator VALUES(%d,%d)",operators[i].id,operators[i].customer_acc_no_list[j]);
			if (mysql_query(con, query)) {
				finish_with_error(con);
			}
		}
	}
}

//Read operator details from DB
void load_operators_from_db() {
	char query[150];
	if (mysql_query(con, "SELECT * FROM operators")) {
		finish_with_error(con);
	}
	result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}
	MYSQL_ROW row;
	bank_operator op;
	while ((row = mysql_fetch_row(result))) { 
		strcpy(op.name,row[0]);
		op.id = atoi(row[1]);
		strcpy(op.password,row[2]);
		op.no_of_customers = atoi(row[3]);
		if(!strcmp(row[4],"true"))
			op.is_admin = true;
		else
			op.is_admin = false;
		operators.push_back(op);
	}
	for(int i = 0;i < operators.size();i++) {
		int j = 0;
		sprintf(query,"SELECT acc_no FROM accounts_under_operator where id = %d",operators[i].id);
		if (mysql_query(con, query)) {
			finish_with_error(con);
		}
		result = mysql_store_result(con);
		if (result == NULL) {
			finish_with_error(con);
		}
		while((row = mysql_fetch_row(result))) {
			operators[i].customer_acc_no_list[j++] = atoi(row[0]);
		}
	}
	mysql_free_result(result);
}
