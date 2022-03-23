#include"tinyweb.h"

//控制台打印请求行和请求头
//支持
//  静态内容(./[filename] || index.html):html,gif,png,jpg text
//  动态内容(./cgi-bin/cginame[?var=x[&let=y]]):
//  实现HTTP/1.0支持三个的请求:GET HEAD POST


bool is_support(const char* method){
    static const char support_method[3][10]={"GET","HEAD","POST"};
    if(method==NULL)
        return false;
    for(int i=0;i<3;++i){
        if(strcmp(method,support_method[i])==0){
            return true; 
        }
    }
    return false;
}


void doit(int fd){

    http_request this_request={};
    char method[10]={};
    char http_version[10]={};
    char uri[MAXLINE]={};
    char filename[MAXLINE]={};
    char cgi_args[MAXLINE]={};    

    rio_t rio;
    Rio_readinitb(&rio,fd);
    if(fill_http_request(&rio,&this_request)==false)
        goto finally;  //填充失败,可能是http报文不合规范
    sscanf(this_request.line_request,"%s %s %s",method,uri,http_version);
    
    if(is_support(method)!=true){ //过滤不支持的请求方法.
         client_error(fd,405,"Unsupported request method."); 
         goto finally;
    }
    
    //打印请求行和请求头,注意,多线程情况下,打印的请求和响应会乱序,所以加上一个描述符来标记.
    printf("fd=%d\thttp request:\n%s%s\n",fd,this_request.line_request,this_request.headers_request);

    bool is_static=parse_uri(uri,filename,cgi_args);  //在这里填充缺省文件
    struct stat file_stat;
    if(stat(filename,&file_stat)==-1){
        client_error(fd,404,"File Not found.");
        goto finally;
    } 

    if(is_static==true){  //静态内容
        //S_IRUSR 0400  110010000
        if(!S_ISREG(file_stat.st_mode) || !(S_IRUSR & file_stat.st_mode)){  //是一个普通文件且该账户拥有读权限.
            client_error(fd,403,"Tiny web couldn't read the file or directory.");
            goto finally;
        }
        bool is_head=strcmp(method,"HEAD")==0?true:false;
        service_static(fd,filename,file_stat.st_size,is_head,&this_request);

    }else{  //动态内容
        if(!S_ISREG(file_stat.st_mode) || !(S_IXUSR & file_stat.st_mode)){
            client_error(fd,403,"Tiny web couldn't run the CGI program.");
            goto finally;
        }
        service_dynamic(fd,filename,cgi_args);
    }
finally:  //统一资源处理
    Close(fd);
    fflush(stdout);
}
