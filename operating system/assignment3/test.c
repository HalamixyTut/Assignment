#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#define BUFFER_SIZE 128

struct thread_data
{
    int fd;
};

void *get(void *thread_args)
{
    char buffer[BUFFER_SIZE] = {0};

    struct thread_data *args;
    args = (struct thread_data *) thread_args;
    
    int fd = args->fd;

    while (1)
    {
        while (read(fd, buffer, BUFFER_SIZE) == 0) {}
        printf("%s\n", buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }
}

void *post(void *thread_args)
{    
    char buffer[BUFFER_SIZE] = {0};

    struct thread_data *args;
    args = (struct thread_data *) thread_args;
    int fd = args->fd;

    while (1)
    {
        scanf("%s", buffer);
        // gets(buffer);
        if (!strcmp(buffer, "Bye!"))
            break;
        write(fd, buffer, BUFFER_SIZE);
        memset(buffer, 0, BUFFER_SIZE);
    }
}

int main()
{
    int fd = open("/dev/chatroom", O_RDWR|O_ASYNC);


    pthread_t threads[2];
    struct thread_data args;

    args.fd = fd;
    
    pthread_create(&threads[0], NULL, post, &args);
    pthread_create(&threads[1], NULL, get, &args);
    pthread_join(threads[0], NULL);

    close(fd);

    return 0;
}