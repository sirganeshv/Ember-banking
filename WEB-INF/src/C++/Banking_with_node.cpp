#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
//#include <bits/stdc++.h>
#include<stdbool.h>
//#include "WinTimer.h"
#include <node.h>
#include "valid.h"
#include "timestamp.h"
#include "file_handler.h"
//#include "dbconnector.h"
//#include "global.h"
#define MAX_WITHDRAW 300
#define MAX_NO_OF_TRANSACTIONS 2
#define INTEREST_NORMAL 5
#define INTEREST_SENIOR 10


using namespace std;
using chrono::system_clock;

struct tm next_time,last_clearing_time = get_timestamp(),next_deposit_time = get_timestamp();
struct tm next_fd_time;

int current_operator_position;
vector<transaction> transactions;
vector<customer_details> customer_list;
vector<int> customer_frequency;
vector<transaction> pending_transactions;
vector<bank_operator> operators;
vector<customer_info> customer_infos;
vector<fixed_deposit> deposits;
int next_acc_no = 1000;
struct timevall current_accessed_time;

CTimer timer;
CTimer clear_limits_timer;
CTimer next_deposit_timer;
CTimer next_fd_timer;

bool is_first_run = false;
bank_operator * current_operator;

void load_files();
void write_files();
void load_customers();
void clear_fd();
bool forgot_password(int);
//void get_accounts_by_phone_no(char[] phone_no);

//Used to find the position of customer in the list based on account number
int find_customer_position(int acc_no) {
	int i;
	for(i = 0;i < customer_list.size();i++) {
		if(customer_list[i].acc_no == acc_no) {		
			break;
		}
	}
	if(i == customer_list.size()) {
		cout<<"Enter valid account number";
		return -1;
	}
	return i;
}						


//Finds the position of transaction in the list based on account number
int find_transaction_position(int acc_no) {
	int i;
	for(i = 0;i < pending_transactions.size();i++) {
		if(pending_transactions[i].acc_no == acc_no) {		
			break;
		}
	}
	return i;
}		

int find_customer_info_position(int acc_no) {
	int i;
	for(i = 0;i < customer_infos.size();i++) {
		if(customer_infos[i].acc_no == acc_no) {		
			break;
		}
	}
	return i;
}		


//Finds operator position in the list based on operator ID
int find_operator_position(int id) {
	int i;
	for(i = 0;i < operators.size();i++) 
		if(operators[i].id == id)	
			return i;
}	


//Checked whether the customer is under current operator or not 
bool is_customer_under_current_operator(int position) {
	if(std::find(current_operator->customer_acc_no_list.begin(), current_operator->customer_acc_no_list.end(), 
		customer_list[position].acc_no) != current_operator->customer_acc_no_list.end())
			return true;
	return false;
}


//Find operator for certain account number
int find_operator(int acc_no) {
	for(int i = 0;i < operators.size();i++)
		if(std::find(operators[i].customer_acc_no_list.begin(), operators[i].customer_acc_no_list.end(), 
			acc_no) != operators[i].customer_acc_no_list.end())
				return operators[i].id;
}


//Gets the account number
int get_acc_no() {
	int acc_no;
	cout<<"Enter your account number\n";
	cin>>acc_no;
	return acc_no;
}		

//Gets the passphrase from the console
string get_passphrase() {
	string passphrase;
	cout<<"Enter your passphrase\n";
	char c =' ';
	int i;
	c = getch();
	while (c!=13){
		if(c == 8) {
			printf("\b ");
			printf("\b");
			passphrase.pop_back();
			c = getch();
			i--;
		}
		else {
			passphrase.push_back(c);
			printf("*");
			c = getch();
			i++;
		}
	}
	cout<<"\n";
	return passphrase;
}


//Checks if the passphrase of a customer is valid or not
bool is_passphrase_valid(string passphrase,int i) {
	if(strcmp(passphrase.c_str(),customer_list[i].passphrase)) {
		cout<<"You have entered a wrong passphrase\n";
		customer_list[i].wrong_attempts++;
		return false;
	}
	customer_list[i].wrong_attempts = 0;
	return true;
}


//Gets the operator password from the console
string get_operator_password() {
	string password;
	cout<<"Enter operator password\n";
	char c =' ';
	int i;
	c = getch();
	while (c!=13){
		if(c == 8) {
			printf("\b ");
			printf("\b");
			password.pop_back();
			c = getch();
			i--;
		}
		else {
			password.push_back(c);
			printf("*");
			c = getch();
			i++;
		}
	}
	cout<<"\n";
	return password;
}

//Checks if the operator password is correct 
bool is_operator_password_correct(string password) {
	if(strcmp(password.c_str(),current_operator->password)) {
		cout<<"You have entered wrong password\n";
		return false;
	}
	return true;
}


//Triggered if a scheduled transfer event happens or at the time of loading program if pending_transactions exists
void listener() {
		bool is_deposited = false;
		bool is_completed = true;
		if(pending_transactions.size() >0 )
			next_time = pending_transactions[0].timestamp;
		//cout<<"Next time is "<<next_time.tm_hour<<" : "<<next_time.tm_min<<" : "<<next_time.tm_sec<<"\n"; 
		tm unmodified_time = next_time;
		for(int i = 0;i < pending_transactions.size();i++) {
			is_completed = true;
			if(is_time_less_than_equal(pending_transactions[i].timestamp.tm_hour,pending_transactions[i].timestamp.tm_min,
				get_timestamp().tm_hour,get_timestamp().tm_min)) {
				int k;
				for(k = 0;k < customer_list.size();k++)
					if(customer_list[k].acc_no == pending_transactions[i].acc_no)
						break;
				if(k == customer_list.size())
					read_customer(pending_transactions[i].acc_no);
				string str = "----";
				if(strcmp((pending_transactions[i].withdraw),str.c_str())) {
					std::this_thread::sleep_for(1s);
					int j = find_customer_position(pending_transactions[i].acc_no);
					pending_transactions[i].balance = customer_list[j].balance - stoi(pending_transactions[i].withdraw);
					if(pending_transactions[i].balance >= 0) {
						customer_list[j].balance -= stoi(pending_transactions[i].withdraw);
						is_completed = pending_transactions[i].is_completed;
						pending_transactions[i].is_completed = true;
						transactions.push_back(pending_transactions[i]);		
						pending_transactions[i].is_completed = is_completed;
					}
					else {
						pending_transactions.erase(pending_transactions.begin()+i+1);
					}
				}
				else { 
					int j = find_customer_position(pending_transactions[i].acc_no);
					pending_transactions[i].balance = customer_list[j].balance + stoi(pending_transactions[i].deposit);
					customer_list[j].balance += stoi(pending_transactions[i].deposit);
					is_completed = pending_transactions[i].is_completed;
					cout<<is_completed<<"\n";
					pending_transactions[i].is_completed = true;
					transactions.push_back(pending_transactions[i]);
					pending_transactions[i].is_completed = is_completed;
					is_deposited = true;
					cout<<pending_transactions[i].is_completed<<"\n";
				}
				int j = find_customer_position(pending_transactions[i].acc_no);
				update_customer(customer_list[j]);
				if(pending_transactions[i].is_completed == false) {
					pending_transactions[i].timestamp = add_minutes_to_timestamp(pending_transactions[i].timestamp,pending_transactions[i].period);
					pending_transactions.push_back(pending_transactions[i]);
				}						
				//cout<<"Erasing "<<pending_transactions[i].acc_no<<"\n";
				pending_transactions.erase(pending_transactions.begin()+i);
				i--;
				srand(1);
				write_files();
				srand(1);
				load_files();
			}
			else {
				if(!is_timestamp_less_than(next_time,pending_transactions[i].timestamp)) {
						next_time = pending_transactions[i].timestamp;
					}
			}			
		}
		tm current = get_timestamp();
		if(pending_transactions.size() != 0 ) {
			if(is_timestamp_equal(next_time,unmodified_time)) {
				next_time = pending_transactions[0].timestamp;
				for(int i = 0;i < pending_transactions.size();i++) {
					if(!is_timestamp_less_than(next_time,pending_transactions[i].timestamp)) {
						next_time = pending_transactions[i].timestamp;
					}
				}
			}
			struct tm current = get_timestamp();
			//if(is_deposited) {
				//cout<<"Timer at "<<next_time.tm_hour<<" : "<<next_time.tm_min<<" : "<<next_time.tm_sec<<"\n";
				timer = CTimer(listener, (mktime(&next_time) - mktime(&current))*1000);
				timer.Start();
			//}
		}
		if(pending_transactions.size() == 0) {
			timer.Stop();
		}
		write_files();
		load_files();
}


