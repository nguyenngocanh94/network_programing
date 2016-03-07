#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>   
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
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
/* declare romove funtion */
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
/* prefix for funtion */
	char *per = "%";
/*SESSION */
	int SESSION	;
	char USER[20];
/* click checkbox */
	int checked;		
			
/**************************************************************/
int checkSQL(char *str){
	int i;
	int k = 0;
	if((strlen(str) == 0)||(strlen(str) >= 50)){
		k = 1;
	}else{
		for (i = 0; i < strlen(str); i++)
			{
				if((str[i] =='<')||(str[i] =='>')||(str[i] == '=') || (str[i] =='-')||(str[i] =='\"')||(str[i] =='`') ||(str[i] =='\'') ){
				k = 1;
				break;
			}
		}
	}
	return k;
}
void connectSK(){
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	bzero(&server,sizeof(server)); 
  	server.sin_addr.s_addr = inet_addr("127.0.0.1");
  	server.sin_family = AF_INET;
  	server.sin_port = htons(PORT);
  	connect(sockfd,(struct sockaddr*)&server , sizeof(struct sockaddr));
}
/***** end of SOCKET API and check funtion*******************/
void transferADD(char *str,const char *str1,const char *str2){
	strcat(str,USER);
	strcat(str,per);
	strcat(str,str1);
	strcat(str,per);
	strcat(str,str2);
	strcat(str,".");
}
void transferLR(char *str,const char *str1,char *str2){
	strcat(str,USER);
	strcat(str,per);
	strcat(str,str1);
	strcat(str,per);
	strcat(str,str2);
}
void transferLG(char *str,const char *str1,const char *str2){
	strcat(str,str1);
	strcat(str,per);
	strcat(str,str2);
}
/*******************end of utility funtion*************/
GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}
static gboolean delete_event (GtkWidget *wigdet, GdkEvent *event, gpointer data){
	gtk_main_quit();
	return FALSE;
}
static void about_dialog(GtkMenuItem *menuitem, gpointer data){
	GdkPixbuf *icon1;
	icon1 = create_pixbuf("img/logo.png");
	static const gchar *authors[] ={"Nguyễn Ngọc Anh","Đặng Mỹ Linh",NULL};
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog),icon1);
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),"English Dictionary from Oxford");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),"Version 0.1");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),"Program build from GTK3, Socket API and MySQL API");
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog),authors);
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),"http://facebook.com/nca.nguyen.anh");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog),"Visit Him !");
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
/***********************************/
void dialog_error3(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window3),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"You Do Not Permission!");
	gtk_window_set_title(GTK_WINDOW(dialog), "Eror Permission");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(window3);
}
void dialog_error4(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window4),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"You Do Not Permission!");
	gtk_window_set_title(GTK_WINDOW(dialog), "Eror Permission");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(window4);
}
void dialog_warning(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Can't include special character");
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void dialog_succsess1(){	  
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window1),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Wellcome Back !");
	gtk_window_set_title(GTK_WINDOW(dialog), "Wellcome Back");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(window1);
}

