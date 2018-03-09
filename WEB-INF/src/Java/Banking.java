import java.util.Scanner;
import javax.servlet.http.*;  
import javax.servlet.*;  
import java.io.*;  
import java.util.logging.Level;
import java.util.logging.Logger;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;
import org.json.simple.parser.JSONParser;
import org.json.*;


public class Banking extends HttpServlet{
	
	//private native void startMain();
	private native int get_acc_no();
	private native Customer get_customer_from_list(int position);
	private native void read_customer(int acc_no);
	private native int find_customer_position(int acc_no);
	//private native boolean is_customer_under_current_operator(int position);
	private native boolean is_current_operator_admin();
	private native void initialize();
	private native void add_account();
	private native void update_account();
	private native void display_customer(int position);
	private native void display_customer_list();
	//private native boolean deposit(int position,int deposit_value);
	private native void record_deposit(int acc_no,int deposit_value);
	private native boolean withdraw_money(int position,int withdraw_value);
	private native void record_withdrawal(int acc_no,int withdraw_value);
	//private native void transfer_money();
	//private native void print_account_statement();
	private native void print_account_summary_in_range();
	//private native void schedule_transfer();
	private native void write_files();
	private native void print_pending_transactions();
	private native void push_back(int acc_no);
	private native void display_current_cache();
	private native int get_cache_size();
	private native void clear_cache();
	//private native void add_standing_transactions();
	//private native void fixed_deposit();
	
	
	private native boolean is_valid_account(int acc_no);
	private native boolean is_customer_under_current_operator(int acc_no);
	private native boolean is_operator_password_correct(String password);
	
	private native boolean login(int id,String password);
	private native boolean add_operator(String name,int id,String password,String is_admin);
	private native int add_customer(String name,int age,String phone,String address,String passphrase,String security_qn,String security_ans);
	private native void update_customer(int acc_no,String details,String phone,String address);
	private native void delete_account(int acc_no,String password);
	private native Customer display(int acc_no);
	private native Customer[] display_all();
	private native void deposit(int acc_no,int money);
	private native boolean withdraw(int acc_no,int money,String customer_passphrase,String operator_password);
	private native void transfer_money(int acc_no,int money,String phone);
	private native void schedule_transfer(int withdraw_acc_no,int money,int acc_no,String customer_passphrase,String operator_password,int hour,int min);
	private native Transaction[] print_account_statement(int acc_no);
	private native Transaction[] print_account_statement_range(int acc_no,int start_hour,int start_min,int stop_hour,int stop_min);
	private native void add_standing_transactions(int withdraw_acc_no,int money,int acc_no,String customer_passphrase,String operator_password,int hour,int min,int period);
	private native String fixed_deposit(int acc_no,int money,int duration);
	private native String get_security_question(int acc_no);
	private native boolean forgot_password(int acc_no,String answer);
	private native void change_password(int acc_no,String passphrase);

	private native boolean is_passphrase_valid(int acc_no,String customer_passphrase);
	private native boolean is_max_transactions_reached(int acc_no);
	private native boolean is_phone_no_valid(String phone_no);
	
	
	
	private static final String LOGIN = "/login";
	private static final String ADD_OPERATOR = "/add_operator";
	private static final String ADD_CUSTOMER = "/add_customer";
	private static final String UPDATE_CUSTOMER = "/update_customer";
	private static final String DELETE_ACCOUNT = "/delete_account";
	private static final String DISPLAY = "/display";
	private static final String DISPLAY_ALL = "/display_all";
	private static final String DEPOSIT = "/deposit";
	private static final String WITHDRAW = "/withdraw";
	private static final String TRANSFER = "/transfer";
	private static final String SCHEDULE_TRANSFER = "/schedule_transfer";
	private static final String STANDING_INSTRUCTIONS = "/standing_instructions";
	private static final String FIXED_DEPOSIT = "/fixed_deposit";
	private static final String FORGOT_PASSWORD = "/forgot_password";
	private static final String SECURITY = "/security";
	private static final String CHANGE_PASSWORD = "/change_password";
	private static final String ACCOUNT_STATEMENT = "/account_statement";
	private static final String ACCOUNT_STATEMENT_RANGE = "/account_statement_range";
	