void clear_limits() {
	//cout<<"Clearing limits\n";
	struct tm current = get_timestamp();
	last_clearing_time.tm_min++;
	last_clearing_time.tm_sec = 0;
	if(last_clearing_time.tm_min == 60 ) {
		last_clearing_time.tm_min = 0;
		last_clearing_time.tm_hour = last_clearing_time.tm_hour + 1;
	}
	for(int i = 0;i < customer_infos.size();i++) {
		customer_infos[i].no_of_transactions = 0;
		customer_infos[i].amount_withdrawn = 0;
	}
	write_files();
	load_files();
	clear_limits_timer = CTimer(clear_limits, (mktime(&last_clearing_time) - mktime(&current))*1000);
	clear_limits_timer.Start();
}

void init_clear_limits() {
	struct tm current = get_timestamp();
	if(is_timestamp_less_than_equal(last_clearing_time,current)) {
		last_clearing_time = get_timestamp();
		clear_limits();
	}
	else {
		last_clearing_time.tm_sec = 0;
		clear_limits_timer = CTimer(clear_limits, (mktime(&last_clearing_time) - mktime(&current))*1000);
		clear_limits_timer.Start();
	}
}


bank_operator oprator;
//Creates new Admin Operator on first run
void create_operator() {
	cout<<"Creating an operator account.......\n";
	cout<<"Enter your name\t\t";
	cin>>oprator.name;
	cout<<"Enter your id\t\t";
	cin>>oprator.id;
	cout<<"Enter you password\t";
	cin>>oprator.password;
	current_operator = & oprator;
	current_operator->is_admin = true;
	operators.push_back(oprator);
	current_operator = & operators[0];
	srand(1);
	write_files();
	srand(1);
	load_files();
}


//Deposit money into user account
bool deposit_money(int i,int deposit_value) {
	customer_list[i].balance += deposit_value;
	if(customer_list[i].wrong_attempts >= 3) {
		customer_list[i].wrong_attempts = 0;
	}
	gettimeofday(&customer_list[i].last_accessed_time);
	//customer_list[i].frequency++;
	customer_frequency[i]++;
	cout<<"Frequency is "<<customer_frequency[i];
	write_files();
	cout<<"wrote files\n";
	load_files();
	cout<<"loaded files\n";
	update_customer_in_db(customer_list[i]);
	cout<<"wrote customer\n";
	return true;
}


//Withdraw money if sufficient balance and valid passphrase exists
bool withdraw_money(int i,int withdraw_value) {
	if(!is_customer_under_current_operator(i) && !current_operator->is_admin ) {
		cout<<"The customer is under a different operator";
		return false;
	}
	if(customer_list[i].balance < 0) {
		cout<<"You do not have sufficient balance";
		return false;
	}
	int k = find_customer_info_position(customer_list[i].acc_no);
	if(customer_infos[k].no_of_transactions == MAX_NO_OF_TRANSACTIONS) {
		cout<<"Max number of transactions reached";
		return false;
	}
	if(customer_infos[k].amount_withdrawn == MAX_WITHDRAW) {
		cout<<"Already 3000 taken this minute";
		return false;
	}
	if(customer_infos[k].amount_withdrawn + withdraw_value > MAX_WITHDRAW) {
		cout<<"Can withdraw only upto " << MAX_WITHDRAW - customer_infos[k].amount_withdrawn;
		return false;
	}
	customer_list[i].balance -= withdraw_value;
	gettimeofday(&customer_list[i].last_accessed_time);
	//customer_list[i].frequency++;
	customer_frequency[i]++;
	customer_infos[k].no_of_transactions++;
	customer_infos[k].amount_withdrawn += withdraw_value;
	srand(1);
	write_files();
	update_customer(customer_list[i]);
	srand(1);
	load_files();
	return true;
}

//Gives the option to reset password based on security question
bool forgot_password(int i) {
	cout<<customer_list[i].security_question<<"\n";
	string answer;
	cin.ignore();
	getline(cin,answer);
	if(!strcmp(answer.c_str(),customer_list[i].security_answer)) {
		strcpy(customer_list[i].passphrase,get_passphrase().c_str());
	}
	else {
		cout<<"Please try again\n";
		return false;
	}
	customer_list[i].wrong_attempts = 0;
	update_customer(customer_list[i]);
	return true;
}

//Transfer money to another existing account
/*void transfer_money() {
	char passphrase[10];
	int withdraw_acc_no = get_acc_no();
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == withdraw_acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(withdraw_acc_no);
	int i = find_customer_position(withdraw_acc_no);
	if(i == -1) 
		return;
	int x = find_customer_info_position(withdraw_acc_no);
	if(customer_infos[x].no_of_transactions == MAX_NO_OF_TRANSACTIONS) {
		cout<<"Max number of transactions reached";
		return;
	}
	cout<<"Enter the amount to transfer\t\t";
	int amount;
	cin>>amount;
	while(amount <= 0) {
		cout<<"Enter valid amount\n";
		cin>>amount;
	}
	int acc_no;
	char phone_no[11];
	if(customer_list[i].balance - amount > 0) {
		cout<<"Enter mobile number of the receiver\t";
		cin>>phone_no;
		while(strlen(phone_no) !=4 || !(is_valid_no(phone_no))) {
			cout<<"Enter valid number(4 digits)\n";
			cin>>phone_no;
		}
		vector<customer_details> customers = get_accounts_by_phone_no(phone_no);
		if(customers.empty()) {
			cout<<"Phone number doesn't exist";
			return;
		}
		timevall last_accessed_time = customers[0].last_accessed_time;
		acc_no = customers[0].acc_no;
		timevall time;
		for(int l = 0;l < customers.size();l++) {
			time = customers[l].last_accessed_time;
			if(((last_accessed_time.tv_sec - time.tv_sec)*1000 + (last_accessed_time.tv_usec - time.tv_usec)/1000.0f) < 0 ) {
				last_accessed_time = customers[l].last_accessed_time;
				acc_no = customers[l].acc_no;
			}
		}
		cout<<"The account number is "<<acc_no<<"\n";
		for(k = 0;k < customer_list.size();k++) {
			if(customer_list[k].acc_no == acc_no) {
				customer_list.push_back(customer_list[k]);
				customer_list.erase(customer_list.begin() +  k );
				customer_frequency.push_back(customer_frequency[k]);
				customer_frequency.erase(customer_frequency.begin() + k);
				break;
			}
		}
		if(k == customer_list.size())
			read_customer(acc_no);
		int i = find_customer_position(withdraw_acc_no);
		int j = find_customer_position(acc_no);
		if(j == -1) {
			return;
		}
		if(customer_list[i].wrong_attempts >= 3) {
			cout<<"Locked\nDeposit to unlock\n";
			return;
		}
		if(i == j || !strcmp(customer_list[i].phone_no,customer_list[j].phone_no)) {
			cout<<"You cannot transfer your money to yourself\n";
			return;
		}
		if(!withdraw_money(i,amount))
			return;
		gettimeofday(&customer_list[i].last_accessed_time);
		//customer_list[i].frequency++;
		customer_frequency[i]++;
		deposit_money(j,amount);
		struct transaction deposit_transaction;
		deposit_transaction.timestamp = get_timestamp();
		deposit_transaction.acc_no = acc_no;
		strcpy(deposit_transaction.deposit,(to_string(amount)).c_str());
		deposit_transaction.balance = customer_list[j].balance;
		transactions.push_back(deposit_transaction);
		struct transaction withdraw_transaction;
		withdraw_transaction.timestamp = get_timestamp();
		withdraw_transaction.acc_no = withdraw_acc_no;
		strcpy(withdraw_transaction.withdraw,(to_string(amount)).c_str());
		withdraw_transaction.balance = customer_list[i].balance;
		transactions.push_back(withdraw_transaction);
		srand(1);
		write_files();
		update_customer(customer_list[i]);
		update_customer(customer_list[j]);
		srand(1);
		load_files();
	}
	else 
		cout<<"You do not have sufficient balance";
}*/


