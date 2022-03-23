#pragma once

#ifndef SBUF_H
#define SBUF_H

#include<semaphore.h>
typedef struct{
    int* buf;  //循环队列缓冲区
    int n;  //缓冲区长度
    int front;  //队列第一个元素下标
    int rear;  //队列最后一个元素的后一位(待插入位)的下标
    sem_t mutex;  //互斥信号量
    sem_t slots;  //缓冲区空闲槽位
    sem_t items;  //缓冲区可用项目
    //加锁队列,不用预留空间判断队空队满.
}sbuf_t;

void sbuf_init(sbuf_t*,int);
void sbuf_deinit(sbuf_t*);
void sbuf_push(sbuf_t*,int);
int sbuf_pop(sbuf_t*);

#endif

