//
// Created by ricky on 17-5-14.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 64

/*
 * split a string and set the result to dst
 * dst string array, str splited string, the character for split
 * return the number of string
 *
 * */
int split(char ***dst, char *str, char spliter)
{
    int str_num = 0;
    int each_size;
    int index = 0;
    int str_index = 0;
    int start_index = 0;

    while (str[index] != '\0')
    {
        if (str[index] == spliter)
        {
            index++;
            while(str[index] == spliter)
            {
                index++;
            }

            if (str[index] != '\0'){
                str_num ++;
            }
        }
        else
        {
            index++;
        }
    }
    str_num++;

    *dst = (char **) malloc((str_num + 1)*sizeof(char*));
    index = 0;

    while (str[index] != '\0')
    {
        if (str[index] != spliter)
        {
            start_index = index;
            each_size = 0;

            while (str[index] != spliter && str[index] != '\0')
            {
                index++;
                each_size++;
            }

            (*dst)[str_index] = (char*) malloc((each_size + 1)*sizeof(char));
            int cur_i = 0;

            while (start_index != index)
            {
                (*dst)[str_index][cur_i] = str[start_index];
                start_index++;
                cur_i++;
            }

            (*dst)[str_index][cur_i] = '\0';
            str_index++;
        }
        else
        {
            index++;
        }
    }

    (*dst)[str_num] = NULL;
    return str_num;
}

void freeArray(char ***array, int size)
{
    for (int i = 0; i < size; i++)
    {
        free((*array)[i]);
    }
    free(*array);
}


void outRedirect(char **strs, int *size) {
    char f[30];
    int fd;
    char *str = strs[*size - 1];

    if (str[0] == '>') {
        strcpy(f, str + 1);
        strs[*size - 1] = NULL;
        *size = *size - 1;

        fd = open(f, O_CREAT|O_TRUNC, S_IRWXG);
        if (fd == -1) {
            perror("Create new file fail.\n");
            exit(1);
        }
        dup2(fd, stdout);
        close(fd);
    }
}

int inCommand(char **strs) {

    if (strcmp(strs[0], "exit") == 0) {
        return 1;
    } else if (strcmp(strs[0], "cd") == 0) {
        chdir(strs[1]);
        return 0;
    }

    return -1;
}

int main(int argc, char *argv[]) {
    int size;
    char cmd[MAX_COMMAND_LENGTH];
    char **strs;
    pid_t pid;
    int ret;

    for (; ;) {
        fgets(cmd, MAX_COMMAND_LENGTH, stdin);

        size = strlen(cmd);
        cmd[size - 1] = '\0';

        size = split(&strs, cmd, ' ');

        ret = inCommand(strs);
        if (ret == 1) {
            break;
        } else if (ret == 0) {
            continue;
        }

        pid = fork();

        if (pid < 0) {
            perror("Create process fail!\n");
            exit(1);
        }
        else if (pid == 0) {

            outRedirect(strs, &size);

            ret = execvp(strs[0], strs);
            if (ret == -1) {
                perror("Execute fail!\n");
                exit(1);
            }
        }
        else {
            wait(NULL);
            freeArray(&strs, size);
        }
    }

    return 0;
}