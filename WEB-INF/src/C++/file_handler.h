#include <fstream>
//#include "encryption.h"
#include "dbconnector.h"

fstream file;

bool read_customer(int acc_no) {
	return read_customer_from_db(acc_no);
}

//Writes new customer's info  
int write_customer(customer_details customer) {
	return write_customer_into_db(customer);
}

//Updates a customer
void update_customer(customer_details customer) {
	update_customer_in_db(customer);
}

//Loads all the customer info into customer_list for display
void load_customers() {
	customer_list.clear();
	load_customers_from_db();
}

//Load files into the program
void load_files() {
	//clearing all the vectors before use
	transactions.clear();
	pending_transactions.clear();
	operators.clear();
	customer_infos.clear();
	deposits.clear();
	//Reading from DB to load details
	load_operators_from_db();
	load_transactions_from_db();
	load_pending_transactions_from_db();
	load_fixed_deposits_from_db();
	load_customer_info_from_db();
	load_times_from_db();
}

//Write the details of banking system into files
void write_files() {
	//Writing details into DB
	write_operator_to_db();
	write_transactions_to_db();
	write_pending_transactions_to_db();
	write_fixed_deposits_to_db();
	write_customer_info_to_db();
	write_times_to_db();
}

//Gets all accounts linked with a phone_no
vector<customer_details> get_accounts_by_phone_no(char* phone_no) {
	return get_accounts_by_phone_no_from_db(phone_no);
}
