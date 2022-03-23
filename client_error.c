#include"csapp_alter.h"
#include"tinyweb.h"

char* get_status_name(int status_code,char* status_name){
    if(status_code==100)
        strcpy(status_name,"Continue");
    else if(status_code==101)
        strcpy(status_name,"Switching Protocols");
    else if(status_code==200)
        strcpy(status_name,"OK");
    else if(status_code==201)
	    strcpy(status_name,"Created");
    else if(status_code==202)
    	strcpy(status_name,"Accepted");
    else if(status_code==203)
    	strcpy(status_name,"Non-Authoritative");
    else if(status_code==204)
    	strcpy(status_name,"No Content");
    else if(status_code==205)
    	strcpy(status_name,"Reset Content");
    else if(status_code==206)
    	strcpy(status_name,"Partial Content");
    else if(status_code==300)
    	strcpy(status_name,"Multiple Choices");
    else if(status_code==301)
    	strcpy(status_name,"Moved Permanently");
    else if(status_code==302)
    	strcpy(status_name,"Found");
    else if(status_code==303)
    	strcpy(status_name,"See Other");
    else if(status_code==304)
    	strcpy(status_name,"Not Modified");
    else if(status_code==305)
    	strcpy(status_name,"Use Proxy");
    else if(status_code==306)
    	strcpy(status_name,"Unused");
    else if(status_code==307)
    	strcpy(status_name,"Temporary Redirect");
    else if(status_code==400)
    	strcpy(status_name,"Bad Request");
    else if(status_code==401)
    	strcpy(status_name,"Unauthorized	");
    else if(status_code==402)
    	strcpy(status_name,"Payment Required");
    else if(status_code==403)
    	strcpy(status_name,"Forbidden");
    else if(status_code==404)
    	strcpy(status_name,"Not Found");
    else if(status_code==405)
    	strcpy(status_name,"Method Not Allowed");
    else if(status_code==406)
    	strcpy(status_name,"Not Acceptable");
    else if(status_code==407)
    	strcpy(status_name,"Proxy Authentication Required");
    else if(status_code==408)
    	strcpy(status_name,"Request Time-out");
    else if(status_code==409)
    	strcpy(status_name,"Conflict");
    else if(status_code==410)
    	strcpy(status_name,"Gone");
    else if(status_code==411)
    	strcpy(status_name,"Length Required	Content-Length");
    else if(status_code==412)
    	strcpy(status_name,"Precondition Failed");
    else if(status_code==413)
    	strcpy(status_name,"Request Entity Too Large");
    else if(status_code==414)
    	strcpy(status_name,"Request-URI Too Large");
    else if(status_code==415)
    	strcpy(status_name,"Unsupported Media Type");
    else if(status_code==416)
    	strcpy(status_name,"Requested range not satisfiable");
    else if(status_code==417)
    	strcpy(status_name,"Expectation Failed");
    else if(status_code==500)
    	strcpy(status_name,"Internal Server Error");
    else if(status_code==501)
    	strcpy(status_name,"Not Implemented");
    else if(status_code==502)
    	strcpy(status_name,"Bad Gateway");
    else if(status_code==503)
    	strcpy(status_name,"Service Unavailable");
    else if(status_code==504)
    	strcpy(status_name,"Gateway Time-out");
    else if(status_code==505)
    	strcpy(status_name,"HTTP Version not supported");
    else
        return NULL;
    return status_name;
}

void client_error(int fd,int status_code,char* msg){
    char headers[MAXLINE]={};
    char body[MAXLINE]={};
    char status_name[40]={};

    if(get_status_name(status_code,status_name)==NULL){
        strcpy(status_name,"user-define");
    }

    sprintf(body,"<html><head><title>%d %s</title></head><body><h1>%s</h1></body></html>",status_code,status_name,msg);
    
    sprintf(headers,"HTTP/1.0 %d %s\r\n",status_code,status_name);
    sprintf(headers,"%sServer: HJ Tiny Web\r\n",headers);
    sprintf(headers,"%sContent-Type: text/html; charset=UTF-8\r\n",headers);  
    sprintf(headers,"%sContent-Length: %ld\r\n\r\n",headers,strlen(body));
    //Content-Length 只算请求体的长度.
    
    Rio_writen(fd,headers,strlen(headers));
    Rio_writen(fd,body,strlen(body));
    printf("fd=%d\tclient error response headers:\n%s",fd,headers);

    fflush(stdout);
}
