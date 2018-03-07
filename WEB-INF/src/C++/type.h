#include <array>
#include <vector>
#include <string>
#include <time.h>
#include <stdint.h>
#include "WinTimer.h"

struct customer_details {
	int acc_no;
	char customer_name[20];
	unsigned int age;
	char phone_no[11];
	char address[30];
	int balance;
	char passphrase[10];
	int wrong_attempts;
	bool is_active;
	char security_question[50];
	char security_answer[30];
	struct timevall last_accessed_time;
	customer_details():wrong_attempts(0),is_active(true) { }
};

struct bank_operator {
	char name[20];
	int id;
	char password[10];
	array<int, 50> customer_acc_no_list;
	int no_of_customers;
	bool is_admin;
	bank_operator() : is_admin(false),no_of_customers(0) { 
	}
};

struct transaction {
	int acc_no;
	char deposit[10];
	char withdraw[10];
	int balance;
	int period;
	transaction(): balance(1000),is_completed(true),period(0)
    {
		timestamp = get_timestamp();
		strcpy(deposit,("----"));
		strcpy(withdraw,("----"));
    }
	bool is_completed;
	tm timestamp;
};

struct fixed_deposit {
	int acc_no;
	int duration;
	int rate_of_interest;
	int principal;
	tm start_time;
	tm maturity_time;
	int amount;
};

struct customer_info {
	int acc_no;
	int amount_withdrawn;
	int no_of_transactions;
	tm standing_time;
	customer_info(): amount_withdrawn(0),no_of_transactions(0) {}
};


extern vector<transaction> transactions;
extern vector<customer_details> customer_list;
extern vector<transaction> pending_transactions;
extern vector<bank_operator> operators;
extern bank_operator * current_operator;
extern vector<customer_info> customer_infos;
extern vector<fixed_deposit> deposits;
extern vector<int> customer_frequency;

extern int find_operator(int acc_no);
extern int find_customer_position(int acc_no);
extern string find_operator_password(int position);
extern bool is_customer_under_current_operator(int position);
extern int find_operator_position(int id);
extern tm get_timestamp_from_string(string time_str);
extern int next_acc_no;
extern tm last_clearing_time;
extern tm next_fd_time;
