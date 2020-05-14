#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>  //ps -eaf

int main()
{
    int i;
    int pid = fork();
    if (pid==0)
    {
        for (i=0; i<10; i++)
            printf("Child\n");
    }
    else
    {
       // wait(0);
        printf("Parent\n");
        while(1);
    }
}