//Prints pending transaction (Not visible to user)
void print_pending_transactions() {
	if(pending_transactions.size() > 0 ) {
		cout<<"\nAcc no\t\tDeposit\t\tWithdraw\tBalance\t\tTime\n";
		for(int i = 0;i < pending_transactions.size();i++) {
			cout<<pending_transactions[i].acc_no<<"\t\t";
			cout<<pending_transactions[i].deposit<<"\t\t";
			cout<<pending_transactions[i].withdraw<<"\t\t";
			cout<<pending_transactions[i].balance<<"\t\t";
			cout<<get_timestamp_string(pending_transactions[i].timestamp)<<"\n";
		}
	}
	else 
		cout<<"There are no pending transactions";
}


void initialize () {
	next_time = get_timestamp();
	next_fd_time = get_timestamp();
	load_files();
	//cout<<next_time.tm_hour<<" : "<<next_time.tm_min<<"\n";
	if(!pending_transactions.empty()) {
		is_first_run = true;
		listener();
	}
	init_clear_limits();
	if(!deposits.empty())
		clear_fd();
	//Creates operator account if there is no operator
	//Else operator is asked to logged in
	/*if(operators.empty()) {
		cout<<"You are the first operator\n";
		create_operator();
	}
	else
		operator_login();
	*/
}


void record_deposit(int acc_no, int deposit_value) {
	int i = find_customer_position(acc_no);
	struct transaction deposit_transaction;
	deposit_transaction.timestamp = get_timestamp();
	deposit_transaction.acc_no = acc_no;
	strcpy(deposit_transaction.deposit,(to_string(deposit_value)).c_str());
	deposit_transaction.balance = customer_list[i].balance;
	transactions.push_back(deposit_transaction);
}


void record_withdrawal(int acc_no, int withdraw_value) {
	int i = find_customer_position(acc_no);
	struct transaction withdraw_transaction;
	withdraw_transaction.timestamp = get_timestamp();
	withdraw_transaction.acc_no = acc_no;
	strcpy(withdraw_transaction.withdraw,(to_string(withdraw_value)).c_str());
	withdraw_transaction.balance = customer_list[i].balance;
	transactions.push_back(withdraw_transaction);
}

//Displays current cache
void display_current_cache() {
	if(customer_list.size() > 0 ) {
		cout<<"Account num \n";
		for(int i =0;i < customer_list.size();i++)
			cout<<customer_list[i].acc_no<<"\t"<<customer_frequency[i]<<"\n";
	}
	else
		cout<<"Cache is empty\n";
}

//Gets current cache size
int get_cache_size() {
	return customer_list.size();
}

//Clears the cache
void clear_cache() {
	customer_list.clear();
	cout<<"Cache cleared\n";
}

//Clears the fd by setting appropriate timers
void clear_fd() {
	next_fd_time = deposits[0].maturity_time;
	tm unmodified_time = next_fd_time;
	for(int i = 0;i < deposits.size();i++) {
		if(is_time_less_than_equal(deposits[i].maturity_time.tm_hour,deposits[i].maturity_time.tm_min,
			get_timestamp().tm_hour,get_timestamp().tm_min)) {
			struct transaction deposit_transaction;
			int k;
			for(k = 0;k < customer_list.size();k++)
				if(customer_list[k].acc_no == deposits[i].acc_no)
					break;
			if(k == customer_list.size())
				read_customer(deposits[i].acc_no);
			std::this_thread::sleep_for(1s);
			int j = find_customer_position(deposits[i].acc_no);
			if(j == -1)
				read_customer(deposits[i].acc_no);
			deposit_money(j,deposits[i].amount);
			deposit_transaction.timestamp = deposits[i].maturity_time;
			deposit_transaction.acc_no = deposits[i].acc_no;
			strcpy(deposit_transaction.deposit,(to_string(deposits[i].amount)).c_str());
			deposit_transaction.is_completed = true;
			deposit_transaction.balance = customer_list[k].balance;
			transactions.push_back(deposit_transaction);
			deposits.erase(deposits.begin()+i);
			i--;
			update_customer(customer_list[k]);
			write_files();
			load_files();
		}
		else {
			if(!is_timestamp_less_than(next_fd_time,deposits[i].maturity_time))
				next_fd_time = deposits[i].maturity_time;
		}
	}
	if(deposits.size() != 0) {
		if(is_timestamp_equal(next_fd_time,unmodified_time)) {
			next_fd_time = deposits[0].maturity_time;
			for(int i = 0;i < deposits.size();i++) {
				if(!is_timestamp_less_than(next_fd_time,deposits[i].maturity_time))
					next_fd_time = deposits[i].maturity_time;
			}
		}
		tm current = get_timestamp();
		next_fd_timer = CTimer(clear_fd, (mktime(&next_fd_time) - mktime(&current))*1000);
		next_fd_timer.Start();
	}
	write_files();
	load_files();
}

int main() {
		init_db();
		initialize();
		/*while(true) {
			cout<<"\n\n1.Add account\t\t2.Update details\t3.Delete account\t4.Display customer(using Acc no)\n";
			cout<<"5.Display cusomer list\t6.Add money\t\t7.Withdraw money\t8.Transfer money\t9.Acc statement\n";
			cout<<"10.Acc summary(time)\t11.Schedule transfer\t12.Create operator\t13.Exit\t\t\t16.Clear cache	\n";
			cout<<"18.Standing Instructions\t19.Fixed Deposit\t20.Forgot password\n";
			int option;
			cin>>option;
			switch(option) {
				case 1: 
					add_account();
				break;
				case 2:
					update_account();
				break;
				case 3:
					delete_account();
				break;
				case 4:
				{
						int acc_no = get_acc_no();
						int k;
						for(k = 0;k < get_cache_size();k++) {
							customer_details customer = customer_list[k];
							if(customer.acc_no == acc_no) {
								int i = find_customer_position(acc_no);
								cout<<"customer at "<<i;
								customer_list.push_back(customer_list[i]);
								customer_list.erase(customer_list.begin() + i);
								customer_frequency.push_back(customer_frequency[i]);
								customer_frequency.erase(customer_frequency.begin() + i);
								break;
							}
						}
						if(k == get_cache_size())
							read_customer(acc_no);
						int i = find_customer_position(acc_no);
						if(i == -1 ) 
							break;
						cout<<"The position is "<<i;
						if(!is_customer_under_current_operator(i) && !current_operator->is_admin) {
							cout<<"The customer is under some other operator\n";
							break;
						}
						cout<<"Details of customer \n\n";
						cout<<"\nName\t\tAcc No\t\tAge\t\tPhone\t\tAddress\t\tBalance\n\n";
						display_customer(i);
				}
				break;
				case 5:
					display_customer_list();
				break;
				case 6:
				{
						int acc_no = get_acc_no();
						int j;
						customer_details customer;
						for(j = 0;j < get_cache_size();j++) {
							customer = customer_list[j];
							if(customer.acc_no == acc_no) {
								int i = find_customer_position(acc_no);
								customer_list.push_back(customer_list[i]);
								customer_list.erase(customer_list.begin() + i);
								customer_frequency.push_back(customer_frequency[i]);
								customer_frequency.erase(customer_frequency.begin() + i);
								break;
							}
						}
						if(j == get_cache_size())
							read_customer(acc_no);
						int i = find_customer_position(acc_no);
						if(i == -1)
							break;
						int deposit_value;
						cout<<"How much do you want to deposit\n";
						cin>>deposit_value;
						bool is_successful = deposit_money(i,deposit_value);
						if(is_successful)
							record_deposit(acc_no,deposit_value);
						write_files();
						load_files();
				}
				break;
				case 7:
				{
					int acc_no = get_acc_no();
					int j;
					customer_details customer;
						for(j = 0;j < get_cache_size();j++) {
							customer = customer_list[j];
							if(customer.acc_no == acc_no) {
								int i = find_customer_position(acc_no);
								customer_list.push_back(customer_list[i]);
								customer_list.erase(customer_list.begin() + i);
								customer_frequency.push_back(customer_frequency[i]);
								customer_frequency.erase(customer_frequency.begin() + i);
								break;
							}
						}
						if(j == get_cache_size())
							read_customer(acc_no);
						int i = find_customer_position(acc_no);
						if(i == -1) 
							break;
						customer = customer_list[i];
						if(customer.wrong_attempts >= 3) {
							cout<<"Locked\nDeposit to unlock\n";
							break;
						}
						int withdraw_value;
						cout<<"How much do you want to withdraw\n";
						cin>>withdraw_value;
						bool is_successful = withdraw_money(i,withdraw_value);
						if(is_successful) {
							record_withdrawal(acc_no,withdraw_value);
						}
						write_files();
						load_files();
				}
				break;
				case 8:
					transfer_money();
				break;
				case 9:
					print_account_statement();
				break;
				case 10:
					print_account_summary_in_range();
				break;
				case 11:
					schedule_transfer();
				break;
				case 12:
					if(current_operator->is_admin)
						add_operator();
					else	
						cout<<"You do not have admin privileges\n";
				break;
				case 13:
					write_files();
					close_db();
					exit(0);
				case 14:
					print_pending_transactions();
				break;
				case 15: {
					int size = get_cache_size();
					cout<<"The size " << size <<"\n";
				break;
				}
				case 16:
					clear_cache();
				break;
				case 17:
					display_current_cache();
				break;
				case 18:
					add_standing_transactions();
				break;
				case 19:
					fixed_deposit();
				break;
				case 20: 
				{
					int acc_no = get_acc_no();
					int j;
					for(j = 0;j < customer_list.size();j++) {
						if(customer_list[j].acc_no == acc_no) {
							customer_list.push_back(customer_list[j]);
							customer_list.erase(customer_list.begin()+j);
							customer_frequency.push_back(customer_frequency[j]);
							customer_frequency.erase(customer_frequency.begin() + j);
							break;
						}
					}
					if(j == customer_list.size())
						read_customer(acc_no);
					int i = find_customer_position(acc_no);
					if(i == -1)
						break;
					forgot_password(i);
				break;
				}
				default:
					cout<<"Enter the correct option\n";
			}	
		}*/
	}
