#include<string.h>
#include<stdbool.h>

//解析uri,并填充filename和cgi_args这两个数组,返回true是静态文件,返回false是动态文件
int parse_uri(char* uri,char* filename,char* cgi_args){
    if(memcmp(uri,"/cgi-bin/",9)==0){//在/cgi-bin/中的文件都是动态文件
        if(uri[strlen(uri)-1]=='/'){
            strcpy(filename,"./cgi-bin/test.out");
        }else{
            char* ptr=strchr(uri,'?');
            if(ptr!=NULL){
                strcpy(cgi_args,ptr+1);  //cgi_args="a=x&b=y&c=z"
                *ptr='\0';
            }
            strcpy(filename,".");
            strcat(filename,uri);
        }
        return false;
    }else{//静态文件处理,会忽略后续参数
        if(strcmp(uri,"/")==0){ //默认访问静态的index.html
            strcpy(filename,"./index.html");
        }else{ //没有对uri中的文件进行访问权限检查,可能有bug
            char* ptr=strchr(uri,'?');
            if(ptr!=NULL)
                *ptr='\0';
            strcpy(filename,".");
            strcat(filename,uri);
        }
        return true; 
    }
}



