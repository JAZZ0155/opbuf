#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000
#define CPU_N 10

float element(int);
float cacu(int,int);
void *thr(void *);

int main(){
    pthread_t CPU[CPU_N];
    int arg[CPU_N];
    float sum=0;

    for(int i=0;i<CPU_N;i++){
        arg[i]=i*N/CPU_N;
    }

    for(int i=0;i<CPU_N;i++){
        int fail=pthread_create(&CPU[i],NULL,&thr,&arg[i]);
        if(fail){
            printf("fail to create thread\n");
            exit(0);
        }
    }

    for(int i=0;i<CPU_N;i++){
        float *result;
        pthread_join(CPU[i],(void **)&result);
        printf("CPU[%d] %f\n",i+1,*result);
        sum=sum+(*result);
    }

    printf("sum=%f\n",sum);

}
