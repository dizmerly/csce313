// Need for getline
#define  _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const char* INPUT_FILE = "./input_file";
const char* OUTPUT_FILE = "./output_file";

void close_fd(const int fd)
{
        const int retval = close(fd);
        if(retval < 0)
                fprintf(stderr, "Could not close the file. Error code %d\n", errno);
}

int main()
{
        // Open file descriptors
        const int input_fd = open(INPUT_FILE, O_RDONLY);
        if(input_fd < 0)
        {
                fprintf(stderr, "Could not open input file. Exiting...\n");
                return -1;
        }

        const int output_fd = open(OUTPUT_FILE, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
        if(input_fd < 0)
        {
                close_fd(input_fd);
                fprintf(stderr, "Could not open input file. Exiting...\n");
                return -1;
        }

        // Redirect stdin to input_fd
        dup2(input_fd, 0);

        // Redidrect stdout to output_fd
        dup2(output_fd, 1);

        // Read input and generate output
        size_t line_length = 1024;
        char* line = malloc(sizeof(char) * line_length);
        if(!line)
        {
                fprintf(stderr, "Malloc failed. Exiting...\n");
                close_fd(output_fd);
                close_fd(input_fd);
                return -1;
        }

        printf("Generating output...\n");

        ssize_t bytes_read = 0;
        while((bytes_read = getline(&line, &line_length, stdin)) > 0)
                printf("%s", line);

        free(line);

        close_fd(output_fd);
        close_fd(input_fd);
        return 0;
}