namespace demo1 {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;
using v8::Array;


//Adds a new operator
void add_operator(const FunctionCallbackInfo<Value>& args) {
	bank_operator opratorr;
	v8::String::Utf8Value param1(args[0]->ToString());
	std::string name = std::string(*param1);
	strcpy(opratorr.name,name.c_str());
	opratorr.id = args[1]->Int32Value();
	v8::String::Utf8Value param2(args[2]->ToString());
	std::string password = std::string(*param2);
	strcpy(opratorr.password,password.c_str());
	v8::String::Utf8Value param3(args[3]->ToString());
	std::string is_admin = std::string(*param3);
	if(!strcmp(is_admin.c_str(),"yes"))
		opratorr.is_admin = true;
	else
		opratorr.is_admin = false;
	//cout<<"string  "<<is_admin;
	current_operator_position = current_operator->id;
	operators.push_back(opratorr);

	for(int i = 0;i < operators.size();i++)
		if(operators[i].id == current_operator_position) {
			current_operator = & operators[i];
			break;
		}
	srand(1);
	write_files();
	srand(1);
	load_files();
}


//Logs the operator in
void operator_login(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	int id;
	id = args[0]->Int32Value();
	v8::String::Utf8Value param1(args[1]->ToString());
	std::string password = std::string(*param1);
	int i;
	for(i =0 ;i < operators.size();i++) {
		if((operators[i].id == id) && (!strcmp(operators[i].password,password.c_str()))) {
			current_operator = &operators[i];
			cout<<"Welcome "<<current_operator->name;
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
			break;
		}
	}
	if(i == operators.size()) {
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));
	}
}


//Adds account for new customer and assigns account number
void add_account(const FunctionCallbackInfo<Value>& args){
	Isolate* isolate = args.GetIsolate();
	Local<Object> obj = Object::New(isolate);
	customer_details customer;
	v8::String::Utf8Value param1(args[0]->ToString());
	std::string name = std::string(*param1);
	strcpy(customer.customer_name,name.c_str());
	customer.age = args[1]->Int32Value();	
	int32_t phone_no = args[2]->Int32Value();
	strcpy(customer.phone_no,std::to_string(phone_no).c_str());	
	v8::String::Utf8Value param2(args[3]->ToString());
	std::string address = std::string(*param2);
	strcpy(customer.address,address.c_str());
	v8::String::Utf8Value param3(args[4]->ToString());
	std::string passphrase = std::string(*param3);
	strcpy(customer.passphrase,passphrase.c_str());
	v8::String::Utf8Value param4(args[5]->ToString());
	std::string security_qn = std::string(*param4);
	strcpy(customer.security_question,security_qn.c_str());
	v8::String::Utf8Value param5(args[6]->ToString());
	std::string security_ans = std::string(*param5);
	strcpy(customer.security_answer,security_ans.c_str());	
	customer.balance = 1000;
	gettimeofday(&customer.last_accessed_time);
	int acc_no = write_customer_into_db(customer);
	cout<<"Your account number is "<<acc_no;
	customer.acc_no = acc_no;
	struct transaction initial_transaction;
	initial_transaction.acc_no = customer.acc_no;
	transactions.push_back(initial_transaction);
	//cout<<"Operator id is "<<current_operator->id<<"\n";
	current_operator->customer_acc_no_list[current_operator->no_of_customers] = customer.acc_no;
	//cout<<"customer under operator is "<<current_operator->customer_acc_no_list[0];
	current_operator->no_of_customers++;
	struct customer_info customer_information;
	customer_information.acc_no = customer.acc_no;
	customer_infos.push_back(customer_information);
	srand(1);
	write_files();
	srand(1);
	load_files();
}

//Update account details (phone_no and address for existing customer)
void update_account(const FunctionCallbackInfo<Value>& args){
	Isolate* isolate = args.GetIsolate();
	int acc_no = args[0]->Int32Value();
	int j;
	for(j = 0;j < customer_list.size();j++) {
		if(customer_list[j].acc_no == acc_no) {
			customer_list.push_back(customer_list[j]);
			customer_list.erase(customer_list.begin() +  j );
			customer_frequency.push_back(customer_frequency[j]);
			customer_frequency.erase(customer_frequency.begin() + j);
			break;
		}
	}
	if(j == customer_list.size())
		read_customer(acc_no);
	int i = find_customer_position(acc_no);
	if(i == -1)
		return;
	v8::String::Utf8Value param1(args[1]->ToString());
	std::string option = std::string(*param1);
	if(!strcmp("phone",option.c_str())) {
		int32_t phone_no = args[2]->Int32Value();
		strcpy(customer_list[i].phone_no,std::to_string(phone_no).c_str());
	}
	else {
		v8::String::Utf8Value param2(args[3]->ToString());
		std::string address = std::string(*param2);
		strcpy(customer_list[i].address,address.c_str());
	}
	gettimeofday(&customer_list[i].last_accessed_time);
	//customer_list[i].frequency++;
	customer_frequency[i]++;
	srand(1);
	write_files();
	update_customer(customer_list[i]);
	srand(1);
	load_files();
}


//Delete account of a user based on account number
void delete_account(const FunctionCallbackInfo<Value>& args) {
	//int curr_customer;
	int acc_no = args[0]->Int32Value();
	int operator_id = find_operator(acc_no);
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(acc_no);
	int i = find_customer_position(acc_no);
	if(i == -1) 
		return;
	/*if(!is_customer_under_current_operator(i) && !current_operator->is_admin ) {
		cout<<"The customer is under a different operator";
		return;
	}*/
	v8::String::Utf8Value param1(args[1]->ToString());
	std::string operator_password = std::string(*param1);
	if(!is_operator_password_correct(operator_password))
		return;	
	customer_details customer = customer_list[i];
	customer.is_active = false;
	customer_list.erase(customer_list.begin()+i);	
	cout<<"Account deleted successfully";
	int operator_position = find_operator_position(operator_id);
	bank_operator* op = & operators[operator_position];
	int j;
	for(j = 0;j < op->no_of_customers;j++) {
		if(op->customer_acc_no_list[j] == acc_no) {
			break;
		}
	}
	std::copy((op->customer_acc_no_list).begin() + j + 1,(op->customer_acc_no_list).begin() + 51,(op->customer_acc_no_list).begin() + j);
	op->no_of_customers--;
	update_customer(customer);
	srand(1);
	write_files();
	srand(1);
	load_files();
}


