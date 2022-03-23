#include<unistd.h>
#include<stdio.h>
#include<string.h>
int main(){
    sleep(5);
    printf("Content-Lenght: 17\r\n\r\n");
    printf("sleeped 5 secone!");
    fflush(stdout);
}

