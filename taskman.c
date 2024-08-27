#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <mariadb/mysql.h>
int get_date(){
	time_t current_time = time(NULL);
	char date_string[20];
	strftime(date_string, 20, "%d-%m-%Y", localtime(&current_time));
	int ans = 0;
	ans = (date_string[0] - '0');
	ans = ans*10 + (date_string[1] - '0');
	return ans;
}
int get_date_full(){
	time_t current_time = time(NULL);
	char date_string[20];
	strftime(date_string, 20, "%d%m%Y", localtime(&current_time));
	int ans=0;
	ans = (date_string[0] - '0');
	ans = ans*10 + (date_string[1]- '0');
	ans = ans*10 + (date_string[2]- '0');
	ans = ans*10 + (date_string[3]- '0');
	ans = ans*10 + (date_string[4]- '0');
	ans = ans*10 + (date_string[5]- '0');
	ans = ans*10 + (date_string[6]- '0');
	ans = ans*10 + (date_string[7]- '0');
	return ans;
}
bool login(MYSQL* conn){
	char user_input[20];
	char pass_input[40];
	printf("Enter userID: ");
	scanf("%s", &user_input);
	printf("Enter password: ");
	scanf("%s", &pass_input);
	MYSQL_RES* res;
	MYSQL_ROW row;
	if(mysql_query(conn, "select * from credentials")){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);

	printf("\nChecking Credentials. Please wait...\n\n");

	while((row = mysql_fetch_row(res)) != NULL){
		int cmp_user = strcmp(user_input, row[0]);
		int cmp_pass = strcmp(pass_input, row[1]);
		if(!cmp_user && !cmp_pass){
			mysql_free_result(res);
			return true;
		}
	}
	mysql_free_result(res);
	mysql_close(conn);
	return false;
}
bool list_people(MYSQL* conn){
	system("clear");
	printf("\t\t\tThis is listing people service...\n\n");
	MYSQL_ROW row;
	MYSQL_RES* res;
	if(mysql_query(conn, "select * from people order by person_id")){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);
	while((row = mysql_fetch_row(res)) != NULL){
		printf("ID: %s\nName: %s\nType: %s\nBehaviour: %s\nPhone: %s\n\n", row[0], row[1],row[2],row[3], row[4]);
		sleep(0.75);
	}
	printf("Hit 0 to go back to Main Menu: ");
	mysql_free_result(res);
	int back; scanf("%d", &back);
	if(back == 0)
		return true;
}
bool new_person(MYSQL* conn){
	system("clear");
	MYSQL_RES* res;
	MYSQL_ROW row;
	char phone[15];
	printf("\t\t\tHere you can tell about new person you met\n\n");
//id of person
	printf("ID of person: ");
	int id; scanf("%d", &id);
//name of person
	printf("\nName of person: ");
	char name[100];
	scanf("%*c%[^\n]%*c", &name);
//	fgets(name, 100, stdin);
//	printf("you entered: %s\n\n", name);

//type of person
	printf("\nRelation with person: ");
	char relation[100];
	scanf("%s", &relation);

//behavior 
	printf("\nBehaviour of person: ");
	char behaviour[1000];
	scanf("%*c%[^\n]%*c", &behaviour);
	printf("\n");
//phone number\
	printf("\n");
	printf("Phone number of person: ");
	scanf("%s", &phone);

//	printf("\nEntered :\n%d\n%s\n%s\n%s\n%s\n\n", id, name, relation, behaviour, phone);

//enter query to pass taken info into database

	char sql[400];
	sprintf(sql, "insert into people values ('%d', '%s', '%s', '%s', '%s');", id, name, relation, behaviour, phone);
	if(mysql_query(conn, sql)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);
	printf("Person added successfully !\n\n");

	printf("Hit 0 to go back to Main Menu: ");
	mysql_free_result(res);
	int back; scanf("%d", &back);
	if(back == 0)
		return true;
}
bool list_all_task(MYSQL* conn){
	//list all tasks
	system("clear");
	printf("\t\t\tListing all Tasks...\n\n");
		MYSQL_RES* res;
		MYSQL_ROW row;
		if(mysql_query(conn, "select * from tasks;")){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
		res = mysql_use_result(conn);
		while((row = mysql_fetch_row(res)) != NULL){
			printf("Task id: %s\nTitle: %s\nDescription: %s\nDate:%s\nStart time: %s\nEnd Time: %s\nStatus: %s\n\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
		}

		printf("Hit 0 to go back to task menu: ");
		mysql_free_result(res);
		int back; scanf("%d", &back);
		if(back == 0)
			return true;
}
int stoi(char* count){
	int i = 0;
	int ans=0;
	while(count[i] != '\0'){
		ans = ans * 10 + (count[i] - '0');
	}
	return ans;
}
int count_task(MYSQL* conn){
	MYSQL_RES* res;
	MYSQL_ROW row;
//i will count number of tasks in tasklist table
	int date = get_date_full();
	char sql[100];
	sprintf(sql, "select * from tasks where task_date='%d';", date);
	if(mysql_query(conn, sql)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	int ans=0;
	res = mysql_use_result(conn);
	printf("in counting task\n");
	//printf("%s\n",res);

	while((row = mysql_fetch_row(res)) != NULL){
		ans++;
	}
	mysql_free_result(res);
	return ans;
}
bool new_task(MYSQL* conn){
	MYSQL_RES* res;
//	int task_id;
	char task_name[100];
	char task_desc[400];
	int start_time;
	int end_time;
	char task_status[] = "Active";
	system("clear");
	printf("\t\t\tHere you can schedule a new task for the day.\n\n");
	printf("Task title: ");
	scanf("%*c%[^\n]%*c", &task_name);
	printf("\nTask description: ");
	scanf("%[^\n]%*c", &task_desc);
//	printf("\n");
	printf("\nStart Time: ");
	scanf("%d", &start_time);
	printf("\nEnd time: ");
	scanf("%d", &end_time);
//	printf("\nEntered: %s\n%s\n%d\n%d\n", task_name, task_desc, start_time, end_time);

//entering data into task_test(modified) table

//task_id implementation
	int task_initzn = get_date();
	int task_count = count_task(conn) + 1;
	task_initzn = task_initzn * 10 + task_count;
	int date = get_date_full();
	char sql[400];
	sprintf(sql, "insert into tasks values ('%d', '%s', '%s', '%d', '%d', '%d', '%s');", task_initzn, task_name, task_desc, date,  start_time, end_time, task_status);
	if(mysql_query(conn, sql)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);
	printf("Task added successfully !\n\n");

	mysql_free_result(res);
	printf("Hit 0 to go back to task menu: ");
	int back;	scanf("%d", &back);
	if(back ==0 )
		return true;

}
bool list_active_tasks(MYSQL* conn){
	system("clear");
	printf("\t\t\tListing all active tasks...\n\n");
	MYSQL_ROW row;
	MYSQL_RES* res;
	if(mysql_query(conn, "select * from tasks where task_status='Active'")){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);
	while((row = mysql_fetch_row(res)) != NULL){
		printf("Task id: %s\nTitle: %s\nDescription: %s\nDate: %s\nStart time: %s\nEnd Time: %s\nStatus: %s\n\n", row[0], row[1], row[2], row[3], row[4], row[5],row[6]);
	}
	mysql_free_result(res);
	printf("Hit 0 to go back to task menu: ");
	int back;
	scanf("%d",&back);
	if(back == 0)
		return true;
}
char status_options[4][20] = {"Not Completed", "Completed", "Pending", "Active"};
bool update_status(MYSQL* conn){
	MYSQL_ROW row;
	MYSQL_RES* res;
	system("clear");
	printf("\t\t\tHere you can update status of a task\n\n");
	list_all_task(conn);
	printf("Task id for which you want to update the status: ");
	int id;
	scanf("%d",&id);
	printf("Status you want to change to:\nNot Completed[0]\nCompleted[1]\nPending[2]\nActive[3]\nYour input: ");
	int change_status;
	scanf("%d", &change_status);

	char sql[100];
	sprintf(sql,"update tasks set task_status='%s' where task_id='%d';",status_options[change_status], id);

	if(mysql_query(conn,sql)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);

	char sql_verify[100];
	sprintf(sql_verify, "select * from tasks where task_id='%d';", id);
	mysql_free_result(res);
	if(mysql_query(conn, sql_verify)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);

	while((row = mysql_fetch_row(res)) != NULL){
		printf("Task id: %s\nTitle: %s\nDescription: %s\nDate: %s\nStart time: %s\nEnd Time: %s\nStatus: %s\n\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
	}

	mysql_free_result(res);
	printf("Hit 0 to go back to task menu: ");
	int back;
	scanf("%d",&back);
	if(back == 0)
		return true;

}
bool task_redirector(int task_token, MYSQL* conn){
	if(task_token == 20100){
		list_all_task(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return true;
	}else if(task_token == 20102){
		new_task(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return true;
	}else if(task_token == 20101){
		list_active_tasks(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return true;
	}else if(task_token == 20103){
		update_status(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return true;
	}
}
int task_menu(MYSQL* conn){
	system("clear");
//this is task management service
	printf("\t\t\tThis is the task management service\n\n");
	printf("20100: List all tasks.\n");
	printf("20101: List all active tasks.\n");
	printf("20102: Enter new task.\n");
	printf("20103: Update status of a task.\n");
	printf("-1 :  To exit the task manager.\n");
	printf("Enter the token of task service you want to access: ");
	int task_token;
	scanf("%d", &task_token);
	task_redirector(task_token, conn);

}
void taskController(MYSQL* conn){
	//this will redirect to task menu and will allow to exit the task menu
	int exit_service;
	while(true){
		exit_service = task_menu(conn);
		if(exit_service == -1)
			return;
	}
}
bool search_entry(int search_date, int entry_point, MYSQL* conn){
/*i should keep track of entries in database for efficient search so
what i can do is i will simply create a database entry whenever i will write diary through my program
*/
	//entry_point will tell search func from where the call is made (direct = 0, indirect[i.e. write func] = 1);
	if(entry_point == 0){ // my search date is null of '-1'
		MYSQL_ROW row;
		MYSQL_RES* res;
		char user_date[10];
		printf("\nEnter date[ddmmyyyy] you wish to search entry for: ");
		scanf("%s", &user_date);
		//simply run query and find using date provided by the user
		char sql[100];
		sprintf(sql, "select * from diary_entries;");
		if(mysql_query(conn, sql)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
		res = mysql_use_result(conn);
		while((row = mysql_fetch_row(res)) != NULL){
			//int fetched_date = stoi(row[0]);
			if((strcmp(user_date, row[0]))== 0){
				mysql_free_result(res);
				return true;
			}
		}
		mysql_free_result(res);
		return false;
	}else if(entry_point == 1){ //search_date var will be used to search
		MYSQL_ROW row;
		MYSQL_RES* res;
		char sql[100];
		sprintf(sql, "select * from diary_entries;");
		if(mysql_query(conn, sql)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
		res = mysql_use_result(conn);
		while((row = mysql_fetch_row(res)) != NULL){
			int fetched_date = stoi(row[0]);
			if(fetched_date == search_date){
				mysql_free_result(res);
				return true;
			}
		}
		mysql_free_result(res);
		return false;
	}
}
void searchController(MYSQL* conn){
	bool exist = search_entry(-1,0,conn);
	if(exist){
		printf("Diary exist with the given entry.\n");
	}else{
		printf("Diary does not exist with the given entry.\n");
	}
	printf("Hit 0 to go back to diary menu: ");
	int back;
	scanf("%d", &back);
	if(back == 0){
		return;
	}
}
bool write_diary(MYSQL* conn){
//i will pull up nano writer so tha you can write the diary
	bool exist = search_entry(-1,1,conn);
	if(exist){
		//this means diary already exist so ask if he wish to edit
		printf("Diary already exist for today's entry.\n");
		printf("Do you wish to edit it? [0 - no , 1 - yes]: ");
		int edit_service;
		scanf("%d", &edit_service);
		if(edit_service == 0){
			return true;
		}else if (edit_service == 1){
			//call edit_service with the entry_point specific to write_diary service
		}
	}else{
		//create file and open in write mode [simple enough i think]
		
	}
}
bool diary_redirector(MYSQL* conn, int diary_token){
	if(diary_token == 30100){
		write_diary(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return true;
	}else if(diary_token == 30102){
		searchController(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return true;
	}else if(diary_token == 30103){
//		list_diary(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return true;
	}
}
int diary_menu(MYSQL* conn){
	system("clear");
	printf("\t\t\tThis is the diary service.\n\n");
	printf("30100: Write about your day.\n");
	printf("30101: Edit diary.\n");
	printf("30102: Search for a particular day.\n");
	printf("30103: List all enteries.\n");
	printf(" -1:  To exit the diary service.\n");
	printf("Enter the diary service you want to access: ");
	int diary_token;
	scanf("%d", &diary_token);
	if(diary_token == -1)
		return -1;
	diary_redirector(conn, diary_token);
}
bool diaryController(MYSQL* conn){
//this will take me to the diary menu and also will help me in exiting diary menu back to main menu
	int response;
	while(true){
		response = diary_menu(conn);
		if(response == -1){
			return true;
		}
	}
}
void redirector(int service_token, MYSQL* conn){
	if(service_token == -1){
		printf("Exiting the program...\n");
		sleep(5);
		system("clear");
		exit(0);
	}else if(service_token == 11001){
		//access data of all people ->  will go to people table
		list_people(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return;
	}else if(service_token == 11010){
		//creating a new person entry
		new_person(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return;
	}else if(service_token == 11011){
		//task management service
		taskController(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return;
	}else if(service_token == 11100){
		diaryController(conn);
		printf("Exiting the service...\n");
		sleep(3);
		return;
	}

}
int mainmenu(MYSQL* conn){
	system("clear");
	printf("\t\t\tWelcome back Mr RedLeader.\n\n");
	printf("\t\tThis is the main menu.\n\n");
	printf("11001: Access data of all people.\n");
	printf("11010: Enter new person.\n");
	printf("11011: Task Manager.\n");
	printf("11100: Write about your day.\n");
	printf("11111: Exit the program.\n");
	int service_access_token;
	printf("Enter the token of the service you want to access: ");
	scanf("%d", &service_access_token);
	redirector(service_access_token, conn);

	//return 1;
}
void mainMenuController(MYSQL* conn){
	bool check = login(conn);
	if(!check){
		system("clear");
		sleep(1);
		printf("You are not authorised to access this program.\n");
		sleep(3);
		exit(0);
	}
	int flag;
	while(true){
		flag = mainmenu(conn);
		if(flag == -1){
			return;
		}
	}
}
int main(){
	system("clear");
	printf("Hey, I am redleader !\n");
	printf("Connecting to mariadb server...\n");
	MYSQL* conn;
	MYSQL_RES *res;
      MYSQL_ROW row;

      char *server = "localhost";
      char *user = "root";
      //set the password for mysql server here
      char *password = "kkeppjssks"; /* set me first */
      char *database = "taskman";

      conn = mysql_init(NULL);

      /* Connect to database */
      if (!mysql_real_connect(conn, server,
            user, password, database, 0, NULL, 0)) {
          fprintf(stderr, "%s\n", mysql_error(conn));
          exit(1);
      }

      /* send SQL query */
/*      if (mysql_query(conn, "select * from people")) {
          fprintf(stderr, "%s\n", mysql_error(conn));
          exit(1);
      }

      res = mysql_use_result(conn);

      /* output table name */
/*      printf("Data of people met:\n");
      while ((row = mysql_fetch_row(res)) != NULL)
          printf("%s %s %s\n", row[0], row[1], row[2]);

      /* close connection */
	mainMenuController(conn);
//      mysql_free_result(res);
//     mysql_close(conn);



	return 0;
}
