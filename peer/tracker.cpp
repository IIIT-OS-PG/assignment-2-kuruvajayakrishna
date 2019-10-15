#include "header.h"
#include<fstream>
void *handle(void *ptr);
vector<struct grp_info> load_content();
vector<struct grp_info> load_join_requests();
char **string_tokens(char *buffer);

struct file_info
{
   string filepath;
   string grp_id;
   string ip,port;
   string user_id;
};
struct usr_info
{
   string usr_name;
   string usr_pass;
};

struct grp_info
{
    string gid;
    string peer_ip;
    string peer_port;
    string usr_id;
};

struct download_file
{
   string grpid;
   string filename;
   string destination;
   string ip;
   string port;
};

struct grp_info vr;
vector<struct file_info>v;
vector<struct usr_info>u;
vector<struct usr_info>active_users;
vector<struct grp_info>grp;
vector<struct grp_info>join_req;

int main(int args,char *argv[])
{
//cout<<"entered main"<<endl;
//getting file details from file_info file at starting of tracker  
ifstream f_in;
f_in.open("file_info.txt");
string lines;
struct file_info var;
while(getline(f_in,lines))
{
cout<<lines<<endl;
char buf[lines.length()+1];
strcpy(buf,lines.c_str());
char **tokl=string_tokens(buf);
//cout<<"tok[0]"<<tokl[0]<<endl;
var.filepath=tokl[0];
var.grp_id=tokl[1];
var.ip=tokl[2];
var.port=tokl[3];
var.user_id=tokl[4];
v.push_back(var);
}
f_in.close();
//getting usr_details and storing in data structure at start of tracker
f_in.open("user_info.txt");
struct usr_info u_i;
while(getline(f_in,lines))
{
   char buf[lines.length()+1];
   strcpy(buf,lines.c_str());
   char **tokl=string_tokens(buf);
   u_i.usr_name=tokl[0];
   u_i.usr_pass=tokl[1];
   u.push_back(u_i);
}
f_in.close();

//getting contents of group from group_info file
grp=load_content();
//cout<<"this is grp size "<<grp.size()<<endl;
//getting join-requests list into a vector
join_req=load_join_requests();

/*   */
//char *s=argv[1];
//cout<<"ip:"<<s<<endl;   
//int port=*((int*)argv[1]);
//establish connection
int opt=1;
//creating socket at server end 
int server_socket=socket(AF_INET,SOCK_STREAM,0);
//using setsockptions so as to reuse the port
if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
 { 
  perror("setsockopt");
  exit(1); 
 } 
//bind address to socket
string str=string(argv[1]);
struct sockaddr_in servaddr;
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(stoi(str));
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
pthread_create(&t,NULL,handle,pclient); 
}
}


char ** string_tokens(char * buffer)
{
   char **t=(char **)malloc(sizeof(char *)*256);
   char *tokens=strtok(buffer," ");
   int i=0;
   while(tokens)
   {
      t[i++]=tokens;
      tokens=strtok(NULL," ");
   }
   return t;
}

