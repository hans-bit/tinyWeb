#include"tinyweb.h"
#include<stdbool.h>


void get_filetype(char* filename,char* file_type){
//只支持 html,gif,png,jpg (根据11.7号题目要求,增加支持mp4文件),其余全部判定为text/plain 
    char* ext=NULL;
    
    ext=strrchr(filename,'.');

    if(strcmp(ext,".html")==0){
        strcpy(file_type,"text/html");

    }else if(strcmp(ext,".gif")==0){
        strcpy(file_type,"image/gif");

    }else if(strcmp(ext,".png")==0){
        strcpy(file_type,"image/png");

    }else if(strcmp(ext,".jpg")==0){
        strcpy(file_type,"image/jpeg");

    }else if(strcmp(ext,".ico")==0){
        strcpy(file_type,"image/icon");

    }else if(strcmp(ext,".mp4")==0){  //11.7题目
        strcpy(file_type,"video/mp4");

    }else{
        strcpy(file_type,"text/plain");

    }
}

void service_static(int fd,char* filename,__off_t filesize,bool is_head,http_request* this_request){
    char headers[MAXLINE]={};
    char file_type[20]={};

    get_filetype(filename,file_type);

    sprintf(headers,"HTTP/1.0 200 OK\r\n");
    sprintf(headers,"%sServer: HJ Tiny Web\r\n",headers);
    sprintf(headers,"%sContent-Type: %s; charset=UTF-8\r\n",headers,file_type);  
    //Content-Length 只算请求体的长度.
    
    if(strcmp(filename,"./debug/mirror")==0){  //11.6 A 题目
        char body[MAXLINE]={};
        
        strcpy(body,this_request->line_request);
        strcpy(body,this_request->headers_request);

        sprintf(headers,"%sContent-Lenght: %lu\r\n\r\n",headers,strlen(body));
        Rio_writen(fd,headers,strlen(headers));
        if(!is_head){
            Rio_writen(fd,body,strlen(body));
        }
        printf("debug response headers:\n%s",headers);
    }else{  //普通文件       
        sprintf(headers,"%sContent-Length: %lu\r\n\r\n",headers,filesize);
        Rio_writen(fd,headers,strlen(headers));
       
        if(!is_head){  //不是HEAD请求则返回完整响应
            typedef signed char byte;
            byte* request_map=(byte*)Malloc(filesize);
            
            /*request_map=Mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,request_fd,0);
            Close(request_fd);
            Munmap(request_map,filesize); mmap写法 */ 
           
            int request_fd=Open(filename,O_RDONLY,0);
            Rio_readn(request_fd,request_map,filesize);  //11.9题目 
            Rio_writen(fd,request_map,filesize);
            Close(request_fd);
        }
        printf("fd=%d\tstatic response headers:\n%s",fd,headers);
    }

    fflush(stdout);
}
