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

void split(char str[]){
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
}

int main(){

    int pid,res;

    for(;;){
        fgets(cmd,MAX_L,stdin);
        //scanf("%s",cmd);
        cmd[strlen(cmd)-1]='\0';
        split(cmd);

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
