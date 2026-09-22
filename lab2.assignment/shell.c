/*
 * Task: Add the shell code in this file. The shell should run `ls -al / | tr a-z A-Z` using UNIX pipes. Please refer to the course webpage for more information
 */


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int producer()
{
        // Added exec call here
        execlp("ls", "ls", "-al", "/", (char *)NULL);
        perror("producer error");
        return -1;
}

int consumer()
{
        // Added exec call here
        execlp("tr", "tr", "a-z", "A-Z", (char *)NULL);
        perror("consumer error");
        return -1;
}

int main()
{
        // Add a call to pipe here
        int fd[2];
        pid_t c;
        pid_t p;
        // catch error of monseuir pipe
        if (pipe(fd) == -1){
                perror("pipe");
                return -1;
        }
        // Fork off the consumer process here. Make sure to handle the file descriptors appropriately
        c = fork();

        if(c < 0){
                perror("consumer failed");
                return -1;
        }
        else if(c == 0){
                // fprintf(stderr, "consumer child\n");
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                return consumer();
        }
        
        else{
                // Fork off the producer process here. Make sure to handle the file descriptors appropriately
                p  = fork();
                if(p < 0){
                // perror("producer failed");
                return -1;
                }
                else if(p == 0){
                        // fprintf(stderr, "producer child\n");
                        close(fd[0]);
                        dup2(fd[1], STDOUT_FILENO);
                        close(fd[1]);
                        return producer();
                }
                else{
                        // fprintf(stderr, "producer parent\n");
                        // Add a call to waitpid for the consumer process here
                        close(fd[0]);
                        close(fd[1]);
                        // Add a call to waitpid for the producer process here  
                        waitpid(c, NULL, 0);
                        waitpid(p, NULL, 0);
                }
        }
        
        // Handle the file descriptors for the shell process, i.e. this process here


        return 0;
}
