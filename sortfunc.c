#include<stdlib.h>
#include<time.h>
#include<stdio.h>

//从小到大

void check(int *arr,int start,int end){
    int right=1;
    for(int i=start;i<end;i++){
        if(arr[i]>arr[i+1]){
            printf("arr[%d]=%d,arr[%d]=%d\n",i,arr[i],i+1,arr[i+1]);
            right=0;
            break;
        }
    }
    if(right){
        printf("right\n");
    }
}

void selectionSort(int *arr,int start,int end){
    printf("start selectionSort from %d to %d\n",start,end);
	int min,i,j,temp;
	for(i=start;i<end;i++){
		min=i;
		for(j=i+1;j<=end;j++){
			if(arr[min]>arr[j])
			    min=j;
		}
		if(min!=i){
			temp=arr[i];
			arr[i]=arr[min];
			arr[min]=temp;
		}
	}
    check(arr,start,end);
    printf("%d-%d completed\n",start,end);
}

void merge(int *arr,int *temp,int start,int mid,int end){
    printf("start merge from %d to %d\n",start,end);
	int i=start,j=mid+1,k=start;
	while(i!=mid+1&&j!=end+1){
		if(arr[i]>arr[j])
			temp[k++]=arr[j++];
		else
			temp[k++]=arr[i++];
	}
	while(i!=mid+1)
		temp[k++]=arr[i++];
	while(j!=end+1)
		temp[k++]=arr[j++];
	for(i=start;i<=end;i++)
		arr[i]=temp[i];
    check(arr,start,end);
    printf("%d-%d completed\n",start,end);
}

void init(int *arr,int n){
    srand(time(NULL));
    for(int i=0;i<n;i++){
        arr[i]=random();
    }
}
