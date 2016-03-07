#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#define SERV_PORT 5500
#define OPEN_MAX 100
#define LENG 1024
//declear for socket
	int					i, maxi, listenfd, connfd, sockfd;
	int					nready;
	ssize_t				n;
	char				comingbuf[LENG];
	char 				sendbuf[LENG];
	socklen_t			clilen;
	struct pollfd		client[OPEN_MAX];
	struct sockaddr_in	cliaddr, servaddr;
	/*************************************************/
	//declear for SQL api
	MYSQL               *conn;
	MYSQL_RES 			*result;
	long 				num_rows;
	char 				query[LENG];
	//declare error and success
	char *error = "0";
	char *succ = "1";
/*****end error*******/	
	char enter[2] = "\n";
	char *pre[25] = {"1__","2__","3__","4__","5__","6__","7__","8__","9__","10__","11__","12__","13__","14__","15__","16__","17__","18__","19__","20__","21__","22__","23__","24__","25__"};
	/**************************************************/
	void finish_with_error(MYSQL *conn){
    fprintf(stderr, "%s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
}
void err_sys(const char* x){ 
    perror(x); 
    exit(1); 
}
	void connectSQL(){
	conn = mysql_init(NULL);
	mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES utf8");
	if (!mysql_real_connect(conn, "localhost", "root", "thicongtu2", "english", 0, 0, 0))
	{
		finish_with_error(conn);
	}else{
		printf("[Server]: Succsess connect to Database!\n");
	}
}
void endSQL(){
	mysql_free_result(result);
	mysql_close(conn);
}
int querySQL(char *str){
	if (mysql_query(conn, str))
	{
		finish_with_error(conn);
	}
	result = mysql_store_result(conn);
	if (result)
	{
		num_rows = mysql_num_rows(result);
		return 1;
	}else{
		if (mysql_field_count(conn) == 0)
		{
			num_rows = mysql_affected_rows(conn);
			return 1;	
		}else{
			finish_with_error(conn);
			return 0;
		}
	}
}

int selectSQL(char* dbname, char* word){
	strcpy(query, "SELECT * FROM `");
	strcat(query, dbname);
	strcat(query, "` WHERE `word` LIKE \'");
	strcat(query, word);
	strcat(query, "\'");
	querySQL(query);
	return (int)num_rows;
}
int inmodify(char *str1, char *str2){
	strcpy(query,"SELECT MAX(`id`),`mean_reason` FROM `modify`");
	strcat(query,"WHERE `user` LIKE \'");
	strcat(query,str1);
	strcat(query,"\' AND `word` LIKE \'");
	strcat(query,str2);
	strcat(query, "\'");
	querySQL(query);
	mysql_free_result(result);
	return (int)num_rows;
}
/*void resultEV(){
	MYSQL_ROW row;
	int i = 0;
	char a;
	if (num_rows == 0)
	{
		write(sockfd,error,strlen(error));
	}
	
	while(row = mysql_fetch_row(result)){
		strcat(sendbuf,pre[i]);
		strcat(sendbuf,row[1]);
        strcat(sendbuf,row[2]);
        strcat(sendbuf,enter);
        i++;
	}
	write(sockfd,sendbuf,strlen(sendbuf));	
}*/
void resultEV(){
	MYSQL_ROW row;
	int i = 0;
	char a;
	int leng2;
	FILE *f1 = fopen("a.txt","a");
	if (num_rows == 0)
	{
		write(sockfd,error,strlen(error));
	}
	
	while(row = mysql_fetch_row(result)){
		fputs(pre[i],f1);
		fputs(row[1],f1);
        fputs(row[2],f1);
        fputs(enter,f1);
        i++;
	}
	fclose(f1);
	FILE *f2 = fopen("a.txt","r");
	while((leng2 = fread(sendbuf, sizeof(char), LENG, f2))>0)
            {	
                if(send(sockfd,sendbuf, leng2, 0) < 0)
                {
    
                    printf("[Server]: Error when send file \n");
                    exit (1);
                }
                bzero(sendbuf, LENG);
            }
    fclose(f2);
    remove("a.txt");        	
}
void resultEC(){
	MYSQL_ROW row;
	int i = 0;
	char a;
	if (num_rows == 0)
	{
		write(sockfd,error,strlen(error));
	}
	while(row = mysql_fetch_row(result)){
		strcat(sendbuf,pre[i]);
        strcat(sendbuf,row[1]);
        strcat(sendbuf,enter);
        i++;
		/*send resuld to client*/
	}
	write(sockfd,sendbuf,strlen(sendbuf));	
}
void search(){
	strcpy(&comingbuf[0],&comingbuf[1]);
	char *searchW = strtok(comingbuf,"%");
	selectSQL("entries",searchW);
	resultEV();
	bzero(sendbuf,LENG);
	bzero(comingbuf,LENG);
}
void searchUS(){
	strcpy(&comingbuf[0],&comingbuf[1]);
	char *US_R = strtok(comingbuf,"%");
	char *S_W = strtok(NULL,"%");
	if(!inmodify(US_R,S_W)){
		selectSQL("entries",S_W);
		resultEV();
		printf("hello\n");
	}else{
		inmodify(US_R,S_W);
		resultEC();
	}
	bzero(sendbuf,LENG);
	bzero(comingbuf,LENG);
}
int loginSQL(char *str1, char *str2){
	strcpy(query,"SELECT \'role\' FROM `account`");
	strcat(query,"WHERE `user` = \'");
	strcat(query,str1);
	strcat(query,"\'");
	strcat(query,"AND `pass` = \'");
	strcat(query,str2);
	strcat(query,"\'");
	querySQL(query);
	return (int)num_rows;
}
int checklogin(char *acc, char *pass){
	MYSQL_ROW row;
	int session;
	loginSQL(acc,pass);
	if (num_rows == 0)
	{
		session = 0;
	}
	else{
		session = 1;
	}
	return session;
}
void login(){
	int a;
	strcpy(&comingbuf[0], &comingbuf[1]);
	char *User = strtok(comingbuf,"%");
	char *Pass = strtok(NULL,"%");
	a = checklogin(User,Pass);
	if (a == 0){
		write(sockfd,error,1);
	}
	if(a == 1){
		write(sockfd,succ,1);
	}
	bzero(comingbuf,LENG);
}
void regsiterSQL(char *str1,char *str2){
	strcpy(query,"INSERT INTO `account` VALUES (\'");
	strcat(query,succ);
	strcat(query, "\',\'");
	strcat(query,str1);
	strcat(query, "\',\'");
	strcat(query,str2);
	strcat(query, " \')");
	querySQL(query);
}
int isUser(char *str){
	strcpy(query,"SELECT * FROM `account`");
	strcat(query,"WHERE `user` = \'");
	strcat(query,str);
	strcat(query,"\'");
	querySQL(query);
	printf("HOA\n");
	return (int)num_rows;
}
void regsiter(){
	strcpy(&comingbuf[0], &comingbuf[1]);
	char *User = strtok(comingbuf,"%");
	char *Pass = strtok(NULL,"%");
	if (isUser(User) == 1)
	{
		write(sockfd,error,1);
	}else{
	regsiterSQL(User,Pass);
	write(sockfd,succ,1);
	}
	bzero(comingbuf,LENG);
}
/* user - word - mean */
int addSQL(char *str1, char *str2, char *str3){
	int resultAddSQL;
	strcpy(query,"INSERT INTO `modify` (`user`,`word`,`mean_reason`,`kind`) VALUES (\'");
	strcat(query,str1);
	strcat(query, "\',\'");
	strcat(query,str2);
	strcat(query, "\',\'");
	strcat(query,str3);
	strcat(query, "\',\'");
	strcat(query,"AC");
	strcat(query, " \')");
	resultAddSQL = querySQL(query);
	return resultAddSQL;
}
void adds(){
	strcpy(&comingbuf[0], &comingbuf[1]);
	char *New_U = strtok(comingbuf,"%");
	char *New_W = strtok(NULL,"%");
	char *New_M = strtok(NULL,"%");
	printf("%s\n",New_W );
	if(addSQL(New_U,New_W,New_M) == 0){
		write(sockfd,error,1);
	}else{
		write(sockfd,succ,1);
	}
	bzero(comingbuf,LENG);
}
/*******************************************/
int removeSQL(char *str1, char *str2, char *str3){
	int resultRmSQl;
	strcpy(query,"INSERT INTO `modify` (`user`,`word`,`mean_reason`,`kind`) VALUES (\'");
	strcat(query,str1);
	strcat(query, "\',\'");
	strcat(query,str2);
	strcat(query, "\',\'");
	strcat(query,str3);
	strcat(query, "\',\'");
	strcat(query,"R");
	strcat(query, " \')");
	printf("%s\n",query );
	resultRmSQl = querySQL(query);
	printf("DONE\n");
	return resultRmSQl;
}
void removes(){
	strcpy(&comingbuf[0], &comingbuf[1]);
	char *R_User = strtok(comingbuf,"%");
	char *R_Word = strtok(NULL,"%");
	char *R_Reas = strtok(NULL,"%");
	if(removeSQL(R_User,R_Word,R_Reas) == 0){
		write(sockfd,error,1);
	}else{
		write(sockfd,succ,1);
	}
	bzero(comingbuf,LENG);
}
void process(){
	switch(comingbuf[0])
	{
		case '0' : search();
			break;
		case '1' : searchUS();
			break;
		case '2' : login();
			break;			
		case '3' : regsiter();
			break;
		case '4' : adds();
			break;		
		case '5' : removes();
			break;
		default : break;					
	}	
}

/*****************************************************/
int main(int argc, char **argv)
{	
	//connect SQL
	connectSQL();
	//create socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
	{
		perror("[Server]: can't create socket!\n");
		exit(1);
	}
	printf("[Server]: Create socket Sucessfully!\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);
	//bind
	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0){
		perror("[Server]: can't bind socket!");
		exit(1);
	}
	printf("[Server]: Bind Sucessfully with 127.0.0.1 Port 5500...\n");
	//listen
	listen(listenfd, 10);
	printf("[Server]: Listening ....\n");
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1;		
	maxi = 0;
	printf("[Server]: Poll processing....\n");					
	for ( ; ; ) {
		nready = poll(client, maxi+1, 60000);

		if (client[0].revents & POLLRDNORM) {	/* new client connection */
			printf("[Server]: Has a client come !!!\n");
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
			//save decriptor
			for (i = 1; i < OPEN_MAX; i++)
				if (client[i].fd < 0) {
					client[i].fd = connfd;	
					break;
				}
			if (i == OPEN_MAX)
				err_sys("[Server]: Too many clients coming\n");

			client[i].events = POLLRDNORM;
			if (i > maxi)
				maxi = i;			
			// no descriptor can read
			if (--nready <= 0)
				continue;			
		}
		//check incoming client to recv data
		for (i = 1; i <= maxi; i++) {
			if ( (sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				if ( (n = read(sockfd, comingbuf, LENG)) < 0) {

					//something wrong with client
					if (errno == ECONNRESET) { 
						printf("[Server]: Client[%d] aborted connection\n", i);
						close(sockfd);
						client[i].fd = -1;
					} else
						err_sys("[Server]: Read error\n");
				} else if (n == 0) {
					printf("[Server]: Client[%d] closed connection\n", i);
					close(sockfd);
					client[i].fd = -1;
				} else
				/* process signal from client*/
				process();
				if (--nready <= 0)
					break;				/* all done */
			}
		}
	}
	return 0;
}
/********************************************/