void dialog_succsess4(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window4),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Succsessfully !");
	gtk_window_set_title(GTK_WINDOW(dialog), "Succsess");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(window4);	
}
void dialog_succsess3(){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window3),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Succsessfully !");
	gtk_window_set_title(GTK_WINDOW(dialog), "Succsess");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(window3);	
}
void dialog_succsess2(){
	GtkWidget *dialog;
  	dialog = gtk_message_dialog_new(GTK_WINDOW(window2),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Register Succsessfully !");
  	gtk_window_set_title(GTK_WINDOW(dialog), "Register Succsess");
  	gtk_dialog_run(GTK_DIALOG(dialog));
  	gtk_widget_destroy(window2);
}
/***************************end of message dialog*****************/
static void translate(GtkButton *button, gpointer data){
	connectSK();
	char translateW[50];
	if (SESSION == 0){
	char *emptystring = "";
	char trans_string[55] = "0%";
	bzero(buf, LENG);
	const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
	strcpy(translateW,text);
	if(checkSQL(translateW) == 1){
		dialog_warning();
	}else{
	strcat(trans_string,translateW);
	gtk_text_buffer_set_text(gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview)),emptystring, -1);
	/***********************************************/
  	write(sockfd, trans_string, strlen(trans_string));
  	/*******************************************/
  	FILE *f1 = fopen("b.txt","a");
  	bzero(buf, LENG); 
    int leng2 = 0;
    while((leng2 = recv(sockfd, buf, LENG, 0)) > 0)
    {
        fwrite(buf, sizeof(char), leng2, f1);
        bzero(buf, LENG);
        if (leng2 == 0 || leng2 != LENG) 
        {
                break;
        }
    }
    rewind(f1);
    gchar *array;
	gsize length;
	gchar *utf8;
	GError *error = NULL;
	GtkTextBuffer *buffer;
	GtkTextIter iter;
	gsize *bytes_read = NULL; 
	gsize *bytes_written = NULL;
	g_file_get_contents("b.txt", &array, &length, NULL);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	utf8 = g_convert(array, length, "UTF-8", "us-ascii", bytes_read, bytes_written, &error);
	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
	gtk_text_buffer_insert(buffer, &iter, utf8, -1);
  	close(sockfd);
  	fclose(f1);
  	remove("b.txt");
  	}
  }else{
  	char *emptystring = "";
	char trans_string[75] = "1%";
	bzero(buf, LENG);
	strcat(trans_string,USER);
	strcat(trans_string,per);
	const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
	strcpy(translateW,text);
	if(checkSQL(translateW) == 1){
		dialog_warning();
	}else{
	strcat(trans_string,text);
	printf("%s\n",trans_string);
	gtk_text_buffer_set_text(gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview)),emptystring, -1);
	connectSK();
  	write(sockfd, trans_string, strlen(trans_string));
  	read(sockfd, buf,LENG);
  	g_locale_to_utf8(buf, strlen(buf), NULL, NULL, NULL) ;
  	char *message;
  	message =  g_strdup_printf("%s",buf) ;
	GtkTextBuffer* text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	GtkTextIter end;
	gtk_text_buffer_get_end_iter(text_buffer, &end);
	gtk_text_buffer_insert(text_buffer, &end, message, -1);	
  	close(sockfd);
  	}
  }
}
/**************************end of translate*****************************/
static void login_funtion(GtkButton *button, gpointer data){ 
	int resultlogin;
	char logintext[40] = "2%";
	bzero(buf,LENG);
	char accl[20],passl[20];
	const char *nick = gtk_entry_get_text(GTK_ENTRY(entry1));
	const char *pass = gtk_entry_get_text(GTK_ENTRY(entry2));
	strcpy(accl,nick);
	strcpy(passl,pass);
	connectSK();
	if ((checkSQL(accl) == 0) && (checkSQL(passl) == 0)){
			transferLG(logintext,nick,pass);
			send(sockfd,logintext,strlen(logintext),0);
			recv(sockfd,buf,2,0);
			resultlogin = atoi(buf);
	}else{
		gtk_label_set_text(GTK_LABEL(label3),"ID & Pass can't include >:=...");		
	}
	if(resultlogin == 0){
		gtk_label_set_text(GTK_LABEL(label3),"Wrong ID or Password !");
	}
	if(resultlogin == 1){
		SESSION = 1;
		strcpy(USER,accl);
		dialog_succsess1();
	}
}
static void login_popup(GtkMenuItem *menuitem, gpointer data){
    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window1),350,350);
    gtk_window_set_title(GTK_WINDOW(window1), "Log In");
    gtk_window_set_modal(GTK_WINDOW(window1),TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(window1),10);
    grid1 = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid1),TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid1),TRUE);
    img1 = gtk_image_new_from_file("img/login.png");
    gtk_grid_attach(GTK_GRID(grid1),img1,0,0,10,2);
    label1 = gtk_label_new("ID: ");
    gtk_grid_attach(GTK_GRID(grid1),label1,0,2,2,1);
    entry1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid1),entry1,2,2,7,1);
    label2 = gtk_label_new("PW:");
    gtk_grid_attach(GTK_GRID(grid1),label2,0,3,2,1);
    entry2 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry2),FALSE);
    gtk_grid_attach(GTK_GRID(grid1),entry2,2,3,7,1);
    label3 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid1),label3,3,4,4,1);
    button1 = gtk_button_new_with_label("LOG IN");
    g_signal_connect(GTK_BUTTON(button1),"clicked",G_CALLBACK(login_funtion),NULL);
    gtk_grid_attach(GTK_GRID(grid1),button1,4,5,2,1);
    gtk_container_add(GTK_CONTAINER(window1),grid1);
    gtk_widget_show_all(window1);
}
/*************************end of login funtion********************/
static void isclick(GtkCheckButton *button, gpointer *data){
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data)))
       checked = 1;
   else
    checked = 0;
}
static void resigter_funtion(GtkButton *button, gpointer data){ 
	char registertext[50] = "3%";
	int resultregister;
	char Ur[20],Ps[20];
	const char *Userreg = gtk_entry_get_text(GTK_ENTRY(entry3));
	const char *Passreg = gtk_entry_get_text(GTK_ENTRY(entry4));
	strcpy(Ur,Userreg);
	strcpy(Ps,Passreg);
	if(checked == 0){
		gtk_label_set_text(GTK_LABEL(label7),"You must agree all term !");
	}else{
		if ((checkSQL(Ur) == 0) && (checkSQL(Ps) == 0))
		{	
			connectSK();
			transferLG(registertext,Userreg,Passreg);
			send(sockfd,registertext,strlen(registertext),0);
			recv(sockfd,buf,2,0);
			resultregister = atoi(buf);
		if (resultregister == 0)
			{
			gtk_label_set_text(GTK_LABEL(label7),"ID has been existed !");
			gtk_spinner_stop(GTK_SPINNER(spinner1));
			}
		if (resultregister == 1)
			{
			dialog_succsess2();
			}
		}
	}
}
static void register_spinner(GtkButton *button, gpointer data){
	spinner1 = gtk_spinner_new();
	gtk_spinner_start(GTK_SPINNER(spinner1)); 
    gtk_grid_attach(GTK_GRID(grid2),spinner1,4,8,2,1);
    gtk_widget_show(spinner1);
}
static void resigter_window(GtkMenuItem *menuitem, gpointer data){
	window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window2),360,300);
    gtk_window_set_title(GTK_WINDOW(window2), "Resigter");
    gtk_window_set_modal(GTK_WINDOW(window2),TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window2),FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window2),10);
    grid2 = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid2),TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid2),TRUE);
    gtk_container_add(GTK_CONTAINER(window2),grid2);
    img2 = gtk_image_new_from_file("img/resigter.png");
    gtk_grid_attach(GTK_GRID(grid2),img2,0,0,10,2);
    label4 = gtk_label_new("ID: ");
    gtk_grid_attach(GTK_GRID(grid2),label4,0,2,2,1);
    entry3 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid2),entry3,2,2,7,1);
    label5 = gtk_label_new("PW:");
    gtk_grid_attach(GTK_GRID(grid2),label5,0,3,2,1);
    entry4 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry4),FALSE);
    gtk_grid_attach(GTK_GRID(grid2),entry4,2,3,7,1);
    label6 = gtk_label_new("re-PW:");
    gtk_grid_attach(GTK_GRID(grid2),label6,0,4,2,1);
    entry5 = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry5),FALSE);
    gtk_grid_attach(GTK_GRID(grid2),entry5,2,4,7,1);
    checkbutton = gtk_check_button_new_with_label("I agree all term above");
    g_signal_connect (GTK_CHECK_BUTTON(checkbutton),"toggled",G_CALLBACK(isclick),(gpointer *)checkbutton);
    gtk_grid_attach(GTK_GRID(grid2),checkbutton,3,5,4,1);
    label7 = gtk_label_new(" ");
    // insert css for it
    gtk_grid_attach(GTK_GRID(grid2),label7,3,6,4,1);
    button2 = gtk_button_new_with_label("REGISTER");
    gtk_grid_attach(GTK_GRID(grid2),button2,4,7,2,1);
    g_signal_connect(GTK_BUTTON(button2),"clicked",G_CALLBACK(resigter_funtion),NULL);
    g_signal_connect_swapped(GTK_BUTTON(button2),"clicked",G_CALLBACK(register_spinner),NULL);
    gtk_widget_show_all(window2);
}
/*****************************end of register funtion *************/
void logout_funtion(GtkMenuItem *menuitem, gpointer data){
	SESSION = 0;
	bzero(USER,100);
}
/*****************************end of log out*******************/
static void add_funtion(GtkButton *button, gpointer data){
	char addstring[LENG] = "4%";
	int resultadd;
	if((SESSION == 1)||(SESSION ==2)){
	const char *new_word = gtk_entry_get_text(GTK_ENTRY(entry6));
	const char *kind_word = gtk_entry_get_text(GTK_ENTRY(entry7));
	GtkTextBuffer* text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview1));
    GtkTextIter startIter;
    GtkTextIter endIter;
    gtk_text_buffer_get_start_iter(text_buffer, &startIter);
    gtk_text_buffer_get_end_iter(text_buffer, &endIter);
    char *mean_w = NULL;
    mean_w = gtk_text_buffer_get_text(text_buffer, &startIter, &endIter, TRUE);
    	if ((new_word == NULL)||(kind_word == NULL)||(mean_w == NULL))
    	{
    		gtk_label_set_text(GTK_LABEL(status1),"Missing one of fields!");
    	}else{
	    	transferADD(addstring,new_word,kind_word);
	    	strcat(addstring,mean_w);
	    	send(sockfd,addstring,strlen(addstring),0);
	    	recv(sockfd,buf,1,0);
	    	resultadd = atoi(buf);
	    	if(resultadd == 0){
    			gtk_label_set_text(GTK_LABEL(status1),"Server Busy !");
    		}
	    	if(resultadd == 1){
	    		dialog_succsess3();
   			}
		}
	}else{
		dialog_error3();
	}
}
static void add_window(GtkMenuItem *menuitem, gpointer data){
	window3 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window3),"Add New Word");
	gtk_window_set_modal(GTK_WINDOW(window3),TRUE);
	gtk_window_set_default_size(GTK_WINDOW(window3),510,360);
	gtk_window_set_position(GTK_WINDOW(window3), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window3),10);
	grid3 = gtk_grid_new();
	gtk_grid_set_row_homogeneous(GTK_GRID(grid3),TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid3),TRUE);
	label8 = gtk_label_new("English");
	gtk_grid_attach(GTK_GRID(grid3),label8,0,0,2,1);
	entry6 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid3),entry6,3,0,8,1);
	label9 = gtk_label_new("Kind");
	gtk_grid_attach(GTK_GRID(grid3),label9,0,1,2,1);
	entry7 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid3),entry7,3,1,8,1);
	label10 = gtk_label_new("Meaning");
	gtk_grid_attach(GTK_GRID(grid3),label10,0,2,2,3);
	textview1 = gtk_text_view_new();
    gtk_grid_attach(GTK_GRID(grid3),textview1,3,2,8,3);
    status1 = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid3),status1,1,6,5,1);
    button3 = gtk_button_new_with_label("Submit");
    g_signal_connect(GTK_BUTTON(button3),"clicked",G_CALLBACK(add_funtion),NULL);
    gtk_grid_attach(GTK_GRID(grid3),button3,4,7,2,1);
    gtk_container_add(GTK_CONTAINER(window3),grid3);
    gtk_widget_show_all(window3);
}
/**************** end of add funtion*************************/
static void remove_funtion(GtkButton *button, gpointer data){
	char removestring[LENG] = "5%";
	char del[LENG] = "This word has been delete because: ";
	int resultremove;
	if (SESSION == 1)
	{
		const char *rm_word = gtk_entry_get_text(GTK_ENTRY(entry8));
		GtkTextBuffer* text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview2));
	    GtkTextIter startIter;
	    GtkTextIter endIter;
	    gtk_text_buffer_get_start_iter(text_buffer, &startIter);
	    gtk_text_buffer_get_end_iter(text_buffer, &endIter);
	    char *reason_rm = NULL;
	    reason_rm = gtk_text_buffer_get_text(text_buffer, &startIter, &endIter, TRUE);
	    strcat(del,reason_rm);
	    transferLR(removestring,rm_word,del);
	    send(sockfd,removestring,strlen(removestring),0);
	    recv(sockfd,buf,2,0);
	    resultremove = atoi(buf);
	    if(resultremove == 0){
    			gtk_label_set_text(GTK_LABEL(status2),"Server Busy !");
    		}if (resultremove == 1)
    		{
    			dialog_succsess4();
    		}
	}else{
		dialog_error4();	
	}
}
static void remove_window(GtkMenuItem *menuitem, gpointer data){
	window4 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window4),"Remove Word");
	gtk_window_set_modal(GTK_WINDOW(window4),TRUE);
	gtk_window_set_position(GTK_WINDOW(window4), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window4),10);
	grid4 = gtk_grid_new();
	gtk_grid_set_row_homogeneous(GTK_GRID(grid4),TRUE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid4),TRUE);
	label11 = gtk_label_new("English");
	gtk_grid_attach(GTK_GRID(grid4),label11,0,0,2,1);
	entry8 = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid4),entry8,2,0,8,1);
	label12 = gtk_label_new("Reason");
	gtk_grid_attach(GTK_GRID(grid4),label12,0,1,2,3);
	textview2 = gtk_text_view_new();
	gtk_grid_attach(GTK_GRID(grid4),textview2,2,1,8,3);
	status2 = gtk_label_new(" ");
	gtk_grid_attach(GTK_GRID(grid4),status2,2,6,4,1);
	button4 = gtk_button_new_with_label("Submit");
	g_signal_connect(GTK_BUTTON(button4),"clicked",G_CALLBACK(remove_funtion),NULL);
	gtk_grid_attach(GTK_GRID(grid4),button4,4,7,2,1);
	gtk_container_add(GTK_CONTAINER(window4),grid4);
	gtk_widget_show_all(window4);
}
/***********************************end of remove funtion*******************/
static void makegtk(){
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"English Dictionary");
	gtk_window_set_default_size(GTK_WINDOW(window),820,400);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	icon = create_pixbuf("img/icon.jpg");
	gtk_window_set_icon(GTK_WINDOW(window),icon);
	g_signal_connect(window,"delete-event",G_CALLBACK(delete_event),NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	/*end */
	/*create grid chuyen grid xuong cuoi file sau khi da khai bao het widget*/
	grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous(GTK_GRID(grid),TRUE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid),TRUE);
	gtk_container_add(GTK_CONTAINER(window),grid);
	/*end*/
	/*create menu */
	menubar = gtk_menu_bar_new();
	accmenu = gtk_menu_new();
	account = gtk_menu_item_new_with_label("Account");
	login = gtk_menu_item_new_with_label("Log in");
	resigter = gtk_menu_item_new_with_label("Sign in");
	logout = gtk_menu_item_new_with_label("Log out");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(account),accmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(accmenu),login);
	gtk_menu_shell_append(GTK_MENU_SHELL(accmenu),logout);
	gtk_menu_shell_append(GTK_MENU_SHELL(accmenu),resigter);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),account);
	g_signal_connect(GTK_MENU_ITEM(login),"activate",G_CALLBACK(login_popup),NULL);
	g_signal_connect(GTK_MENU_ITEM(resigter),"activate",G_CALLBACK(resigter_window),NULL);
	g_signal_connect(GTK_MENU_ITEM(logout),"activate",G_CALLBACK(logout_funtion),NULL);
	/*g_signal_connect() ..... here*/

	/***************************************/
	util = gtk_menu_new();
	utilities = gtk_menu_item_new_with_label("Utilities");
	add = gtk_menu_item_new_with_label("Add");
	removes = gtk_menu_item_new_with_label("Remove");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(utilities),util);
	gtk_menu_shell_append(GTK_MENU_SHELL(util),add);
	gtk_menu_shell_append(GTK_MENU_SHELL(util),removes);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),utilities);
	g_signal_connect(GTK_MENU_ITEM(add),"activate",G_CALLBACK(add_window),NULL);
	g_signal_connect(GTK_MENU_ITEM(removes),"activate",G_CALLBACK(remove_window),NULL);
	/*****************************************/
	aboutmenu = gtk_menu_new();
	about = gtk_menu_item_new_with_label("About");
	aboutitem = gtk_menu_item_new_with_label("About");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(about),aboutmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(aboutmenu),aboutitem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),about);
	g_signal_connect(GTK_MENU_ITEM(aboutitem),"activate",G_CALLBACK(about_dialog),NULL);
	/**********************************************/
	/*insert menubar in grid*/
	gtk_grid_attach(GTK_GRID(grid),menubar,0,0,10,1);
	/*end menubar*/
	/*add logon in window*/
	image = gtk_image_new_from_file("img/logo.png"); /*size of logo 208x42,8*/
	gtk_grid_attach(GTK_GRID(grid),image,0,3,4,3);
	/*end add logo*/
	/*attach slogan*/
	slogan = gtk_label_new(" ");
	/*change red color for slogan*/
    char *str = "English Dictionary From Oxford";
    char *markup = g_markup_printf_escaped ("<span style=\"italic\" color=\"red\" >%s</span>", str);
    gtk_label_set_markup (GTK_LABEL (slogan), markup);
    g_free(markup);
	gtk_grid_attach(GTK_GRID(grid),slogan,4,3,6,3);
	/**************************end 1st line****************************/
	eng = gtk_label_new("English");
	gtk_grid_attach(GTK_GRID(grid),eng,0,6,1,3);
	entry = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid),entry,1,6,9,3);
	/**************************end 2nd line*********************************/
	mean = gtk_label_new("Meaning :");
	gtk_grid_attach(GTK_GRID(grid),mean,0,9,1,2);
	/*********************end 3rd line*********************************/
	textview = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview),FALSE);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textview),20);
	// has a funtion interfere here to put something to it
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);              
    gtk_container_add(GTK_CONTAINER(scrolled_window), textview);       
    gtk_grid_attach(GTK_GRID(grid),scrolled_window,1,9,9,9);
    /***********************end 4th-7th line for textview**********************/
    button = gtk_button_new_with_label("Translate");
    g_signal_connect(GTK_BUTTON(button),"clicked",G_CALLBACK(translate),NULL);
    /*******funtion for it**************/
    //g_signal_connect()...
   	gtk_grid_attach(GTK_GRID(grid),button,3,19,4,2);
   	/***************************end all**************************/
    gtk_widget_show_all(window);
	g_object_unref(icon);
}
/************************end of make gtk **************************/
int main(int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	makegtk();
	gtk_main();
	return 0;
}