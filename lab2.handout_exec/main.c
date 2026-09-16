#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
        const char* const path = "ls";
        char* const args[] = {"/usr/bin/ls", "-l", "-a", "-h", ".", NULL};
        const int ret = execvp(path, args);
        if(ret < 0)
        {
                fprintf(stderr, "Could not exec(). Error code is %d. Exiting...\n", errno);
                return -1;
        }
        return 0;
}