void * handle(void *ptr)
{
   while(1)
   {
int new_socket=*((int *)ptr);
   char buffer[1024]={0};
read(new_socket,buffer,sizeof(buffer));
//cout<<"this is buffer "<<buffer<<endl;
char **tokens=string_tokens(buffer);
//cout<<"this is tokens[0] "<<tokens[0]<<endl;
int flag=0;
   if(!strcmp(tokens[0],"create"))
   {
      struct usr_info val;
      val.usr_name=tokens[1];
      val.usr_pass=tokens[2];
      vector<struct usr_info>::iterator itr;
      int flag=0;
      for(itr=u.begin();itr!=u.end();itr++)
      {
         struct usr_info us;
         us=*itr;
         if(val.usr_name== us.usr_name )
         {
            cout<<"already exists please choose another logid's"<<endl;
            flag=1;
            break;
         }

      }
      if(flag!=1)
      {
      ofstream f_output;
      f_output.open("user_info.txt",std::ios_base::app);
      f_output<<tokens[1]<<" ";
      f_output<<tokens[2]<<endl;
      f_output.close();
      u.push_back(val);  
      }
      
      
   }
   else if (!strcmp(tokens[0],"login"))
   {
      struct usr_info val;
      val.usr_name=tokens[1];
      cout<<"this is tokens[1]"<<tokens[1]<<endl;
      val.usr_pass=tokens[2];
      cout<<"this is tokens[2]"<<tokens[2]<<endl;
      vector<struct usr_info>::iterator itr;
      int flag=0;
      for(itr=u.begin();itr!=u.end();itr++)
      {
         struct usr_info us;
         us=*itr;
         if(val.usr_name== us.usr_name && val.usr_pass== us.usr_pass)
         {
            cout<<"successfully logged in"<<endl;
            active_users.push_back(val);
            send(new_socket,"success",strlen("success"),0);
            flag=1;
            break;
         }

      }
      if(flag!=1)
      {
         cout<<"invalid credentials"<<endl;
         send(new_socket,"failure",strlen("failure"),0);
      }
   }
   else if(!strcmp(tokens[0],"upload"))
   {
      struct file_info fi;
      //cout<<"this is tokens[1] "<<tokens[1]<<endl;
      //takes filepath
      fi.filepath=tokens[1];
      //cout<<"this is tokens[2] "<<tokens[2]<<endl;
      //this is grp_id
      fi.grp_id=tokens[2];
      //this contains peer_ip
      //cout<<"this is tokens[3] "<<tokens[3]<<endl;
      fi.ip=tokens[3];
      //contains peer_port
      //cout<<"this is tokens[4]"<<tokens[4]<<endl;
      fi.port=tokens[4];
      //usr_id
      //cout<<"this is token[5]"<<tokens[5];
      fi.user_id=tokens[5];
      vector<struct file_info>::iterator itr;
      if(v.size()==0)
      {
         v.push_back(fi);
            ofstream f_output;
            f_output.open("file_info.txt",std::ios_base::app);
            f_output<<fi.filepath<<" "<<fi.grp_id<<" "<<fi.ip<<" "<<fi.port<<" "<<fi.user_id<<endl;
            f_output.close();
      }
      else
      {
         int flag=0;
         
           for(itr=v.begin();itr!=v.end();itr++)
          {
            struct file_info f;
             f=*itr;
             if(fi.filepath == f.filepath && fi.grp_id==f.grp_id && fi.ip == f.ip && fi.port == f.port && fi.user_id == f.user_id)
             {
              char mes[20]="file already exist";
              send(new_socket,mes,strlen(mes),0);
              cout<<"mes sent to client"<<endl;
              flag=1;
              break;
              }
         
      }
      if(flag==0)
      {
            v.push_back(fi);
            ofstream f_output;
            f_output.open("file_info.txt",std::ios_base::app);
            f_output<<fi.filepath<<" "<<fi.grp_id<<" "<<fi.ip<<" "<<fi.port<<" "<<fi.user_id<<endl;
            f_output.close();  
      }
      }

   }
    else if(!strcmp(tokens[0],"create_group"))
       {
          struct grp_info g;
          g.gid=tokens[1];
          g.peer_ip=tokens[2];
          g.peer_port=tokens[3];
          g.usr_id=tokens[4];
          //check if group already exists in file 
          if(grp.size()==0)//creating first group && grp is a vector
          {
           ofstream f_output;
           f_output.open("grp_info.txt",std::ios_base::app);
           f_output<<g.gid<<" "<<g.peer_ip<<" "<<g.peer_port<<" "<<g.usr_id<<endl;
           grp.push_back(g);
           f_output.close();
          }
          else
          {
             int flag=0;
             vector<struct grp_info>::iterator itr;
             for(itr=grp.begin();itr!=grp.end();itr++)
             {
                struct grp_info gi=*itr;
                if(gi.gid==g.gid && gi.peer_ip==g.peer_ip && gi.peer_port==g.peer_port && gi.usr_id==g.usr_id)
                {
                   flag=1;
                   cout<<"group already exists"<<endl;
                   break;
                }
             }
             if(flag!=1)
             {
                ofstream f_output;
                f_output.open("grp_info.txt",std::ios_base::app);
                f_output<<g.gid<<" "<<g.peer_ip<<" "<<g.peer_port<<" "<<g.usr_id<<endl;
                f_output.close();
                grp.push_back(g);
             }
          }
          

       }

       else if(!strcmp(tokens[0],"list_groups"))
       {
          ifstream f_input;
          f_input.open("grp_info.txt");
          string lines;
          while(getline(f_input,lines))
          {
             char buf[lines.length()+1];
             strcpy(buf,lines.c_str());
             //cout<<buf<<endl;
             send(new_socket,buf,strlen(buf),0);
          }
          send(new_socket,"sent",sizeof("sent"),0);
          //cout<<"came out of loop"<<endl;
          f_input.close();
       }

       else if(!strcmp(tokens[0],"join_group"))
       {
         struct grp_info g;
         //getting data from peer includes(group_id,peer_ip,peer_port)
         g.gid=tokens[1];//tokens[1] contains group_id
         //cout<<"this is token[1]"<<tokens[1]<<endl;
         //cout<<"this is token[2]"<<tokens[2]<<endl;
         g.peer_ip=tokens[2];//contains ip of peer
         g.peer_port=tokens[3];//contains port of peer
         g.usr_id=tokens[4];
         vector<struct grp_info>::iterator itr;
         struct grp_info gp;
         if(grp.size()==0)
         {
            cout<<"there is  no group"<<endl;
         }
         //getting details from group from already loaded content
         for(itr=grp.begin();itr!=grp.end();itr++)
         { 
            //compare group id if they are same print the corresponding into a file 
            gp=*itr;
            //cout<<gp.gid<<"===="<<g.gid<<endl;
            if(gp.gid==g.gid)
            {
             ofstream f_out;
             f_out.open("join_requests.txt",std::ios_base::app);
             f_out<<g.gid<<" "<<g.peer_ip<<" "<<g.peer_port<<" "<<g.usr_id<<" "<<endl;
             f_out.close();
             join_req.push_back(g);
             break;
            }
            else
            {
               cout<<"please enter valid groupid to send request"<<endl;
            }
            

         }
       }
       else if(!strcmp(tokens[0],"list_requests"))
       {
          vector<struct grp_info>::iterator itr;
          ifstream f_in;
          f_in.open("join_requests.txt");
          string lines;
          while(getline(f_in,lines))
          {
             char buf[lines.length()+1];
             strcpy(buf,lines.c_str());
             send(new_socket,buf,strlen(buf),0);
          }
         
          send(new_socket,"sent",sizeof("sent"),0);
          cout<<"out of loop"<<endl;
           f_in.close();
       }
       else if(!strcmp(tokens[0],"accept_requests"))
       {
          //command contains groupid,userid,appended details ip and port
          //getting command details from peer or owner to accept
          struct grp_info gp;
          gp.gid=tokens[1];
          //tokens[2] contains user_id
          gp.usr_id=tokens[2];
          gp.peer_ip=tokens[3];
          gp.peer_port=tokens[4];
          vector<struct grp_info>::iterator itr;
          struct grp_info g;
          //checking if current peer is has a right to accept i.e is he a owner
         /* for(itr=grp.begin();itr!=grp.end();itr++)
          {
             g=*itr;
          }*/
          //matching above details with join_requests available in join_req file
          for(itr=join_req.begin();itr!=join_req.end();itr++)
          {
             g=*itr;
            if(gp.gid==g.gid && gp.usr_id==g.usr_id)//must include user_id
            {
               ofstream f_out;
               f_out.open("grp_users_info.txt",std::ios_base::app);
               f_out<<g.gid<<" "<<g.peer_ip<<" "<<g.peer_port<<" "<<g.usr_id<<endl;
               f_out.close();
               join_req.erase(itr);
               f_out.open("join_requests.txt");
               for(itr=join_req.begin();itr!=join_req.end();itr++)
               {
                  struct grp_info gg;
                  gg=*itr;
                  f_out<<gg.gid<<" "<<gg.peer_ip<<" "<<gg.peer_port<<" "<<gg.usr_id<<endl;
               }
               f_out.close();
               break;
            }
          }

          //getting details from datastructure(vector)containing join_requests of peers


       }
       else if(!strcmp(tokens[0],"leave_group"))
       {
          int flag=0;
          struct grp_info g;
          g.gid=tokens[1];
          g.peer_ip=tokens[2];
          g.peer_port=tokens[3];
          struct grp_info gi;
          vector<struct grp_info>::iterator itr;
          for(itr=grp.begin();itr!=grp.end();itr++)
          {
            
             gi=*itr;
             if(gi.gid==g.gid && gi.peer_ip==g.peer_ip && gi.peer_port==g.peer_port)
             {
                grp.erase(itr);
                flag=1;
                break;
             }
          }
          if(flag==1)
          {
             ofstream f_o;
             f_o.open("grp_info.txt");
             for(itr=grp.begin();itr!=grp.end();itr++)
             {
                gi=*itr;
                f_o<<gi.gid<<" "<<gi.peer_ip<<" "<<gi.peer_port<<endl;
             }
             f_o.close();
          }

       }
       
       else if(!strcmp(tokens[0],"list_files"))
       {
         vector<struct file_info>::iterator itr;
         struct file_info fr;
         for(itr=v.begin();itr!=v.end();itr++)
         { 
            //compare group id if they are same print the corresponding file 
            fr=*itr;
            cout<<fr.filepath<<" "<<fr.grp_id<<" "<<fr.port<<endl;

         }
       }
       else if(!strcmp(tokens[0],"download_file"))
       {
          struct download_file d;
          //cout<<"this is token[1] "<<tokens[1]<<endl;
          d.grpid=tokens[1];
          //cout<<"this is token[2] "<<tokens[2]<<endl;
          d.filename=tokens[2];
          //cout<<"this is token[3] "<<tokens[3]<<endl;
          d.destination=tokens[3];
          //cout<<"this is tokens[4] "<<tokens[4]<<endl;
          d.ip=tokens[4];
          //cout<<"this is tokens[5] "<<tokens[5]<<endl;
          d.port=tokens[5];
          /*getting details from file_info about file,peer_ip,peer_port 
          who has the file by comparing d.grpid with file.grpid i.e v.grpid*/
          vector<struct file_info>::iterator itr;
          struct file_info f;
          struct grp_info gi;
          /*check if he is a member of group by getting details from user_group_info file
            this file contains gid,ip,port,user_id
          for(itr=grp.begin();itr!=grp.end();itr++)
          {
             
          }*/
          for(itr=v.begin();itr!=v.end();itr++)
          {
             f=*itr;
             string str="";
             if(f.grp_id==d.grpid && f.filepath==d.filename)
             {
                str=str+d.filename+" "+f.ip+" "+f.port+" "+d.destination;
                char buf[str.length()+1];
                strcpy(buf,str.c_str());
                //cout<<"this is details of peer having a file "<<buf<<endl;
                //sending ip and port number of peer having file
                send(new_socket,buf,strlen(buf),0);
                break;
             }

          }




       }
   
   }
}

