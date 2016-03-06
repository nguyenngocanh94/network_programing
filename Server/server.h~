//
//  server.h
//  MySQL-TuDien
//
//  Created by Linh Dang on 12/8/15.
//  Copyright (c) 2015 Linh Dang. All rights reserved.
//

#ifndef MySQL_TuDien_server_h
#define MySQL_TuDien_server_h
#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define SERV_PORT 5500
#define OPENMAX 100
#define LENG 1024
//declear for socket
int					i, maxi, listenfd, connfd, sockfd;
int					nready;
ssize_t				n;
char				comingbuf[LENG];
char 				sendbuf[LENG];
socklen_t			clilen;
struct pollfd		client[OPENMAX];
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
char 				*notfound = "WORD NOT EXIST";
/*****end error*******/
char enter[2] = "\n";
char *pre[25] = {"1__","2__","3__","4__","5__","6__","7__","8__","9__","10__","11__","12__","13__","14__","15__","16__","17__","18__","19__","20__","21__","22__","23__","24__","25__"};
int querySQL(char *str);

#endif
