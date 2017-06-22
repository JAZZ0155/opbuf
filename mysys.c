#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void mysys(char input[]){
    //假设参数小于4个

    char **argv=(char **)malloc(5*sizeof(char *));
    char *result=NULL;
    char *p=input;
    int argc=0,k=0;
    char delims=' ';

    result=(char *)malloc(50);
    while(*p!='\0'){
        if(*p!=' '){
            result[k++]=*p;
            p++;
        }else{
            argv[argc++]=result;
			result=(char *)malloc(50);
			k=0;
			p++;
        }
    }
    argv[argc]=NULL;

    int pid;
    pid=fork();
    if(pid==0){
        int succeed=0;
        if(argc==0){
            exit(0);
        }else if(argc==1){
            succeed=execlp(argv[0],argv[0],NULL);
            if(succeed==-1){
                printf("%s",argv[0]);
            }
            exit(0);
        }
        succeed=execvp(argv[0],argv);
        if(succeed==-1){
            printf("command not exist\n");
        }
        exit(0);
    }

    wait(NULL);
}

int main(int argc,char **argv){
    char *input="echo HELLO WORLD";
    printf("------------\n");
    mysys("echo HELLO WORLD");
    printf("------------\n");
    system("ls /");
    mysys("------------\n");
    return 0;
}