//Display a specific customer
void display_customer(const FunctionCallbackInfo<Value>& args) {
	int acc_no = args[0]->Int32Value();
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(acc_no);
	int i = find_customer_position(acc_no);
	if(i == -1) 
		return;
	Isolate* isolate = args.GetIsolate();
	Local<Object> obj = Object::New(isolate);
	obj->Set(String::NewFromUtf8(isolate, "name"), 
		String::NewFromUtf8(isolate, customer_list[i].customer_name));
	obj->Set(String::NewFromUtf8(isolate, "acc_no"), 
		Number::New(isolate, customer_list[i].acc_no));
	obj->Set(String::NewFromUtf8(isolate, "age"), 
		Number::New(isolate, customer_list[i].age));
	obj->Set(String::NewFromUtf8(isolate, "phone_no"), 
		String::NewFromUtf8(isolate, customer_list[i].phone_no));
	obj->Set(String::NewFromUtf8(isolate, "address"), 
		String::NewFromUtf8(isolate, customer_list[i].address));
	obj->Set(String::NewFromUtf8(isolate, "balance"), 
		Number::New(isolate, customer_list[i].balance));
	args.GetReturnValue().Set(obj);
}


void display_all(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Local<Array> result_list = Array::New(isolate);
	if(current_operator->is_admin) {
		load_customers();
		for (unsigned i=0; i<customer_list.size(); ++i) {
			if (is_customer_under_current_operator(i) || current_operator->is_admin) {
				Local<Object> obj = Object::New(isolate);
				obj->Set(String::NewFromUtf8(isolate, "name"), 
					String::NewFromUtf8(isolate, customer_list[i].customer_name));
				obj->Set(String::NewFromUtf8(isolate, "acc_no"), 
					Number::New(isolate, customer_list[i].acc_no));
				obj->Set(String::NewFromUtf8(isolate, "age"), 
					Number::New(isolate, customer_list[i].age));
				obj->Set(String::NewFromUtf8(isolate, "phone_no"), 
					String::NewFromUtf8(isolate, customer_list[i].phone_no));
				obj->Set(String::NewFromUtf8(isolate, "address"), 
					String::NewFromUtf8(isolate, customer_list[i].address));
				obj->Set(String::NewFromUtf8(isolate, "balance"), 
					Number::New(isolate, customer_list[i].balance));
				result_list->Set(i,obj);
			}
		}
	}
	else {
		for(int i = 0;i<current_operator->no_of_customers;i++) {
			int k;
			int l = 0;
			for(k = 0;k < customer_list.size();k++) {
				if(customer_list[k].acc_no == current_operator->customer_acc_no_list[i]) {
					customer_list.push_back(customer_list[k]);
					customer_list.erase(customer_list.begin() +  k );
					customer_frequency.push_back(customer_frequency[k]);
					customer_frequency.erase(customer_frequency.begin() + k);
					break;
				}
			}
			if(k == customer_list.size())
				read_customer(current_operator->customer_acc_no_list[i]);
			int j= find_customer_position(current_operator->customer_acc_no_list[i]);
			Local<Object> obj = Object::New(isolate);
				obj->Set(String::NewFromUtf8(isolate, "name"), 
					String::NewFromUtf8(isolate, customer_list[j].customer_name));
				obj->Set(String::NewFromUtf8(isolate, "acc_no"), 
					Number::New(isolate, customer_list[j].acc_no));
				obj->Set(String::NewFromUtf8(isolate, "age"), 
					Number::New(isolate, customer_list[j].age));
				obj->Set(String::NewFromUtf8(isolate, "phone_no"), 
					String::NewFromUtf8(isolate, customer_list[j].phone_no));
				obj->Set(String::NewFromUtf8(isolate, "address"), 
					String::NewFromUtf8(isolate, customer_list[j].address));
				obj->Set(String::NewFromUtf8(isolate, "balance"), 
					Number::New(isolate, customer_list[j].balance));
				result_list->Set(l,obj);
			l++;
		}
	}
	customer_list.clear();
	args.GetReturnValue().Set(result_list);

}


void deposit(const FunctionCallbackInfo<Value>& args) {
	int32_t acc_no = args[0]->Int32Value();
	cout<<acc_no<<"\n";
	int j;
	customer_details customer;
	for(j = 0;j < get_cache_size();j++) {
		customer = customer_list[j];
		if(customer.acc_no == acc_no) {
			int i = find_customer_position(acc_no);
			customer_list.push_back(customer_list[i]);
			customer_list.erase(customer_list.begin() + i);
			customer_frequency.push_back(customer_frequency[i]);
			customer_frequency.erase(customer_frequency.begin() + i);
			break;
		}
	}
	if(j == get_cache_size())
		bool success = read_customer(acc_no);
	int i = find_customer_position(acc_no);
	if(i == -1)
		return;
	int32_t deposit_value = args[1]->Int32Value();
	cout<<deposit_value<<"\n";
	bool is_successful = deposit_money(i,deposit_value);
	cout<<"deposited\n";
	if(is_successful)
		record_deposit(acc_no,deposit_value);
	cout<<"successfully record_deposit\n";
	write_files();
	load_files();
}

void withdraw(const FunctionCallbackInfo<Value>& args) {
	int32_t acc_no = args[0]->Int32Value();
	int j;
	customer_details customer;
	for(j = 0;j < get_cache_size();j++) {
		customer = customer_list[j];
		if(customer.acc_no == acc_no) {
			int i = find_customer_position(acc_no);
			customer_list.push_back(customer_list[i]);
			customer_list.erase(customer_list.begin() + i);
			customer_frequency.push_back(customer_frequency[i]);
			customer_frequency.erase(customer_frequency.begin() + i);
			break;
		}
	}
	if(j == get_cache_size())
		bool success = read_customer(acc_no);
	cout<<"read\n";
	int i = find_customer_position(acc_no);
	if(i == -1)
		return;
	customer = customer_list[i];
	if(customer.wrong_attempts >= 3) {
		cout<<"Locked\nDeposit to unlock\n";
		return;
	}
	int32_t withdraw_value = args[1]->Int32Value();
	
	v8::String::Utf8Value param1(args[2]->ToString());
	std::string customer_passphrase = std::string(*param1);
	if(!is_passphrase_valid(customer_passphrase,i)) {
			customer_list[i].wrong_attempts++;
			update_customer(customer_list[i]);
			write_files();
			load_files();
			return;
	}
	
	v8::String::Utf8Value param2(args[3]->ToString());
	std::string operator_password = std::string(*param2);
	
	if(!is_operator_password_correct(operator_password))
		return;	
	bool is_successful = withdraw_money(i,withdraw_value);
	if(is_successful) {
		record_withdrawal(acc_no,withdraw_value);
	}
	write_files();
	load_files();
}


