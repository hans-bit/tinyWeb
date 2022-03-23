#pragma once

#ifndef TINYWEB_H
#define TINYWEB_H

#include"csapp_alter.h"
#include<stdbool.h>
#include<sys/select.h>

typedef struct{
    char line_request[MAXLINE];
    char headers_request[MAXLINE];
    char body_request[MAXLINE];
}http_request;

void doit(int);
bool fill_http_request(rio_t*,http_request*);
int parse_uri(char*,char*,char*); //uri,filename,cgi_args
void client_error(int,int,char*); //fd,status_code,error_msg
void service_static(int,char*,__off_t,bool,http_request*);
void service_dynamic(int,char*,char*);  //fd,filename,cgi_args
void sigpipe_handler(int);  //SIGPIPE handler

#endif
