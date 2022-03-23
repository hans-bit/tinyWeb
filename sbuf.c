#include"sbuf.h"
#include"csapp_alter.h"
/*
 * 循环队列,尾指针指向待插入的元素的位置(也就是最后一个元素的后面).
 * 判空判满由信号量决定,所以不用浪费一个元素位置或者添加一个tag来判断空满.
 * */

//初始化一个sbuf_t
void sbuf_init(sbuf_t* sp,int n){
    sp->buf=Calloc(n,sizeof(int));
    sp->n=n;
    sp->front=sp->rear=0;
    Sem_init(&sp->mutex,0,1);
    Sem_init(&sp->slots,0,n);
    Sem_init(&sp->items,0,0);
}

void sbuf_deinit(sbuf_t* sp){
    free(sp->buf);
}

//一个元素从尾部压入队列
void sbuf_push(sbuf_t* sp,int item){
    P(&sp->slots);  //等待到有或直接减少一个空槽位,为插入元素做准备.(同步锁)
    P(&sp->mutex);  //互斥锁加锁
    sp->buf[sp->rear]=item;
    sp->rear=(sp->rear+1)%sp->n;
    V(&sp->mutex);  //互斥锁解锁
    V(&sp->items);  //可用元素数量加一.
}

//一个元素从头部出队列
int sbuf_pop(sbuf_t* sp){
    int item=0;
    P(&sp->items);  //等待到有或减少一个可用元素,为弹出元素做准备.(同步锁)
    P(&sp->mutex);  //互斥锁加锁
    item=sp->buf[sp->front];
    sp->front=(sp->front+1)%sp->n;
    V(&sp->mutex);  //互斥锁解锁
    V(&sp->slots);  //空槽位数量加一
    return item;
}