//Transfer money to another existing account
void transfer_money(const FunctionCallbackInfo<Value>& args) {
	int32_t withdraw_acc_no = args[0]->Int32Value();
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == withdraw_acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(withdraw_acc_no);
	int i = find_customer_position(withdraw_acc_no);
	if(i == -1) 
		return;
	int x = find_customer_info_position(withdraw_acc_no);
	if(customer_infos[x].no_of_transactions == MAX_NO_OF_TRANSACTIONS) {
		cout<<"Max number of transactions reached";
		return;
	}
	int amount = args[1]->Int32Value();
	cout<<amount<<"\n";
	while(amount <= 0) {
		cout<<"Enter valid amount\n";
		cin>>amount;
	}
	int acc_no;
	char phone_no[11];
	if(customer_list[i].balance - amount > 0) {
		/*cout<<"Enter mobile number of the receiver\t";
		cin>>phone_no;
		while(strlen(phone_no) !=4 || !(is_valid_no(phone_no))) {
			cout<<"Enter valid number(4 digits)\n";
			cin>>phone_no;
		}*/
		int32_t phone = args[2]->Int32Value();
		strcpy(phone_no,std::to_string(phone).c_str());
		vector<customer_details> customers = get_accounts_by_phone_no(phone_no);
		if(customers.empty()) {
			cout<<"Phone number doesn't exist";
			return;
		}
		timevall last_accessed_time = customers[0].last_accessed_time;
		acc_no = customers[0].acc_no;
		timevall time;
		for(int l = 0;l < customers.size();l++) {
			time = customers[l].last_accessed_time;
			if(((last_accessed_time.tv_sec - time.tv_sec)*1000 + (last_accessed_time.tv_usec - time.tv_usec)/1000.0f) < 0 ) {
				last_accessed_time = customers[l].last_accessed_time;
				acc_no = customers[l].acc_no;
			}
		}
		cout<<"The account number is "<<acc_no<<"\n";
		for(k = 0;k < customer_list.size();k++) {
			if(customer_list[k].acc_no == acc_no) {
				customer_list.push_back(customer_list[k]);
				customer_list.erase(customer_list.begin() +  k );
				customer_frequency.push_back(customer_frequency[k]);
				customer_frequency.erase(customer_frequency.begin() + k);
				break;
			}
		}
		if(k == customer_list.size())
			read_customer(acc_no);
		int i = find_customer_position(withdraw_acc_no);
		int j = find_customer_position(acc_no);
		if(j == -1) {
			return;
		}
		if(customer_list[i].wrong_attempts >= 3) {
			cout<<"Locked\nDeposit to unlock\n";
			return;
		}
		if(i == j || !strcmp(customer_list[i].phone_no,customer_list[j].phone_no)) {
			cout<<"You cannot transfer your money to yourself\n";
			return;
		}
		if(!withdraw_money(i,amount))
			return;
		gettimeofday(&customer_list[i].last_accessed_time);
		customer_frequency[i]++;
		deposit_money(j,amount);
		struct transaction deposit_transaction;
		deposit_transaction.timestamp = get_timestamp();
		deposit_transaction.acc_no = acc_no;
		strcpy(deposit_transaction.deposit,(to_string(amount)).c_str());
		deposit_transaction.balance = customer_list[j].balance;
		transactions.push_back(deposit_transaction);
		struct transaction withdraw_transaction;
		withdraw_transaction.timestamp = get_timestamp();
		withdraw_transaction.acc_no = withdraw_acc_no;
		strcpy(withdraw_transaction.withdraw,(to_string(amount)).c_str());
		withdraw_transaction.balance = customer_list[i].balance;
		transactions.push_back(withdraw_transaction);
		write_files();
		update_customer(customer_list[i]);
		update_customer(customer_list[j]);
		load_files();
	}
	else 
		cout<<"You do not have sufficient balance";
}


//Schedules transfer in future time
void schedule_transfer(const FunctionCallbackInfo<Value>& args) {
	int withdraw_acc_no = args[0]->Int32Value();
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == withdraw_acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(withdraw_acc_no);
	int i = find_customer_position(withdraw_acc_no);
	if(i == -1) 
		return;
	/*if(!is_customer_under_current_operator(i) && !current_operator->is_admin ) {
		cout<<"The customer is under a different operator";
		return;
	}*/
	int amount = args[1] -> Int32Value();
	while(amount <= 0) {
		cout<<"Enter valid amount\n";
		cin>>amount;
	}
	int acc_no;	
	if(customer_list[i].balance - amount > 0) {
		//cout<<"Enter account number of the receiver\t";		
		acc_no = args[2]->Int32Value();
		for(k = 0;k < customer_list.size();k++) {
			if(customer_list[k].acc_no == acc_no) {
				customer_list.push_back(customer_list[k]);
				customer_list.erase(customer_list.begin() +  k );
				customer_frequency.push_back(customer_frequency[k]);
				customer_frequency.erase(customer_frequency.begin() + k);
				break;
			}
		}
		if(k == customer_list.size())
			read_customer(acc_no);
		int i = find_customer_position(withdraw_acc_no);
		int j = find_customer_position(acc_no);
		if(j == -1) {
			return;
		}
		if(customer_list[i].wrong_attempts >= 3) {
			cout<<"Locked\nDeposit to unlock\n";
			return;
		}
		if(i == j) {
			cout<<"You cannot transfer your money to yourself\n";
			return;
		}
		v8::String::Utf8Value param1(args[3]->ToString());
		std::string customer_passphrase = std::string(*param1);
		if(!is_passphrase_valid(customer_passphrase,i)) {
			/*cout<<"Do you want to use the forget password option ?\n0->No(default)   1->Yes\t";
			int choice;
			cin>>choice;
			int is_changed;
			if(choice == 1) {
				is_changed = forgot_password(i);
				if(!is_changed)
					return;
			}
			else {*/
				customer_list[i].wrong_attempts++;
				update_customer(customer_list[i]);
				return;
		}
		v8::String::Utf8Value param2(args[4]->ToString());
		std::string operator_password = std::string(*param2);
		if(!is_operator_password_correct(operator_password)) {
			return;
		}
		int hour,min;
		while(true) {
			//cout<<"Enter time (hh mm)\t";
			//get_time(&hour,&min);
			hour = args[5]->Int32Value();
			min = args[6]->Int32Value();
			if(hour >= get_timestamp().tm_hour) {
				if(hour == get_timestamp().tm_hour && min <= get_timestamp().tm_min) {
					cout<<"Enter future time\n";
					continue;
				}
				break;
			}
		}
		gettimeofday(&customer_list[i].last_accessed_time);
		//customer_list[i].frequency++;
		customer_frequency[i]++;
		tm time = get_timestamp();
		time.tm_min = min;
		time.tm_hour = hour;
		time.tm_sec = 0;
		struct transaction withdraw_transaction;
		withdraw_transaction.timestamp = time;
		withdraw_transaction.acc_no = withdraw_acc_no;
		strcpy(withdraw_transaction.withdraw,(to_string(amount)).c_str());
		withdraw_transaction.is_completed = true;
		pending_transactions.push_back(withdraw_transaction);
		struct transaction deposit_transaction;
		deposit_transaction.timestamp = time;
		deposit_transaction.acc_no = acc_no;
		strcpy(deposit_transaction.deposit,(to_string(amount)).c_str());
		deposit_transaction.is_completed = true;
		pending_transactions.push_back(deposit_transaction);
		//cout<<"Less or not "<<is_timestamp_less_than(time,next_time)<<"\n";
		cout<<"Before time is "<<next_time.tm_hour<<" : "<<next_time.tm_min<<" and "<<time.tm_hour<<" : "<<time.tm_min<<"\n";
		if(pending_transactions.size() == 2  || is_timestamp_less_than(time,next_time)) {
			next_time = time;
			//cout<<"NExt time is "<<next_time.tm_hour<<" : "<<next_time.tm_min;
			//cout<< put_time(&next_time,"%X") << '\n';
			struct tm current = get_timestamp();
			timer.Stop();
			timer = CTimer(listener, (mktime(&time) - mktime(&current))*1000);
			timer.Start();
		}
	}
	else {
		cout<<"You do not have sufficient balance";
	}
	srand(1);
	write_files();
	srand(1);
	load_files();
}


//Prints the account statement of a user based on account number
void print_account_statement(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Local<Array> result_list = Array::New(isolate);
	int j = 0;
	int acc_no = args[0]->Int32Value();
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(acc_no);
	if(find_customer_position(acc_no) == -1) {
		return;
	}
	//cout<<"\nAcc no\t\tDeposit\t\tWithdraw\tBalance\t\tTime\n";
	for(int i = 0;i < transactions.size();i++) {
		if(transactions[i].acc_no != acc_no)
			continue;
		Local<Object> obj = Object::New(isolate);
		obj->Set(String::NewFromUtf8(isolate, "acc_no"), 
			Number::New(isolate, transactions[i].acc_no));
		obj->Set(String::NewFromUtf8(isolate, "deposit"), 
			String::NewFromUtf8(isolate, transactions[i].deposit));
		obj->Set(String::NewFromUtf8(isolate, "withdraw"), 
			String::NewFromUtf8(isolate, transactions[i].withdraw));
		obj->Set(String::NewFromUtf8(isolate, "balance"), 
			Number::New(isolate, transactions[i].balance));
		obj->Set(String::NewFromUtf8(isolate, "time"), 
			String::NewFromUtf8(isolate, get_timestamp_string(transactions[i].timestamp).c_str()));
		result_list->Set(j,obj);
		j++;
	}
	args.GetReturnValue().Set(result_list);
}


