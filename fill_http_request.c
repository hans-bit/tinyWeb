#include"tinyweb.h"

//填充完整的请求头到http_request,如果成功返回true,失败返回false
bool fill_http_request(rio_t* rio,http_request* this_request){
    char buf[MAXLINE]={};
    if(Rio_readlineb(rio,buf,sizeof(buf))<=0)  //读取请求行
        return false;
    strcpy(this_request->line_request,buf);  //填充请求行

    memset(buf,0,sizeof(buf));
    this_request->headers_request[0]='\0';  //清空请求头
    while(strcmp(buf,"\r\n")!=0){//填充请求头,当请求头不完整时跳出
        if(Rio_readlineb(rio,buf,sizeof(buf))<=0)
            return false;
        strcat(this_request->headers_request,buf);
    }
    return true;
    //请求体暂时不填充
}

