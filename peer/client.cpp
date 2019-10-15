#include "header.h"

struct file_down
{
    string filename;
    string ip;
    string port;
    string destination;
};
void *client_function(void* f_n)
{

string filename=(((struct file_down *)f_n)->filename);
string ip=(((struct file_down *)f_n)->ip);
string port1=(((struct file_down *)f_n)->port);
int port=stoi(port1);
string destination=(((struct file_down *)f_n)->destination);

//char *command=(char *)malloc(sizeof(char)*1024);
//strcpy(command,(char *)f_n);    
struct sockaddr_in clientaddr;
int new_socket1,valread;
/*
cout<<"this is length of client"<<strlen(command)<<endl;
cout<<"hello my file "<<command<<endl;*/
//establishing socket at client end
int sock_fd1=socket(AF_INET,SOCK_STREAM,0);
//establishing connection 
clientaddr.sin_family=AF_INET;
clientaddr.sin_port=htons(port);
clientaddr.sin_addr.s_addr=INADDR_ANY;
if((connect(sock_fd1,(struct sockaddr *)&clientaddr,sizeof(clientaddr)))<0) 
 {
  perror("error in binding");
  exit(1);
 }
else
 {
 cout<<"client connected"<<endl;
 char buf1[filename.length()+1];
 cout<<"this is sfilename in client "<<buf1<<endl; 
 strcpy(buf1,filename.c_str());
 cout<<"this i file name in source "<<buf1<<endl;
 //sending source file_path to server
 send(sock_fd1,buf1,strlen(buf1),0);
 //cout<<"this is file name"<<file_name<<endl;
 /*
 char command1[1024];
 strcpy(command1,command);
 cout<<"this is command1 "<<command<<endl;
 char *token=strtok(command1," ");
 cout<<"this is token "<<token <<endl;
 if(!strcmp(token,"create"))
 {
 cout<<"this is command  "<<command <<endl;
 cout<<"this is size "<<sizeof(command)<<endl;
 send(sock_fd,command,1024,0);
 //free(command)
 }
 else
 { */
 char des[destination.length()+1];
 strcpy(des,destination.c_str());
 cout<<"this is destination file "<<des<<endl;
 int fd1=open(des,O_WRONLY |O_TRUNC|O_CREAT,0777);
 if(fd1<0)
 cout<<"error"<<endl;
 int n;
 char buffer[200];
 while((n=read(sock_fd1,buffer,sizeof(buffer)))>0)
 {
     //cout<<"this is buffer in client "<<buffer<<endl;
     write(fd1,buffer,sizeof(buffer));
 }
close(fd1);
 
 
 }
 }