	public void doGet(HttpServletRequest req,HttpServletResponse res)  
	throws ServletException,IOException {
		doProcess(req,res);
	}

	public void doPost(HttpServletRequest req,HttpServletResponse res)  
	throws ServletException,IOException  {  
		doProcess(req,res);
	}
	public void doProcess(HttpServletRequest req,HttpServletResponse res)  
	throws ServletException,IOException {	
		Banking bank = new Banking();
		bank.initialize();
		String path = req.getServletPath();
		res.setContentType("text/html");
		PrintWriter pw=res.getWriter();
		switch (path) {
			case LOGIN: {
				//getServletContext().log("Id of the operator is "+req.getParameter("ID"));
				int id = Integer.parseInt(req.getParameter("ID"));
				String password = req.getParameter("password");
				boolean status = bank.login(id,password);
				pw.println(status);
			}
			break;
			case ADD_OPERATOR: {
				int id = Integer.parseInt(req.getParameter("id"));
				String password = req.getParameter("pass");
				boolean status = false;
				status = bank.add_operator(req.getParameter("name"),id,password,req.getParameter("is_admin"));
				pw.println("Added");
			}
			break;
			case ADD_CUSTOMER: {
				int age = Integer.parseInt(req.getParameter("age"));
				int acc_no;
				acc_no = bank.add_customer(req.getParameter("name"),age,req.getParameter("phone"),req.getParameter("address"),
					req.getParameter("passphrase"),req.getParameter("security_qn"),req.getParameter("security_ans"));
				pw.println("Your account number is " + acc_no);
			}
			break;
			case UPDATE_CUSTOMER: {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				bank.update_customer(acc_no,req.getParameter("details"),req.getParameter("phone"),req.getParameter("address"));
				pw.println("Updated");
			}
			break;
			case DELETE_ACCOUNT : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				String password = req.getParameter("operator_password");
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid)
					pw.println("Account number is not valid");
				else if(!bank.is_operator_password_correct(password))
					pw.println("Operator password wrong");
				else {
					bank.delete_account(acc_no,password);
					pw.println("Account deleted successfully");
				}
			}
			break;
			case DISPLAY : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else { 
					Customer customer = bank.display(acc_no);
					//JSONArray obj = new JSONArray();
					JSONArray jcustomer = new JSONArray();
					jcustomer.add(customer.customer_name);
					jcustomer.add(customer.acc_no);
					jcustomer.add(customer.age);
					jcustomer.add(customer.phone_no);
					jcustomer.add(customer.address);
					jcustomer.add(customer.balance);
					pw.println(jcustomer);
				}
			}
			break;
			case DISPLAY_ALL : {
				getServletContext().log("display all inside");
				Customer[] customers = bank.display_all();
				JSONArray obj = new JSONArray();
				getServletContext().log("display all "+customers.length+" customers");
				for(int i = 0;i < customers.length;i++) {
					JSONArray customer = new JSONArray();
					customer.add(customers[i].customer_name);
					customer.add(customers[i].acc_no);
					customer.add(customers[i].age);
					customer.add(customers[i].phone_no);
					customer.add(customers[i].address);
					customer.add(customers[i].balance);
					obj.add(customer);
				}
				pw.println(obj);
			}
			break;
			case DEPOSIT : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				int money = Integer.parseInt(req.getParameter("money"));
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else if(money <= 0)
					pw.println("Enter valid amount");
				else {
					bank.deposit(acc_no,money);
					pw.println("Successfully deposited");
				}
			}
			break;
			case WITHDRAW : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				int money = Integer.parseInt(req.getParameter("money"));
				String customer_passphrase = req.getParameter("customer_passphrase");
				String operator_password = req.getParameter("operator_password");
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else if(money <= 0)
					pw.println("Enter valid amount");
				else if(!bank.is_passphrase_valid(acc_no,customer_passphrase))
					pw.println("Invalid passphrase");
				else if(!bank.is_operator_password_correct(operator_password))	
					pw.println("Operator password wrong");
				else if(bank.is_max_transactions_reached(acc_no)) 
					pw.println("Maximum transactions reached");
				else if(bank.withdraw(acc_no,money,customer_passphrase,operator_password))
					pw.println("Successfully withdrawn");
				else
					pw.println("Withdraw failed");
			}
			break;
			case TRANSFER : {
				int acc_no = Integer.parseInt(req.getParameter("withdraw_acc_no"));
				int money = Integer.parseInt(req.getParameter("money"));
				String phone = req.getParameter("phone_no");
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else if(money <= 0)
					pw.println("Enter valid amount");
				else if(bank.is_max_transactions_reached(acc_no)) 
					pw.println("Maximum transactions reached");
				else if(!bank.is_phone_no_valid(phone)) 
					pw.println("Enter a valid phone number");
				else {
					bank.transfer_money(acc_no,money,phone);
					pw.println("Transferred");
				}
			}
			break;
			case SCHEDULE_TRANSFER : {
				int withdraw_acc_no = Integer.parseInt(req.getParameter("withdraw_acc_no"));
				int money = Integer.parseInt(req.getParameter("money"));
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				String customer_passphrase = req.getParameter("customer_passphrase");
				String operator_password = req.getParameter("operator_password");
				int hour = Integer.parseInt(req.getParameter("hour"));
				int min = Integer.parseInt(req.getParameter("min"));
				boolean is_valid = bank.is_valid_account(withdraw_acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else if(!bank.is_valid_account(acc_no)) 
					pw.println("Receiver account number not valid");
				else if (withdraw_acc_no == acc_no) 
					pw.println("You cannot transfer money to yourself");
				else if(money <= 0)
					pw.println("Enter valid amount");
				else if(hour < 0 || hour >= 24 || min < 0 || min >= 60) 
					pw.println("Enter valid time");
				else if(!bank.is_passphrase_valid(withdraw_acc_no,customer_passphrase))
					pw.println("Invalid passphrase");
				else if(!bank.is_customer_under_current_operator(withdraw_acc_no)) 
					pw.println("The customer is under some other operator");
				else if(!bank.is_operator_password_correct(operator_password))	
					pw.println("Operator password wrong");
				else {
					bank.schedule_transfer(withdraw_acc_no,money,acc_no,customer_passphrase,operator_password,hour,min);
					pw.println("Scheduled for transfer");
				}
			}
			break;
			case ACCOUNT_STATEMENT : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else {
					Transaction[] transactions = bank.print_account_statement(acc_no);
					JSONArray obj = new JSONArray();
					for(int i = 0;i < transactions.length;i++) {
						JSONArray transaction = new JSONArray();
						transaction.add(transactions[i].acc_no);
						transaction.add(transactions[i].deposit);
						transaction.add(transactions[i].withdraw);
						transaction.add(transactions[i].balance);
						transaction.add(transactions[i].time);
						obj.add(transaction);
					}
					pw.println(obj);
				}
			}
			break;
			case ACCOUNT_STATEMENT_RANGE : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				int start_hour = Integer.parseInt(req.getParameter("start_hour"));
				int start_min = Integer.parseInt(req.getParameter("start_min"));
				int stop_hour = Integer.parseInt(req.getParameter("stop_hour"));
				int stop_min = Integer.parseInt(req.getParameter("stop_min"));
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else if(start_hour < 0 || start_hour >= 24 || start_min < 0 || start_min >= 60) 
					pw.println("Invalid time");
				else if(stop_hour < 0 || stop_hour >= 24 || stop_min < 0 || stop_min >= 60) 
					pw.println("Invalid time");
				else {
					Transaction[] transactions = bank.print_account_statement_range(acc_no,start_hour,start_min,stop_hour,stop_min);
					JSONArray obj = new JSONArray();
					for(int i = 0;i < transactions.length;i++) {
						JSONArray transaction = new JSONArray();
						transaction.add(transactions[i].acc_no);
						transaction.add(transactions[i].deposit);
						transaction.add(transactions[i].withdraw);
						transaction.add(transactions[i].balance);
						transaction.add(transactions[i].time);
						obj.add(transaction);
					}
					pw.println(obj);
				}
			}
			break;
			case STANDING_INSTRUCTIONS : {
				int withdraw_acc_no = Integer.parseInt(req.getParameter("withdraw_acc_no"));
				int money = Integer.parseInt(req.getParameter("money"));
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				String customer_passphrase = req.getParameter("customer_passphrase");
				String operator_password = req.getParameter("operator_password");
				int hour = Integer.parseInt(req.getParameter("hour"));
				int min = Integer.parseInt(req.getParameter("min"));
				int period = Integer.parseInt(req.getParameter("period"));
				boolean is_valid = bank.is_valid_account(withdraw_acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else if(!bank.is_valid_account(acc_no)) 
					pw.println("Receiver account number not valid");
				else if (withdraw_acc_no == acc_no) 
					pw.println("You cannot transfer money to yourself");
				else if(money <= 0)
					pw.println("Enter valid amount");
				else if(period <= 0)
					pw.println("Enter valid period(in minutes)");
				else if(hour < 0 || hour >= 24 || min < 0 || min >= 60) 
					pw.println("Enter valid time");
				else if(!bank.is_passphrase_valid(withdraw_acc_no,customer_passphrase))
					pw.println("Invalid passphrase");
				else if(!bank.is_customer_under_current_operator(withdraw_acc_no)) 
					pw.println("The customer is under some other operator");
				else if(!bank.is_operator_password_correct(operator_password))	
					pw.println("Operator password wrong");
				else {
					bank.add_standing_transactions(withdraw_acc_no,money,acc_no,customer_passphrase,operator_password,hour,min,period);
					pw.println("Added periodical transfer for every "+period+" minutes");
				}
			}
			break;
			case FIXED_DEPOSIT : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				int money = Integer.parseInt(req.getParameter("money"));
				int duration = Integer.parseInt(req.getParameter("duration"));
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else if(money <= 0)
					pw.println("Enter valid amount");
				else if(duration <= 0)
					pw.println("Enter valid duration");
				else {
					String maturity = bank.fixed_deposit(acc_no,money,duration);
					pw.println(maturity);
				}
			}
			break;
			case FORGOT_PASSWORD : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				boolean is_valid = bank.is_valid_account(acc_no);
				if(!is_valid) 
					pw.println("Account number is not valid");
				else {
					pw.println(bank.get_security_question(acc_no));
				}
			}	
			break;
			case SECURITY : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				String answer = req.getParameter("answer");
				pw.println(bank.forgot_password(acc_no,answer));
			}
			break;
			case CHANGE_PASSWORD : {
				int acc_no = Integer.parseInt(req.getParameter("acc_no"));
				String passphrase = req.getParameter("passphrase");
				bank.change_password(acc_no,passphrase);
				pw.println("Successfully changed passphrase");
			}
			break;
			default:
			break;
		}
		pw.close();
	}
		
		
	static {
		System.load("C:\\xampp\\tomcat\\webapps\\bank\\banking.dll");
	}
	
	/*public static void main(String[] args) {
		Banking bank = new Banking();
		bank.initialize();
		/*while(true) {
			System.out.println("\n\n1.Add account\t\t2.Update details\t3.Delete account\t4.Display customer(using Acc no)");
			System.out.println("5.Display cusomer list\t6.Add money\t\t7.Withdraw money\t8.Transfer money\t9.Acc statement");
			System.out.println("10.Acc summary(time)\t11.Schedule transfer\t12.Create operator\t13.Exit\t\t\t16.Clear cache	");
			System.out.println("18.Standing Instructions\t19.Fixed Deposit\t20.Forgot password");
			Scanner sc = new Scanner(System.in);
			int option = sc.nextInt();
			switch(option) {
				case 1: 
					bank.add_account();
				break;
				case 2:
					bank.update_account();
				break;
				case 3:
					bank.delete_account();
				break;
				case 4:
				{
					try 
					{	
						int acc_no = bank.get_acc_no();
						int k;
						Customer customer;
						for(k = 0;k < bank.get_cache_size();k++) {
							customer = bank.get_customer_from_list(k);
							if(customer == null)
								throw new ClassNotFoundException();
							if(customer.acc_no == acc_no) {
								bank.push_back(acc_no);
								break;
							}
						}
						if(k == bank.get_cache_size())
							bank.read_customer(acc_no);
						int i = bank.find_customer_position(acc_no);
						if(i == -1 ) 
							break;
						System.out.println("The position is "+i);
						if(!bank.is_customer_under_current_operator(i) && !bank.is_current_operator_admin()) {
							System.out.println("The customer is under some other operator");
							break;
						}
						System.out.println("Details of customer \n");
						System.out.println("\nName\t\tAcc No\t\tAge\t\tPhone\t\tAddress\t\tBalance\n");
						bank.display_customer(i);
					}
					catch(ClassNotFoundException exc) {
						System.out.println("Class cannot be found");
						break;
					}
				}
				break;
				case 5:
					bank.display_customer_list();
				break;
				case 6:
				{
					try {
						int acc_no = bank.get_acc_no();
						int j;
						Customer customer;
						for(j = 0;j < bank.get_cache_size();j++) {
							customer = bank.get_customer_from_list(j);
							if(customer == null)
								throw new ClassNotFoundException();
							if(customer.acc_no == acc_no) {
								bank.push_back(acc_no);
								break;
							}
						}
						if(j == bank.get_cache_size())
							bank.read_customer(acc_no);
						int i = bank.find_customer_position(acc_no);
						if(i == -1)
							break;
						int deposit_value;
						System.out.println("How much do you want to deposit");
						deposit_value = sc.nextInt();
						boolean is_successful = bank.deposit(i,deposit_value);
						if(is_successful)
							bank.record_deposit(acc_no,deposit_value);
					}
					catch(ClassNotFoundException exc) {
						System.out.println("Class cannot be found");
						break;
					}
				}
				break;
				case 7:
				{
					int acc_no = bank.get_acc_no();
					int j;
					Customer customer;
					try {
						for(j = 0;j < bank.get_cache_size();j++) {
							customer = bank.get_customer_from_list(j);
							if(customer == null)
								throw new ClassNotFoundException();
							if(customer.acc_no == acc_no) {
								bank.push_back(acc_no);
								break;
							}
						}
						if(j == bank.get_cache_size())
							bank.read_customer(acc_no);
						int i = bank.find_customer_position(acc_no);
						if(i == -1) 
							break;
						customer = bank.get_customer_from_list(i);
						if(customer.wrong_attempts >= 3) {
							System.out.println("Locked\nDeposit to unlock");
							break;
						}
						int withdraw_value;
						System.out.println("How much do you want to withdraw");
						withdraw_value = sc.nextInt();
						boolean is_successful = bank.withdraw_money(i,withdraw_value);
						if(is_successful) {
							bank.record_withdrawal(acc_no,withdraw_value);
						}
					}
					catch(ClassNotFoundException exc) {
						System.out.println("Class cannot be found");
						break;
					}
				}
				break;
				case 8:
					bank.transfer_money();
				break;
				case 9:
					bank.print_account_statement();
				break;
				case 10:
					bank.print_account_summary_in_range();
				break;
				case 11:
					bank.schedule_transfer();
				break;
				case 12:
					if(bank.is_current_operator_admin())
						bank.add_operator();
					else	
						System.out.println("You do not have admin privileges");
				break;
				case 13:
					bank.write_files();
					System.exit(0);
				case 14:
					bank.print_pending_transactions();
				break;
				case 15:
					int size = bank.get_cache_size();
					System.out.println("The size " + size );
				break;
				case 16:
					bank.clear_cache();;
				break;
				case 17:
					bank.display_current_cache();
				break;
				case 18:
					bank.add_standing_transactions();
				break;
				case 19:
					bank.fixed_deposit();
				break;
				case 20:
					int acc_no = bank.get_acc_no();
					bank.forgot_password(acc_no);
				break;
				default:
					System.out.println("Enter the correct option");
			}	
		}
	}*/
}