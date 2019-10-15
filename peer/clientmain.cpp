#include "header.h"
#include "server.cpp"
#include "client.cpp"
#include<vector>
#include<fstream>
char *user_id,*user_ids;
struct file_download
{
    string filename;
    string ip;
    string port;
    string destination;
};
int main(int args,char * argv[])
{
//argv[1] contains ip address
//argv[2] contains port
//argv[3] contains port of tracker
pthread_t thread1;
string server_port=string(argv[2]);//argv[1] for server port
//string client_port=string(argv[2]);//argv[2] for client port
int port=stoi(server_port);
//cout<<"this is port"<<port;
pthread_create(&thread1,NULL,server_function,(void *)&port);//creating server thread
pthread_detach(thread1);//detaching server thread
//client main thread


//creating socket for connecting to tracker

struct sockaddr_in clientaddr;
int new_socket,valread;
int sock_fd=socket(AF_INET,SOCK_STREAM,0);
//establishing connection 
clientaddr.sin_family=AF_INET;
string str=string(argv[3]);
clientaddr.sin_port=htons((stoi(str)));//port of tracker to get connected to it
cout<<"this is string"<<stoi(str);
clientaddr.sin_addr.s_addr=INADDR_ANY;//tracker ip address
if((connect(sock_fd,(struct sockaddr *)&clientaddr,sizeof(clientaddr)))<0) 
 {
  perror("error in binding");
  exit(1);
 }
int j=0,flag1=0;
char *user_id=(char * )malloc(sizeof(char )*25);
while(1)
{
//cout<<"this us"<<user_id<<endl;
string command;
getline(cin,command);//getting command
cout<<command<<endl;
if(command !=" ")
{
char command1[command.length()+1];
char com[command.length()+1];
//cout<<"memory allocated"<<endl;
strcpy(command1,command.c_str());
//cout<<"command1 "<<command1<<endl;
strcpy(com,command.c_str());
//cout<<"this is command1"<<command1<<endl;
char *tokens=strtok(command1," ");
if(!strcmp(tokens,"login"))
{
    cout<<"this is tokens in login "<<tokens<<endl;
    user_ids=strtok(NULL," ");
    strcpy(user_id,user_ids);
  //  cout<<"this is userid in login"<<user_id<<endl;
}
//cout<<"this is userid out of login"<<user_id<<endl;
//cout<<"this is token "<<tokens<<endl;;
char *t[20];
//cout<<"memory allocated"<<endl;
int i=0;
char status[12]="failure";
if(!strcmp(tokens,"create"))
{
send(sock_fd,com,strlen(com),0);
}
if(flag1==0)
{
while(!strcmp(status,"failure"))
{
  
  if(!strcmp(tokens,"login"))
  {
    
    send(sock_fd,com,strlen(com),0);
    read(sock_fd,status,sizeof(status));
    if(!strcmp(status,"success"))
    {
        flag1=1;
        break;
    }
  //cout<<"this is status "<<status<<endl;
  }
  else
  {
    cout<<"please login"<<endl;
  }
  if(flag1!=1)
  {
   string comn;
   getline(cin,comn);
   char com[comn.length()+1];
   strcpy(com,comn.c_str());
   char c1[comn.length()+1];
   strcpy(c1,comn.c_str());
   char *tokens=strtok(c1," ");
  }
}

//cout<<"this is sur id"<<user_id<<endl;
}
//cout<<"this is user id"<<user_id<<endl;
 if(!strcmp(tokens,"upload"))//command format: upload filepath
 {
    //argv[1] contains ip and argv[2] contains port
  //  cout<<"this is user_id"<<user_id<<endl;
    string coms=command+" "+argv[1]+" "+argv[2]+" "+user_id;
    char buf[coms.length()+1];
    strcpy(buf,coms.c_str());
    send(sock_fd,buf,strlen(buf),0);//sending file_path,ipaddress,port of peer to tracker
 }

 if(!strcmp(tokens,"create_group"))
 {
     //argv[1] && argv[2] gives ip and port of peer 
     string coms=command+" "+argv[1]+" "+argv[2]+" "+user_id;
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     send(sock_fd,buf,strlen(buf),0);

 }
 
 if(!strcmp(tokens,"list_groups"))
 {
     cout<<"entered list groups"<<endl;
     //argv[1] && argv[2] gives ip and port of peer 
     string coms=command+" "+argv[1]+" "+argv[2];
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     send(sock_fd,buf,strlen(buf),0);
      char buf1[1024];
     int n;
     while(n=(read(sock_fd,buf1,sizeof(buf1)))>0)
     {  
         if(!strcmp(buf1,"sent"))
         {
             break;
         }
        // cout<<"value of n"<<n<<endl;
      //   cout<<endl;
    //     cout<<"hello"<<endl;
         cout<<buf1<<endl;
     }
     //cout<<"came out of loop"<<endl;

 }

 if(!strcmp(tokens,"accept_requests"))
 {
     //argv[1] peerip and argv[2] peerport
     string coms=command+" "+argv[1]+" "+argv[2]+" ";
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     send(sock_fd,buf,strlen(buf),0);

 }

 if(!strcmp(tokens,"list_files"))
 {
     //argv[1] peerip and argv[2] peerport
     string coms=command+" "+argv[1]+" "+argv[2]+" ";
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     send(sock_fd,buf,strlen(buf),0);

 }
 if(!strcmp(tokens,"join_group"))
 {
     //argv[1] && argv[2] gives ip and port of peer 
     string coms=command+" "+argv[1]+" "+argv[2]+" "+user_id;
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     send(sock_fd,buf,strlen(buf),0);


 }
  
 if(!strcmp(tokens,"list_requests"))
 {
     //argv[1] && argv[2] gives ip and port of peer 
     string coms=command+" "+argv[1]+" "+argv[2];
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     send(sock_fd,buf,strlen(buf),0);
     char buf1[1024];
     int n;
     while((n=read(sock_fd,buf1,sizeof(buf1)))>0)
     {  
         if(!strcmp(buf1,"sent"))
         {
             break;
         }
       //  cout<<"value of n"<<n<<endl;
        // cout<<endl;
        // cout<<"hello"<<endl;
         cout<<buf1<<endl;
     }
    // cout<<"came out of loop"<<endl;
 }

 if(!strcmp(tokens,"leave_group"))
 {
     //argv[1] && argv[2] gives ip and port of peer 
     string coms=command+" "+argv[1]+" "+argv[2];
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     send(sock_fd,buf,strlen(buf),0);

 }

 
if(!strcmp(tokens,"download_file"))
 {
     //argv[1] && argv[2] gives ip and port of peer 
     string coms=command+" "+argv[1]+" "+argv[2];
     char buf[coms.length()+1];
     strcpy(buf,coms.c_str());
     //sending details to tracker to get info of where the file is
     send(sock_fd,buf,strlen(buf),0);
     /*getting details from tracker which represents where the file is
     i.e ip and port of peer having file*/
     char buff[1024];
     read(sock_fd,buff,sizeof(buff));
     cout<<"this is buff "<<buff<<endl;
     char **tok=string_tokens(buff);
     //creating a structure for file_download contains filepath,ip,port,destination
     struct file_download *fd=(struct file_download *)malloc(sizeof(struct file_download));
     fd->filename=tok[0];
     fd->ip=tok[1];
     fd->port=tok[2];
     fd->destination=tok[3];
     pthread_create(&thread1,NULL,client_function,(void *)fd);

//cout<<"this is length "<<strlen(commands)<<endl;
//cout<<"this is in while(1)"<<commands<<endl;
//
//pthread_create(&thread1,NULL,client_function,(void *)args);
//int sock_fd=create_socket(2003);
/*if command is get file or download receive ip and port
 from tracker which tells from where to download*/
}

}
}
}

char ** string_tokens(char * buf)
{

    char * t=strtok(buf," ");
    char **tok=(char **)malloc(sizeof(char *)*10);
    int i=0;
    while(t)
    {
        tok[i++]=t;
        t=strtok(NULL," ");
    }
    return tok;

}