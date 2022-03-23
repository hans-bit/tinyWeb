#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAXLINE 8192

int main(int argc,char** argv,char** env){
    char body[MAXLINE];
    memset(body,0,sizeof(body));

    char* param=getenv("QUERY_STRING");
    if(param==NULL){
        fprintf(stderr,"QUERY_STRING error\n");
        return 1;
    }

    char* num1=strchr(param,'=');
    char* num2=strrchr(param,'=');
    char* p=strchr(param,'&');

    if(p!=NULL && num1!=NULL && num2!=NULL){
        *p='\0';
        int one=atoi(num1+1);
        int two=atoi(num2+1);
        sprintf(body,"%d",one+two);
    }
    

    printf("Content-Lenght: %lu\r\n\r\n",strlen(body));
    printf("%s",body);
    fflush(stdout);
}
