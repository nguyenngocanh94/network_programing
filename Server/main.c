#include "server.h"
/**************************************************/
void finish_with_error(MYSQL *conn){
    fprintf(stderr, "%s\n", mysql_error(conn));
    mysql_close(conn);
    //exit(1);
}
void err_sys(const char* x){
    perror(x);
    exit(1);
}
void connectSQL(){
    conn = mysql_init(NULL);
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES utf8");
    if (!mysql_real_connect(conn, "127.0.0.1", "root", "", "english", 0, 0, 0))
    {
        finish_with_error(conn);
    }else{
        printf("[Server]: Succsess connect to Database!\n");
    }
    querySQL("UPDATE account SET online = 0");
}
void endSQL(){
    mysql_free_result(result);
    mysql_close(conn);
}
int querySQL(char *str){

    //printf("\nquery - %s\n ",str);
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
    strcpy(query,"SELECT `id`,`mean_reason` FROM `modify` ");
    strcat(query,"WHERE `user` LIKE \'");
    strcat(query,str1);
    strcat(query,"\' AND `word` LIKE \'");
    strcat(query,str2);
    strcat(query, "\'  ORDER BY id DESC LIMIT 1");
    querySQL(query);
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
    int leng2;
    FILE *f1 = fopen("a.txt","a");
    if (num_rows == 0)
    {
        write(sockfd,notfound,strlen(notfound));
    }
    
    while((row = mysql_fetch_row(result))){
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
    if (num_rows == 0)
    {
        write(sockfd,notfound,strlen(notfound));
    }
    while((row = mysql_fetch_row(result))){
        strcat(sendbuf,pre[i]);
        strcat(sendbuf,row[1]);
        strcat(sendbuf,enter);
        i++;
    }
    write(sockfd,sendbuf,strlen(sendbuf));
}
void search(){
    memmove(&comingbuf[0],&comingbuf[1], 1);
    char *searchW = strtok(comingbuf,"%");
    selectSQL("entries",searchW);
    resultEV();
    bzero(sendbuf,LENG);
    bzero(comingbuf,LENG);
}
void searchUS(){
    memmove(&comingbuf[0],&comingbuf[1], 1);
    char *US_R = strtok(comingbuf,"%");
    char *S_W = strtok(NULL,"%");
    if(!inmodify(US_R,S_W)){
        mysql_free_result(result);
        selectSQL("entries",S_W);
        resultEV();
        printf("hello\n");
    }else{
        mysql_free_result(result);
        inmodify(US_R,S_W);
        resultEC();
    }
    bzero(sendbuf,LENG);
    bzero(comingbuf,LENG);
}
int loginSQL(char *str1, char *str2){
    strcpy(query,"SELECT `role`,`online` FROM `account`");
    strcat(query," WHERE `user` = \'");
    strcat(query,str1);
    strcat(query,"\' ");
    strcat(query,"AND `pass` = \'");
    strcat(query,str2);
    strcat(query,"\'");
    querySQL(query);
    return (int)num_rows;
}
int set_status(char *acc, int online){
    char str[]= "1";
    if(online == 0)strcpy(str,"0");
    strcpy(query,"UPDATE `english`.`account` SET `online` = \'");
    strcat(query, str);
    strcat(query,"\' WHERE `user` = \'");
    strcat(query,acc);
    strcat(query,"\'");
    querySQL(query);
    return (int)num_rows;
}
int checklogin(char *acc, char *pass){
    loginSQL(acc,pass);
    MYSQL_ROW row;
    int logged_in = 0;
    while(row = mysql_fetch_row(result)){
        logged_in = atoi(row[1]);
        }
    if ((num_rows == 0)||(logged_in==1))
    {
        return 0;
    }
    else{
        set_status(acc,1);
        return 1;
    }
}
void login(){
    int a;
    memmove(&comingbuf[0],&comingbuf[1], 1);
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
    strcat(query, "\',\'");
    strcat(query, " \')");
    querySQL(query);
    mysql_free_result(result);
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
    memmove(&comingbuf[0], &comingbuf[1], 1);
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
    mysql_free_result(result);
    return resultAddSQL;
}
void adds(){
    memmove(&comingbuf[0],&comingbuf[1], 1);
    char *New_U = strtok(comingbuf,"%");
    char *New_W = strtok(NULL,"%");
    char *New_M = strtok(NULL,"%");
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
    resultRmSQl = querySQL(query);
    mysql_free_result(result);
    printf("DONE\n");
    return resultRmSQl;
}
void removes(){
    memmove(&comingbuf[0],&comingbuf[1], 1);
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
void logout(){
    memmove(&comingbuf[0],&comingbuf[1], 1);
    char *User = strtok(comingbuf,"%");
    set_status(User, 0);
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
        case '6' : logout();
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
    int optval = 1;
    setsockopt(listenfd , SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(optval));

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
    listen(listenfd, OPENMAX);
    printf("[Server]: Listening ....\n");
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPENMAX; i++)
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
            for (i = 1; i < OPENMAX; i++)
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            if (i == OPENMAX)
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
                {
                    //printf("%s",comingbuf);
                    process();
                }
                if (--nready <= 0)
                    break;				/* all done */
                
            }
           
        }
    }
    return 0;
}
/********************************************/
