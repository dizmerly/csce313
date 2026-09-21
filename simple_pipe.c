void simple_pipe(char* cmd1, char** argv1, char* cmd2, char** argv2) {
    int pipefd[2], r, status;

    pipe(pipefd);

    pid_t child1 = fork();
    if (child1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(cmd1, argv1);
    }
    assert(child1 > 0);

    close(pipefd[0]);

    pid_t child2 = fork();
    if (child2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(cmd2, argv2);
    }
    assert(child2 > 0);
    
    close(pipefd[0]);
    close(pipefd[1]);
    r = waitpid(child1, &status, 0);
    r = waitpid(child2, &status, 0);
}

// close(pipefd[0]);
// close(pipefd[1]);
// dup2(pipefd[0], STDIN_FILENO);
// dup2(pipefd[0], STDOUT_FILENO);
// dup2(pipefd[1], STDIN_FILENO);
// dup2(pipefd[1], STDOUT_FILENO);
// pipe(pipefd);
// r = waitpid(child1, &status, 0);
// r = waitpid(child2, &status, 0);