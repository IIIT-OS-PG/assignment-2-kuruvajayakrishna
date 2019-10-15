#include "header.h"
//server_function
char * filename;
void * server_function(void *ptr)
{
int opt=1;
//cout<<"this is server"<<endl;
//creating socket at server end 
int server_socket=socket(AF_INET,SOCK_STREAM,0);
//using setsockptions so as to reuse the port
//cout<<"this is server"<<endl;
if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
 { 
  perror("setsockopt");
  exit(1); 
 } 
//cout<<"this is server"<<endl;
//bind address to socket
//cout<<"this is server"<<endl;
//int *port1=(int *)malloc(sizeof(int));
int port1=*((int *)ptr);
//cout<<"this is my port "<<port1;
struct sockaddr_in servaddr;
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(port1);
servaddr.sin_addr.s_addr=INADDR_ANY;
int addrlen=sizeof(servaddr);
if(bind(server_socket,(struct sockaddr *)&servaddr,addrlen))
 {
    cout<<"Error while binding"<<endl;
    exit(1);
 }
 else
 {
    cout<<"binding completed"<<endl;
 }
 
//listening on port
if(listen(server_socket,5)<0)
{
 perror("error in listening");
 exit(1);
}

while(1)
{
   cout<<"waiting for connection"<<endl;
int new_socket = accept(server_socket,NULL,NULL);
if(new_socket <0 )
 {
    perror("error in accept");
    exit(1);
 }
else
 {
    cout<<"connection established"<<endl;
 }
pthread_t t;
int *pclient=(int *)malloc(sizeof(int));
cout<<"this new_socket id"<<new_socket<<endl;
*pclient=new_socket;
pthread_create(&t,NULL,handle_connection,pclient); 
}
}

void * handle_connection(void * pclient)
{
int new_sockets;
char buffer[200];
cout<<"entered handle connection"<<endl;
new_sockets=*((int *)pclient);
char b[1024];
cout<<"waiting for reading"<<endl;
read(new_sockets,b,sizeof(b));
cout<<"this is source file "<<b<<endl;
int fd=open(b,O_RDONLY,0644);
int n;
while(n=read(fd,buffer,sizeof(buffer))>0)
{
    //cout<<"this is buf in server "<<buffer<<endl;
    write(new_sockets,buffer,sizeof(buffer));
}
close(fd);
pthread_exit(NULL);
}