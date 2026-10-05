#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        close(fd[1]);

        char buf[2] = {0};
        ssize_t n = read(fd[0], buf, 1);
        printf("child: n=%zd, buf=%s\n", n, buf);

        n = read(fd[0], buf, 1);
        printf("child: second read=%zd\n", n);
        _exit(0);
    }

    close(fd[0]);
    write(fd[1], "A", 1);
    close(fd[1]);
    wait(NULL);
}