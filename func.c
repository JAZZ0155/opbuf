#include<stdlib.h>

float element(int n){
    float result;
    if(n%2==1){
        result=1.0/(2*n-1);
    }else{
        result=-1.0/(2*n-1);
    }
//    printf("n=%d,result=%f\n",n,result);
    return result;
}

float cacu(int first,int last){
    float sum=0;
    for(int i=first;i<=last;i++){
        sum+=element(i);
    }
//    printf("first=%d,last=%d,sum=%f\n",first,last,sum);
    return sum;
}

void *thr(void *arg){
    int *arr=(int *)arg;
    float *result=malloc(sizeof(float));
    *result=cacu(arr[0]+1,arr[1]);
    return result;
}