//Prints the account summary in the given range
void print_account_statement_in_range(const FunctionCallbackInfo<Value>& args) {
	int acc_no = args[0]->Int32Value();
	int start_hour = args[1]->Int32Value();
	int start_min = args[2]->Int32Value();
	int stop_hour = args[3]->Int32Value();
	int stop_min = args[4]->Int32Value();
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(acc_no);
	if(find_customer_position(acc_no) == -1) {
		return;
	}
	bool valid_time;
	Isolate* isolate = args.GetIsolate();
	Local<Array> result_list = Array::New(isolate);
	int j = 0;
	for(int i = 0;i < transactions.size();i++) {
		if(transactions[i].acc_no != acc_no)
			continue;
		valid_time = false;
		if(!is_timestamp_in_range(start_hour,start_min,stop_hour,stop_min,transactions[i].timestamp))
			continue;
		Local<Object> obj = Object::New(isolate);
		obj->Set(String::NewFromUtf8(isolate, "acc_no"), 
			Number::New(isolate, transactions[i].acc_no));
		obj->Set(String::NewFromUtf8(isolate, "deposit"), 
			String::NewFromUtf8(isolate, transactions[i].deposit));
		obj->Set(String::NewFromUtf8(isolate, "withdraw"), 
			String::NewFromUtf8(isolate, transactions[i].withdraw));
		obj->Set(String::NewFromUtf8(isolate, "balance"), 
			Number::New(isolate, transactions[i].balance));
		obj->Set(String::NewFromUtf8(isolate, "time"), 
			String::NewFromUtf8(isolate, get_timestamp_string(transactions[i].timestamp).c_str()));
		result_list->Set(j,obj);
		j++;
	}
	args.GetReturnValue().Set(result_list);
}


//Adds standing instructions
void add_standing_transactions(const FunctionCallbackInfo<Value>& args) {
	int withdraw_acc_no = args[0]->Int32Value();;
	int k;
	for(k = 0;k < customer_list.size();k++) {
		if(customer_list[k].acc_no == withdraw_acc_no) {
			customer_list.push_back(customer_list[k]);
			customer_list.erase(customer_list.begin() +  k );
			customer_frequency.push_back(customer_frequency[k]);
			customer_frequency.erase(customer_frequency.begin() + k);
			break;
		}
	}
	if(k == customer_list.size())
		read_customer(withdraw_acc_no);
	int i = find_customer_position(withdraw_acc_no);
	if(i == -1) 
		return;
	/*if(!is_customer_under_current_operator(i) && !current_operator->is_admin ) {
		cout<<"The customer is under a different operator";
		return;
	}*/
	int amount = args[1]->Int32Value();
	while(amount <= 0) {
		cout<<"Enter valid amount\n";
		cin>>amount;
	}
	int acc_no;	
	if(customer_list[i].balance - amount > 0) {
		acc_no = args[2]->Int32Value();
		for(k = 0;k < customer_list.size();k++) {
			if(customer_list[k].acc_no == acc_no) {
				customer_list.push_back(customer_list[k]);
				customer_list.erase(customer_list.begin() +  k );
				customer_frequency.push_back(customer_frequency[k]);
				customer_frequency.erase(customer_frequency.begin() + k);
				break;
			}
		}
		if(k == customer_list.size())
			read_customer(acc_no);
		i = find_customer_position(withdraw_acc_no);
		int j = find_customer_position(acc_no);
		if(j == -1) {
			return;
		}
		if(customer_list[i].wrong_attempts >= 3) {
			cout<<"Locked\nDeposit to unlock\n";
			return;
		}
		if(i == j) {
			cout<<"You cannot transfer your money to yourself\n";
			return;
		}
		v8::String::Utf8Value param1(args[3]->ToString());
		std::string customer_passphrase = std::string(*param1);
		if(!is_passphrase_valid(customer_passphrase,i)) {
			/*cout<<"Do you want to use the forget password option ?\n0->No(default)   1->Yes\t";
			int choice;
			cin>>choice;
			int is_changed;
			if(choice == 1) {
				is_changed = forgot_password(i);
				if(!is_changed)
					return;
			}
			else {*/
				customer_list[i].wrong_attempts++;
				update_customer(customer_list[i]);
				return;
			//}
		}
		v8::String::Utf8Value param2(args[4]->ToString());
		std::string operator_password = std::string(*param2);
		if(!is_operator_password_correct(operator_password)) {
			return;
		}
		int hour,min;
		//while(true) {
			hour = args[5]->Int32Value();
			min = args[6]->Int32Value();
			if(hour >= get_timestamp().tm_hour) {
				if(hour == get_timestamp().tm_hour && min <= get_timestamp().tm_min) {
					cout<<"Enter future time\n";
					return;
				}
			}
		//}
		int period = args[7]->Int32Value();
		tm time = get_timestamp();
		time.tm_min = min;
		time.tm_hour = hour;
		time.tm_sec = 0;
		struct transaction withdraw_transaction;
		withdraw_transaction.timestamp = time;
		withdraw_transaction.acc_no = withdraw_acc_no;
		strcpy(withdraw_transaction.withdraw,(to_string(amount)).c_str());
		withdraw_transaction.is_completed = false;
		withdraw_transaction.period = period;
		pending_transactions.push_back(withdraw_transaction);
		struct transaction deposit_transaction;
		deposit_transaction.timestamp = time;
		deposit_transaction.acc_no = acc_no;
		strcpy(deposit_transaction.deposit,(to_string(amount)).c_str());
		deposit_transaction.is_completed = false;
		deposit_transaction.period = period;
		pending_transactions.push_back(deposit_transaction);
		//cout<<"Before time is "<<next_time.tm_hour<<" : "<<next_time.tm_min;

		if(pending_transactions.size() == 2 || is_timestamp_less_than(time,next_time)) {
			next_time = time;
			timer.Stop();	
			//cout<<"After time is "<<next_time.tm_hour<<" : "<<next_time.tm_min;
			struct tm current = get_timestamp();
			timer = CTimer(listener, (mktime(&time) - mktime(&current))*1000);
			timer.Start();
		}
	}
	else {
		cout<<"You do not have sufficient balance";
	}
	srand(1);
	write_files();
	srand(1);
	load_files();
	
}


//Do fixed deposit
void fixed_deposit(const FunctionCallbackInfo<Value>& args) {
	int32_t acc_no = args[0]->Int32Value();
	int j;
	struct customer_details customer;
	struct fixed_deposit deposit;
	for(j = 0;j < customer_list.size();j++) {
		if(customer_list[j].acc_no == acc_no) {
			customer_list.push_back(customer_list[j]);
			customer_list.erase(customer_list.begin()+j);
			customer_frequency.push_back(customer_frequency[j]);
			customer_frequency.erase(customer_frequency.begin() + j);
			break;
		}
	}
	if(j == customer_list.size())
		read_customer(acc_no);
	int i = find_customer_position(acc_no);
	if(i == -1)
		return;
	//int deposit_value,duration;
	deposit.principal = args[1]->Int32Value();
	while(deposit.principal <= 0) {
		cout<<"Enter valid amount\n";
		cin>>deposit.principal;
	}
	if(customer_list[i].balance >= deposit.principal) {
		customer_list[i].balance -= deposit.principal;
	}
	deposit.duration = args[2]->Int32Value();
	while(deposit.duration <= 0) {
		cout<<"Enter valid duration\n";
		cin>>deposit.duration;
	}
	if(customer_list[i].age<=60)
		deposit.rate_of_interest = INTEREST_NORMAL;
	else
		deposit.rate_of_interest = INTEREST_SENIOR;
	deposit.acc_no = acc_no;
	
	deposit.amount = deposit.principal + (deposit.principal*deposit.duration*deposit.rate_of_interest)/100;
	cout<<"Your interest will be "<<deposit.amount - deposit.principal<<"  and your maturity amount will be "<<deposit.amount<<"\n";
	tm current_time = get_timestamp();
	deposit.start_time = current_time;
	struct transaction withdraw_transaction;
	withdraw_transaction.timestamp = deposit.start_time;
	withdraw_transaction.acc_no = acc_no;
	strcpy(withdraw_transaction.withdraw,(to_string(deposit.principal)).c_str());
	withdraw_transaction.is_completed = true;
	cout<<"Your balance is "<<customer_list[i].balance<<"\n";
	withdraw_transaction.balance = customer_list[i].balance;
	transactions.push_back(withdraw_transaction);
	tm maturity_time = add_minutes_to_timestamp(current_time,deposit.duration);
	deposit.maturity_time = maturity_time;
	deposits.push_back(deposit);	
	cout<<"Maturity at "<<deposit.maturity_time.tm_hour<<" : "<<deposit.maturity_time.tm_min<<" : "<<deposit.maturity_time.tm_sec<<"\n";
	if(is_timestamp_less_than(deposit.maturity_time,next_fd_time) || deposits.size() == 1 || 
		(is_timestamp_equal(deposit.maturity_time,next_fd_time) && (deposit.maturity_time.tm_sec < next_fd_time.tm_sec))) {
		next_fd_timer.Stop();
		next_fd_time = deposit.maturity_time;
		next_fd_timer = CTimer(clear_fd, (mktime(&maturity_time) - mktime(&current_time))*1000);
		next_fd_timer.Start();
	}
	srand(1);
	write_files();
	update_customer(customer_list[i]);
	srand(1);
	load_files();
}