vector<struct grp_info> load_content()
{
    vector<struct grp_info>gp;
    ifstream f_input;
    f_input.open("grp_info.txt");
    string lines;
    struct grp_info vr;
    while(getline(f_input,lines))
    { 
        char buf[lines.length()+1];
        strcpy(buf,lines.c_str());
       // cout<<"this is buffer in loading"<<buf<<endl;
        char **tokl=string_tokens(buf);
        //cout<<"this is tokl[0]"<<tokl[0]<<endl;
        vr.gid=tokl[0];
        //cout<<"this is tokl[1]"<<tokl[1]<<endl;
        vr.peer_ip=tokl[1];
        //cout<<"this is tokl[2]"<<tokl[2]<<endl;
        vr.peer_port=tokl[2];
        //cout<<"this is vr.port"<<vr.peer_port<<endl;
        vr.usr_id=tokl[3];
        gp.push_back(vr);
    }
    f_input.close();
return gp;
}

vector<struct grp_info> load_join_requests()
{
    vector<struct grp_info>gp;
    ifstream f_input;
    f_input.open("join_requests.txt");
    string lines;
    struct grp_info vr;
    while(getline(f_input,lines))
    { 
        char buf[lines.length()+1];
        strcpy(buf,lines.c_str());
        char **tokl=string_tokens(buf);
        vr.gid=tokl[0];
        vr.peer_ip=tokl[1];
        vr.peer_port=tokl[2];
        vr.usr_id=tokl[3];
        gp.push_back(vr);
    }
    f_input.close();
return gp;
}