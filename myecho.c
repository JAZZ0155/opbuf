#include<stdio.h>

int main(int argc, char **argv){
	int i;
	for(i=1;i<argc;i++)
	{
		if(i!=1)
			pritnf(" ");
		printf("%s",argv[i]);
	}
	printf("\n");
}