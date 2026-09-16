#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int parent(pid_t child_pid)
{
        printf("[PID: %d] Hi! I'm the parent. My child's PID is %d\n", getpid(), child_pid);
        const pid_t terminated_child = waitpid(child_pid, NULL, 0);
        if(terminated_child < 0)
        {
                fprintf(stderr, "waitpid() failed. Error code is %d. Exiting...\n", errno);
                return -1;
        }
        return 0;
}

int child()
{
        printf("[PID: %d] Hi! I'm the child\n", getpid());
        return 0;
}

int main()
{
        const pid_t child_pid = fork();
        if(child_pid == 0)
                return child();
        else if(child_pid > 0)
                return parent(child_pid);
        else
        {
                fprintf(stderr, "fork() failed.i Error code is %d. Exiting...\n", errno);
                return -1;
        }
        return 0;
}
