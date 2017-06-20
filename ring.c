#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define N 10

int buf[N];
pthread_t thr[N];
pthread_cond_t buffull[N];
pthread_mutex_t bufmut[N];
int stop=0;

typedef struct{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}sema_t;

sema_t semas[N];

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

void *first(void *arg){
    int *index=(int *)(arg);
    int temp=1;
    while(!stop){
        buf[*index+1]=temp;
        printf("%d号线程向下一个线程发送%d\n",*index,temp);
        //pthread_cond_signal(&buffull[*index+1]);
        sema_signal(&semas[*index+1]);
        sema_wait(&semas[*index]);
        //pthread_cond_wait(&buffull[*index],&bufmut[*index]);
        temp=buf[*index]+1;
    }
}

void *other(void *arg){
    int *index=(int *)(arg);
    int t=(*index==N-1?0:*index+1);
    while(!stop){
        //pthread_cond_wait(&buffull[*index],&bufmut[*index]);
        sema_wait(&semas[*index]);
        buf[t]=buf[*index]+1;
        printf("%d号线程向下一个线程发送%d\n",*index,buf[t]);
        sema_signal(&semas[t]);
        //pthread_cond_signal(&buffull[t]);
    }
}

int main(){
    int zero=0,digit[N];
    for(int i=0;i<N;i++){
        sema_init(&semas[i],0);
    }
    pthread_create(&thr[0],NULL,first,(void *)(&zero));
    for(int i=1;i<N;i++){
        digit[i]=i;
        pthread_create(&thr[i],NULL,other,(void *)(&digit[i]));
    }
    printf(".....\n");
    getchar();
    stop=1;
    for(int i=0;i<N;i++)
        pthread_join(thr[i],NULL);
    return 0;
}
