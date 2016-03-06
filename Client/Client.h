//
//  Client.h
//
//  Created by Rush on 11/20/15.
//  Copyright (c) 2015 Rush. All rights reserved.
//

#ifndef MySQL_TuDien_Client_h
#define MySQL_TuDien_Client_h
#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
static void makegtk();
/**********************************************/
#define PORT 5500
#define STANDARD 500
#define LENG 1024
/**********************************************/
/*declare widget main window*/
GtkWidget *window;
GtkWidget *scrolled_window;
GdkPixbuf *icon;
GtkWidget *grid;
/*declare menu item*/
GtkWidget *menubar;
GtkWidget *accmenu;
GtkWidget *account;
GtkWidget *login;
GtkWidget *logout;
GtkWidget *resigter;
/****************/
GtkWidget *util;
GtkWidget *utilities;
GtkWidget *add;
GtkWidget *removes;
/****************/
GtkWidget *aboutmenu;
GtkWidget *about;
GtkWidget *aboutitem;
/*end declare menu item*/
GtkWidget *image;
GtkWidget *slogan;
GtkWidget *eng;
GtkWidget *mean;
GtkWidget *entry;
/******************/
GtkWidget *textview;
GtkWidget *button;
/* declare for pop-up login window*/
GtkWidget *window1;
GtkWidget *grid1;
GtkWidget *button1;
GtkWidget *label1;
GtkWidget *label3;
GtkWidget *label2;
GtkWidget *img1;
GtkWidget *entry1;
GtkWidget *entry2;
GtkWidget *spinner;
/*declare for register window*/
GtkWidget *window2;
GtkWidget *img2;
GtkWidget *label4;
GtkWidget *label5;
GtkWidget *label6;
GtkWidget *label7;
GtkWidget *checkbutton;
GtkWidget *entry3;
GtkWidget *entry4;
GtkWidget *entry5;
GtkWidget *button2;
GtkWidget *spinner1;
GtkWidget *grid2;
/* declare for add window */
GtkWidget *label8;
GtkWidget *label9;
GtkWidget *label10;
GtkWidget *entry6;
GtkWidget *entry7;
GtkWidget *textview1;
GtkWidget *button3;
GtkWidget *grid3;
GtkWidget *window3;
GtkWidget *status1;
/* declare romove function */
GtkWidget *label11;
GtkWidget *label12;
GtkWidget *entry8;
GtkWidget *textview2;
GtkWidget *button4;
GtkWidget *grid4;
GtkWidget *window4;
GtkWidget *status2;
GError *error = NULL;
/*end declare widget */
/* declare for socket */
ssize_t sockfd;
char buf[LENG];
struct sockaddr_in server;
/* prefix for function */
char *per = "%";
/*SESSION */
int SESSION	;
char USER[20];
/* click checkbox */
int checked;

void dialog_(GtkWidget *win,char * title, char* msg);
void logout_function();
int checkSQL(char *str);
void connectSK();
#endif
