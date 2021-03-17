#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main() {
    char * args[] = {"./child", NULL};
    

    int pid = fork();

    if(pid == 0){
        printf("Child program running.\n");
        execvp(args[0], args);
    }else{
        printf("Parent:\n");
    }

   

    
    return 0;
}