#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

#define CAP 4
#define TABLE_SIZE 8
#define ITEM_COUNT 1000

char table[]={'a','b','c','d','e','f','g','h'};

typedef struct{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}sema_t;

void sema_init(sema_t *sema,int value){
    sema->value=value;
    pthread_mutex_init(&sema->mutex,NULL);
    pthread_cond_init(&sema->cond,NULL);
}

void sema_wait(sema_t *sema){
    pthread_mutex_lock(&sema->mutex);
    while(sema->value<=0)
        pthread_cond_wait(&sema->cond,&sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema){
    pthread_mutex_lock(&sema->mutex);
    sema->value++;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_b1,mutex_b2;
sema_t empty_b1,full_b1;
sema_t empty_b2,full_b2;

typedef struct{
    char buf[CAP+1];
    int head;
    int tail;
}buffer;

buffer b1,b2;

void produce(buffer *b,char c){
    b->buf[b->head]=c;
    b->head=(b->head+1)%(CAP+1);
    printf("tail=%d,head=%d\n",b->tail,b->head);
}

char comsume(buffer *b){
    char c=b->buf[b->tail];
    b->tail=(b->tail+1)%(CAP+1);
    printf("tail=%d,head=%d\n",b->tail,b->head);
    return c;
}

void *pro_thr(void *arg){
    for(int i=0;i<ITEM_COUNT;i++)
    {
        char c=table[rand()%TABLE_SIZE];

        sema_wait(&empty_b1);
        sema_wait(&mutex_b1);
        produce(&b1,c);
        printf("producer:我生产了 %d %c\n",i+1,c);
        sema_signal(&full_b1);
        sema_signal(&mutex_b1);
    }
}

void *cou_thr(void *arg){
    for(int i=0;i<ITEM_COUNT;i++){
        sema_wait(&full_b1);
        sema_wait(&mutex_b1);
        char c=comsume(&b1);
        sema_signal(&empty_b1);
        sema_signal(&mutex_b1);

        c=c-32;

        sema_wait(&empty_b2);
        sema_wait(&mutex_b2);
        produce(&b2,c);
        printf("counter:我转换了 %d %c\n",i+1,c);
        sema_signal(&full_b2);
        sema_signal(&mutex_b2);
    }
}

void *com_thr(void *arg){
    for(int i=0;i<ITEM_COUNT;i++){
        sema_wait(&full_b2);
        sema_wait(&mutex_b2);
        char c=comsume(&b2);
        printf("comsumer:我消耗了 %d %c\n",i+1,c);
        sema_signal(&empty_b2);
        sema_signal(&mutex_b2);
    }
}

int main(){
    srand(time(NULL));
    pthread_t producer,counter,comsumer;
    sema_init(&mutex_b1,1);
    sema_init(&mutex_b2,1);
    sema_init(&empty_b1,4);
    sema_init(&empty_b2,4);
    sema_init(&full_b1,0);
    sema_init(&full_b2,0);

    b1.head=b1.tail=b2.head=b2.tail=0;

    pthread_create(&producer,NULL,pro_thr,NULL);
    pthread_create(&counter,NULL,cou_thr,NULL);
    pthread_create(&comsumer,NULL,com_thr,NULL);

    pthread_join(comsumer,NULL);

    printf("程序正常退出\n");
    return 0;

}
