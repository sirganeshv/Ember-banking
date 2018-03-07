public class Customer {
	int acc_no;
	String customer_name;
	int age;
	String phone_no;
	String address;
	int balance;
	String passphrase;
	int wrong_attempts;
	boolean is_active;
	public Customer() {
		wrong_attempts = 0;
		is_active = true;
	}
	
}