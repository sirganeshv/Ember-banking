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
	private native boolean is_customer_under_current_operator(int position);
	private native boolean is_current_operator_admin();
	private native void initialize();
	private native void add_account();
	private native void update_account();
	private native void delete_account();
	private native void display_customer(int position);
	private native void display_customer_list();
	private native boolean deposit(int position,int deposit_value);
	private native void record_deposit(int acc_no,int deposit_value);
	private native boolean withdraw_money(int position,int withdraw_value);
	private native void record_withdrawal(int acc_no,int withdraw_value);
	private native void transfer_money();
	private native void print_account_statement();
	private native void print_account_summary_in_range();
	private native void schedule_transfer();
	private native void write_files();
	private native void print_pending_transactions();
	private native void push_back(int acc_no);
	private native void display_current_cache();
	private native int get_cache_size();
	private native void clear_cache();
	private native void add_standing_transactions();
	private native void fixed_deposit();
	private native void forgot_password(int acc_no);
	
	private native boolean login(int id,String password);
	private native boolean add_operator(String name,int id,String password,String is_admin);
	private native int add_customer(String name,int age,String phone,String address,String passphrase,String security_qn,String security_ans);
	
	private static final String LOGIN = "/login";
	private static final String ADD_OPERATOR = "/add_operator";
	private static final String ADD_CUSTOMER = "/add_customer";
	
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
				//getServletContext().log("Id of the operator is "+req.getParameter("id"));
				int id = Integer.parseInt(req.getParameter("ID"));
				String password = req.getParameter("password");
				boolean status = bank.login(id,password);
				pw.println(status);
			}
			break;
			case ADD_OPERATOR: {
				int id = Integer.parseInt(req.getParameter("id"));
				String password = req.getParameter("pass");
				getServletContext().log("The method is getting executed "+id + "   "+password);
				boolean status = false;
				status = bank.add_operator(req.getParameter("name"),id,password,req.getParameter("is_admin"));
				getServletContext().log("Status is "+status);
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
			default:
			break;
		}
		pw.close();
	}
		
		
	static {
		System.load("C:\\xampp\\tomcat\\webapps\\bank\\banking.dll");
	}
	
	public static void main(String[] args) {
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
		}*/
	}
}