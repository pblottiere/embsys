#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd = shm_open("SHM", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if (fd == -1)
    {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    float * glass;
    if (ftruncate(fd, sizeof(float)) != 0)
    {
        perror("ftruncate");
        return EXIT_FAILURE;
    }

    glass = mmap(NULL, sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (glass == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }

    while(1)
    {
        if ((* glass) < 1)
            (* glass) = (* glass) + 0.1;
        sleep(1);
    }

    return EXIT_SUCCESS;
}
