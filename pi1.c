#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define N 50

float element(int n);
float cacu(int,int);
void *thr(void *);

int main(){
    pthread_t pth;
    int arg[]={N/2,N};
    float sum,sum1,*sum2;
    int fail=pthread_create(&pth,NULL,&thr,arg);
    if(fail){
        printf("fail to create thread\n");
        exit(0);
    }
    sum1=cacu(1,N/2);
    pthread_join(pth,(void **)&sum2);
    sum=sum1+(*sum2);
    printf("sum=%f,sum1=%f,sum2=%f\n",sum,sum1,*sum2);
}
