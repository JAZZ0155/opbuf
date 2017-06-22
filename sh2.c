#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>

#define MAX_L 30
#define MAX_P 5

char cmd[MAX_L];
char param[MAX_P][MAX_L];
char *para[MAX_P+1];

int split(char str[]){
    int i=0;
    char *p=strtok(str," ");
    while(p!=NULL){
        strcpy(param[i++],p);
        p=strtok(NULL," ");
    }
    for(int j=0;j<i;j++){
        para[j]=param[j];
    }
    para[i]=NULL;
    return i;
}

void outRedirect(char **pa){
    char f[30];
    int fd;
    strcpy(f,*pa+1);
    *pa=NULL;

    fd=creat(f,444);
    if(fd==-1){
        perror("Create new file fail.\n");
        exit(1);
    }
    dup2(fd,STDOUT_FILENO);
    close(fd);
}

int main(){

    int pid,res,paramsize;

    for(;;){
        fgets(cmd,MAX_L,stdin);
        cmd[strlen(cmd)-1]='\0';
        paramsize=split(cmd);

        if(strcmp(para[0],"exit")==0){
            break;
        }else if(strcmp(para[0],"cd")==0){
            chdir(para[1]);
            continue;
        }

        pid=fork();

        if(pid<0){
            perror("Create process fail!\n");
            exit(1);
        }else if(pid==0){

            if(para[paramsize-1][0]=='>'){
                outRedirect(&para[paramsize-1]);
            }

            res=execvp(para[0],para);
            if(res==-1){
                perror("Execute fail!\n");
                exit(1);
            }
        }else{
            wait(NULL);
        }
    }
}
