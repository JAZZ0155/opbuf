#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>

#define MAX_L 100
#define MAX_P 10

char cmd[MAX_L];
char param[MAX_P][MAX_L];
char *para[MAX_P+1];
int subcmd[MAX_P];
int paramsize;
int subcmdnum;

void split(char str[]){

    int scn=0;
    int ps=0;
    subcmd[scn++]=ps;
    char *p=strtok(str," ");
    while(p!=NULL){
        strcpy(param[ps],p);

        if(strcmp(p,"|")==0){
            para[ps]=NULL;
            subcmd[scn++]=ps+1;
        }else{
            para[ps]=param[ps];
        }

        ps++;
        p=strtok(NULL," ");
    }
    para[ps]=NULL;
    subcmdnum=scn;
    paramsize=ps;
}

void restore(){
	for(int i=0;i<subcmdnum;i++){
		for(int j=0;(para+subcmd[i])[j]!=NULL;j++){
			printf("%s ",(para+subcmd[i])[j]);
		}
		if(i!=subcmdnum-1){
			printf("| ");
		}
	}

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

void tes(char *a[]){
    int i=0;
    while(a[i]!=NULL){
        printf("%s ",a[i++]);
    }
    printf("\n");
}

void main(){
    int pid,fstdin,fstdout,res,count;
    int pfd[4][2];

    fstdin=dup(STDIN_FILENO);
	fstdout=dup(STDOUT_FILENO);

    for(;;){//外层循环预处理

		fgets(cmd,MAX_L,stdin);
        cmd[strlen(cmd)-1]='\0';
        split(cmd);
		//restore();

        if(strcmp(para[0],"exit")==0){
            break;
        }else if(strcmp(para[0],"cd")==0){
            chdir(para[1]);
            continue;
        }

        count=0;

        while(count<subcmdnum-1){
			res=pipe(pfd[count]);
			if(res!=0){
        		perror("Create pipe fail!\n");
        		exit(1);
    		}

            pid=fork();
            if(pid<0){
                printf("Create process fail! count=%d\n",count);
                exit(1);
            }else if(pid==0){
                dup2(pfd[count][1],STDOUT_FILENO);
                close(pfd[count][0]);
                close(pfd[count][1]);
                res=execvp(para[subcmd[count]],para+subcmd[count]);
				if(res==-1){
					printf("child:Execute fail! count=%d\n",count);
					exit(1);
				}
                exit(0);
            }else{
        		dup2(pfd[count][0],STDIN_FILENO);
                close(pfd[count][0]);
                close(pfd[count][1]);
				//printf("father:count=%d\n",count);
				count++;
            }
        }

        pid=fork();
        if(pid<0){
            perror("Create process fail!\n");
            exit(1);
        }else if(pid==0){
            if(para[paramsize-1][0]=='>'){
				//printf("%s\n",para[paramsize-1]);
                outRedirect(&para[paramsize-1]);
            }
            res=execvp(para[subcmd[count]],para+subcmd[count]);
            if(res==-1){
                printf("child:Execute fail! count=%d\n",count);
                exit(1);
            }
			exit(0);
        }else{
			//printf("father:count=%d\n",count);
            wait(NULL);
        }
        dup2(fstdin,STDIN_FILENO);
		dup2(fstdout,STDOUT_FILENO);
    }
}
