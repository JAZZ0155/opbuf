#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv){
	int id,flag,i,count;
	count=0;
	char *p;
	char buf[100];
	char *filename=argv[1];
	id=open(filename,O_RDONLY);
	if(id==-1){
		printf("file open error!\n");
		exit(0);
	}
	while(flag=read(id,&buf,100)!=0){
		for(p=buf,i=0;i<flag;i++,p++){
			putchar(*p);
			if((*p)=='\n')
				if(++count==24)
					getchar();
		}
	}
	return 0;
}