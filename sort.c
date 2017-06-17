#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define N 1000

typedef struct{
	int *arr;
	int start;
	int end;
}str_arr;

void merge(int *,int *,int,int,int);
void selectionSort(int *,int,int);
void init(int *,int);
void check(int *,int,int);

void *chr(void *arg){
	str_arr *in=(str_arr *)arg;
	selectionSort(in->arr,in->start,in->end);
}

int main(){
	int array[N];
	int temp[N];
	init(array,N);

	str_arr input;
	input.arr=array;
	input.start=0;
	input.end=N/2-1;

	pthread_t pth;
	pthread_create(&pth,NULL,&chr,&input);
	selectionSort(array,N/2,N-1);
	pthread_join(pth,NULL);

	merge(array,temp,0,N/2-1,N-1);

}
