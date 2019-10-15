#include<iostream>
#include <netinet/in.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include <arpa/inet.h>
#include<fcntl.h>
#include<vector>
#include<fstream>
using namespace std;
void * server_function(void *ptr);
void client_function(int port);
void * handle_connection(void *pclient);
void get_file();
char **string_tokens(char * buf);