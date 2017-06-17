#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

#define CAP 4
#define TABLE_SIZE 8
#define ITEM_COUNT 1000

char table[]={'a','b','c','d','e','f','g','h'};

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t wait_empty_buffer1;
pthread_cond_t wait_full_buffer1;
pthread_cond_t wait_empty_buffer2;
pthread_cond_t wait_full_buffer2;

typedef struct{
    char buf[CAP+1];
    int head;
    int tail;
}buffer;

buffer b1,b2;

int buffer_full(buffer *b){
    if((b->tail+CAP)%(CAP+1)==b->head)
        return 1;
    else
        return 0;
}

int buffer_empty(buffer *b){
    if(b->tail==b->head)
        return 1;
    else
        return 0;
}

void produce(buffer *b,char c){
    b->buf[b->head]=c;
    b->head=(b->head+1)%(CAP+1);
    printf("tail=%d,head=%d\n",b->tail,b->head);
}

//从b1取一个到b2
void count(buffer *b1,buffer *b2){
//    char temp=b1->buf[b1->tail]-32;
    b2->buf[b2->head]=b1->buf[b1->tail]-32;
    b2->head=(b2->head+1)%(CAP+1);
    b1->tail=(b1->tail+1)%(CAP+1);
}

char comsume(buffer *b){
    char c=b->buf[b->tail];
//    printf("comsumer print:%c\n",c);
    b->tail=(b->tail+1)%(CAP+1);
    printf("tail=%d,head=%d\n",b->tail,b->head);
    return c;
}

void *pro_thr(void *arg){
    for(int i=0;i<ITEM_COUNT;i++)
    {
        //printf("producer:我看看b1是不是满的\n");
        //printf("producer:我准备把mutex1锁了\n");
        pthread_mutex_lock(&mutex1);
        //printf("producer:我已经把mutex1锁了\n");

        while(buffer_full(&b1)){
            //printf("producer:b1满了\n");
            pthread_cond_wait(&wait_empty_buffer1,&mutex1);
        }
        char c=table[rand()%TABLE_SIZE];
        produce(&b1,c);
        pthread_cond_signal(&wait_full_buffer1);
        printf("producer:我生产了 %d %c\n",i+1,c);
        pthread_mutex_unlock(&mutex1);
        //printf("producer:我已经把mutex1解了\n");
    }
}

void *cou_thr(void *arg){
    for(int i=0;i<ITEM_COUNT;i++){
        //printf("counter:我看看b1是不是空的\n");
        //printf("counter:我准备把mutex1锁了\n");
        pthread_mutex_lock(&mutex1);
        //printf("counter:我已经把mutex1锁了\n");

        while(buffer_empty(&b1)){
            //printf("counter:b1是空的\n");
            pthread_cond_wait(&wait_full_buffer1,&mutex1);
        }

        char c=comsume(&b1);
        pthread_cond_signal(&wait_empty_buffer1);
        pthread_mutex_unlock(&mutex1);
        //printf("counter:我已经把mutex1解了\n");

        c=c-32;
        //printf("counter:我看看b2是不是满的\n");
        //printf("counter:我准备把mutex2锁了\n");
        pthread_mutex_lock(&mutex2);
        //printf("counter:我已经把mutex2锁了\n");

        while(buffer_full(&b2)){
            //printf("counter:b2是满的\n");
            pthread_cond_wait(&wait_empty_buffer2,&mutex2);
        }

        produce(&b2,c);
        pthread_cond_signal(&wait_full_buffer2);
        printf("counter:我转换了 %d %c\n",i+1,c);
        pthread_mutex_unlock(&mutex2);
        //printf("counter:我已经把mutex2解了\n");

    }
}

void *com_thr(void *arg){
    for(int i=0;i<ITEM_COUNT;i++){
        //printf("comsumer:我看看b2是不是空的\n");
        //printf("comsumer:我准备把mutex2锁了\n");
        pthread_mutex_lock(&mutex2);
        //printf("comsumer:我已经把mutex2锁了\n");

        while(buffer_empty(&b2)){
            //printf("comsumer:b2是空的\n");
            pthread_cond_wait(&wait_full_buffer2,&mutex2);
        }
        char c=comsume(&b2);
        printf("comsumer:我消耗了 %d %c\n",i+1,c);
        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex2);
        //printf("comsumer:我已经把mutex2解了\n");
    }
}

int main(){
    srand(time(NULL));
    pthread_t producer,counter,comsumer;
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    pthread_cond_init(&wait_empty_buffer1,NULL);
    pthread_cond_init(&wait_empty_buffer2,NULL);
    pthread_cond_init(&wait_full_buffer1,NULL);
    pthread_cond_init(&wait_full_buffer2,NULL);

    b1.head=b1.tail=b2.head=b2.tail=0;

    pthread_create(&producer,NULL,pro_thr,NULL);
    pthread_create(&counter,NULL,cou_thr,NULL);
    pthread_create(&comsumer,NULL,com_thr,NULL);

    pthread_join(producer,NULL);
    pthread_join(counter,NULL);
    pthread_join(comsumer,NULL);

    printf("程序正常退出\n");
    return 0;

}
