#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pipefds[2];
    pid_t pid;
    char buf[30];

    // create pipe
    if (pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    memset(buf, 0, 30);
    pid = fork();

    if(pid>0){
        printf("PARENT: writing to the pipe\n");
        close(pipefds[0]);
        write(pipefds[1], "CSCE313", 30);
        close(pipefds[1]);
        wait(NULL);
    }
    else{
        while(read(pipefds[0], buf, 1) == 1) {
            printf("CHILD read from pipe -- %s\n", buf);
        }
        close(pipefds[0]);
        
        printf("CHILD EXITING");
        exit(EXIT_SUCCESS);
    }
    return 0; 
}