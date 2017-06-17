#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char **argv){
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
//    char **p=&argv[1];
    succeed=execvp(argv[0],argv);
    if(succeed==-1){
        printf("command not exist\n");
    }
    return 0;
}
