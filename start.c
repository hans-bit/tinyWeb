#include"csapp_alter.h"
#include<stdbool.h>
#include"tinyweb.h"
#include"sbuf.h"

/* 并行服务器
 * 用预线程化并行处理每个请求
 * 2021年12月01日22:40:39
 * */

static const size_t THREAD_NUM=2;  //一般情况下,线程数量最好就是cpu核心数,本机为8,减去已经运行的main线程,则为7
static const size_t SBUF_NUM=MAXLINE;
static sbuf_t sbuf;


//管道破裂信号处理程序,即对等方关闭了socket.
void sigpipe_handler(int sig){  //11.13题目,但注意变为多线程后,每个线程也需要处理这个信号.
    fprintf(stderr,"The peer closes the connection and the remaining bytes may not be received\n");
}

//子程序运行结束的信号处理程序,cgi会用到.
void sigchid_handler(int sig){  //11.8题
    wait(NULL);  //显式回收子程序
}

//打开并返回指定服务(端口)的监听套接字,成功返回非负文件描述符,失败返回-1
int open_listenfd_my(char* service){
    struct addrinfo hints={.ai_family=AF_INET,  //socket类型,AF_INET是ipv4
                           .ai_socktype=SOCK_STREAM,
                           .ai_flags=AI_ADDRCONFIG|AI_PASSIVE  //AI_PASSIVE 作为服务器套接字(被动套接字),此时host应该是NULL.
    };
    struct addrinfo* result=NULL;
    int listenfd=-1;
    int ret_code=0;
    if((ret_code=getaddrinfo(NULL,service,&hints,&result))!=0){
        fprintf(stderr,"errno:%d %s\n",ret_code,gai_strerror(ret_code));
        return ret_code;
    }else{
        for(struct addrinfo* p=result;p!=NULL;p=p->ai_next){
            if((listenfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
                listenfd=-1;
                perror(strerror(errno));
                continue;
            }
            if(bind(listenfd,p->ai_addr,p->ai_addrlen)==-1){
                close(listenfd);
                listenfd=-1;
                perror(strerror(errno));
                continue;
            }
            if(listen(listenfd,1024)==-1){
                close(listenfd);
                listenfd=-1;
                perror(strerror(errno));
                continue;
            }
            break;
        }
        freeaddrinfo(result);
        return listenfd;
    }
}


void* thread(void* nul){
    pthread_detach(pthread_self());  //剥离自己,变为不可结合线程.
    Signal(SIGPIPE,sigpipe_handler);
    Signal(SIGCHLD,sigchid_handler);
    while(true){
        int fd=sbuf_pop(&sbuf);
        doit(fd); //由doit负责关闭fd
    }
    return NULL; 
}

//监听连接,并调用处理函数
int main(int argc,char** argv){
    if(argc!=2) {
        fprintf(stderr,"usage:%s <port>\n",argv[0]);
        return 1;
    }
    int listenfd=-1;
    if((listenfd=open_listenfd_my(argv[1]))==-1){
        fprintf(stderr,"get listenfd error.\n");
        return 1;
    }
    struct sockaddr clientaddr={};
    socklen_t clientlen=0;
    sbuf_init(&sbuf,SBUF_NUM);

    for(int i=0;i<THREAD_NUM;++i){
        pthread_t pid;
        Pthread_create(&pid,NULL,thread,NULL);
    }

    while(true){
        int clientfd=Accept(listenfd,&clientaddr,&clientlen);
        if(clientfd <= 0)
            goto finally;
        sbuf_push(&sbuf,clientfd);
    }
finally:
    sbuf_deinit(&sbuf);
}


