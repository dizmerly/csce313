#include <stdint.h>
#include <stdio.h>
#include <threading.h>

void run_range(int32_t start, int32_t end)
{
        for (int32_t value = start; value < end; value++)
        {
                printf("Worker [%d, %d): item %d\n", start, end, value);
                t_yield();
        }

        printf("Worker [%d, %d): finished\n", start, end);
        t_finish();
}

int main(void)
{
        t_init();

        if (t_create(run_range, 1, 6) != 0) // Spawn a worker context
        {
                fprintf(stderr, "Could not create worker 1!\n");
                return -1;
        }

        if (t_create(run_range, 6, 11) != 0) // Spawn a worker context
        {
                fprintf(stderr, "Could not create worker 2!\n");
                return -1;
        }

        if (t_create(run_range, 11, 16) != 0) // Spawn a worker context
        {
                fprintf(stderr, "Could not create worker 3!\n");
                return -1;
        }

        if (t_create(run_range, 16, 21) != 0) // Spawn a worker context
        {
                fprintf(stderr, "Could not create worker 4!\n");
                return -1;
        }

        while (t_yield() >= 1)
        {
                // Keep yielding until only the main context remains.
        }

        return 0;
}