void get_security_question(const FunctionCallbackInfo<Value>& args) {
	int acc_no = args[0]->Int32Value();
	int j;
	for(j = 0;j < customer_list.size();j++) {
		if(customer_list[j].acc_no == acc_no) {
			customer_list.push_back(customer_list[j]);
			customer_list.erase(customer_list.begin()+j);
			customer_frequency.push_back(customer_frequency[j]);
			customer_frequency.erase(customer_frequency.begin() + j);
			break;
		}
	}
	if(j == customer_list.size())
		read_customer(acc_no);
		int i = find_customer_position(acc_no);
		if(i == -1)
			return;
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, customer_list[i].security_question));
}

void forgot_password(const FunctionCallbackInfo<Value>& args) {
	int acc_no = args[0]->Int32Value();
	v8::String::Utf8Value param2(args[1]->ToString());
	std::string answer = std::string(*param2);
	int i = find_customer_position(acc_no);
	Isolate* isolate = args.GetIsolate();
	if(!strcmp(answer.c_str(),customer_list[i].security_answer)) 
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
	else
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));	
}

void change_password(const FunctionCallbackInfo<Value>& args) {
	int acc_no = args[0]->Int32Value();
	int i = find_customer_position(acc_no);
	v8::String::Utf8Value param1(args[1]->ToString());
	std::string passphrase = std::string(*param1);
	strcpy(customer_list[i].passphrase,passphrase.c_str());
	customer_list[i].wrong_attempts = 0;
	update_customer(customer_list[i]);
}


void is_valid_account(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	int acc_no = args[0]->Int32Value();
	bool is_present;
	int j;
	for(j = 0;j < customer_list.size();j++) {
		if(customer_list[j].acc_no == acc_no) {
			customer_list.push_back(customer_list[j]);
			customer_list.erase(customer_list.begin()+j);
			customer_frequency.push_back(customer_frequency[j]);
			customer_frequency.erase(customer_frequency.begin() + j);
			is_present = true;
			break;
		}
	}
	if(j == customer_list.size())
		is_present = read_customer(acc_no);
	if(!is_present) {
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));
		return;
	}
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
}


void is_passphrase_valid(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	int acc_no = args[0]->Int32Value();
	int j;
	for(j = 0;j < customer_list.size();j++) {
		if(customer_list[j].acc_no == acc_no) {
			customer_list.push_back(customer_list[j]);
			customer_list.erase(customer_list.begin()+j);
			customer_frequency.push_back(customer_frequency[j]);
			customer_frequency.erase(customer_frequency.begin() + j);
			break;
		}
	}
	if(j == customer_list.size())
		read_customer(acc_no);
	int i = find_customer_position(acc_no);
	v8::String::Utf8Value param1(args[1]->ToString());
	std::string passphrase = std::string(*param1);
	if(strcmp(passphrase.c_str(),customer_list[i].passphrase)) {
		cout<<"You have entered a wrong passphrase\n";
		customer_list[i].wrong_attempts++;
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));
		return;
	}
	customer_list[i].wrong_attempts = 0;
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
	return;
}


void is_operator_password_correct(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	v8::String::Utf8Value param1(args[0]->ToString());
	std::string password = std::string(*param1);
	if(strcmp(password.c_str(),current_operator->password)) {
		cout<<"You have entered wrong password\n";
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));
		return;
	}
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
	return;
}


void is_max_transactions_reached(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	int acc_no = args[0]->Int32Value();
	int j = find_customer_info_position(acc_no);
	if(customer_infos[j].no_of_transactions == 2) {
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
		return;
	}
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));
	return;
}

void is_phone_no_valid(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	char phone_no[11];
	int32_t phone = args[0]->Int32Value();
	strcpy(phone_no,std::to_string(phone).c_str());
	vector<customer_details> customers = get_accounts_by_phone_no(phone_no);
	if(customers.empty()) {
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));
		return;
	}
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
	return;
}

void is_customer_under_current_operator(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	int acc_no = args[0]->Int32Value();
	int j;
	for(j = 0;j < customer_list.size();j++) {
		if(customer_list[j].acc_no == acc_no) {
			customer_list.push_back(customer_list[j]);
			customer_list.erase(customer_list.begin()+j);
			customer_frequency.push_back(customer_frequency[j]);
			customer_frequency.erase(customer_frequency.begin() + j);
			break;
		}
	}
	if(j == customer_list.size())
		read_customer(acc_no);
	int i = find_customer_position(acc_no);
	bool status = false;
	for(int j = 0;j < current_operator->no_of_customers;j++) {
		if(current_operator->customer_acc_no_list[j] == customer_list[i].acc_no) {
			status = true;
			break;
		}
	}
	if(!current_operator->is_admin && !status) {
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "false"));
		return;
	}
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "true"));
}

void callMain(const FunctionCallbackInfo<Value>& args) {
	main();
}	
	
void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "main", callMain);
	NODE_SET_METHOD(exports, "add_account", add_account);
	NODE_SET_METHOD(exports, "add_operator", add_operator);
	NODE_SET_METHOD(exports, "login", operator_login);
	NODE_SET_METHOD(exports, "update_account", update_account);
	NODE_SET_METHOD(exports, "delete_account", delete_account);
	NODE_SET_METHOD(exports, "display", display_customer);
	NODE_SET_METHOD(exports, "display_all", display_all);
	NODE_SET_METHOD(exports, "deposit", deposit);
	NODE_SET_METHOD(exports, "withdraw", withdraw);
	NODE_SET_METHOD(exports, "transfer_money", transfer_money);
	NODE_SET_METHOD(exports, "schedule_transfer", schedule_transfer);
	NODE_SET_METHOD(exports, "print_account_statement", print_account_statement);
	NODE_SET_METHOD(exports, "print_account_statement_in_range",print_account_statement_in_range);
	NODE_SET_METHOD(exports, "standing_instructions", add_standing_transactions);
	NODE_SET_METHOD(exports, "fixed_deposit", fixed_deposit);
	NODE_SET_METHOD(exports, "get_security_question", get_security_question);
	NODE_SET_METHOD(exports, "forgot_password", forgot_password);
	NODE_SET_METHOD(exports, "change_password", change_password);
	NODE_SET_METHOD(exports, "is_valid_account", is_valid_account);
	NODE_SET_METHOD(exports, "is_passphrase_valid", is_passphrase_valid);
	NODE_SET_METHOD(exports, "is_operator_password_correct", is_operator_password_correct);
	NODE_SET_METHOD(exports, "is_max_transactions_reached", is_max_transactions_reached);
	NODE_SET_METHOD(exports, "is_phone_no_valid", is_phone_no_valid);
	NODE_SET_METHOD(exports, "is_customer_under_current_operator", is_customer_under_current_operator);
}

NODE_MODULE(demo1, init)

}  
//-static-libgcc -std=c++14 -std=gnu++14
//-std=c++14 -std=gnu++14
