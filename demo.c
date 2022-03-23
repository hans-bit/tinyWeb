#include<stdio.h>
#include<string.h>
int main(void){
    char str[255];
    while(1){
        printf("input:");
        scanf("%s",str);
        if(strcmp(str,"exit")==0)
            break;
        else
            printf("%s:lenght of str is %lu\n",str,strlen(str));
    }
}
