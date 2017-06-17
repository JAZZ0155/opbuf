#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char **argv){
	int num,b,c;
	int inputfileid,outputfileid;
	char buf[100];
	
	inputfileid=open(argc[1],O_RDONLY);
	if(inputfileid==-1){
		printf("inputfile open error!\n");
		exit(0);
	}
	outputfileid=creat(argv[2],666);
	if(outputfileid==-1){
		printf("outputfile open error!\n");
		exit(0);
	}

	while(num=read(inputfileid,&buf,100)!=0){
		write(outputfileid,&buf,num);
	}

	close(inputfileid);
	close(outputfileid);
}