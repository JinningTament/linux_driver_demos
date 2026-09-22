#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf[32] = {0};

    if (argc < 2) {
        printf("Usage: %s <0|1>\n", argv[0]);
        return -1;
    }

    fd = open("/dev/test", O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    buf[0] = argv[1][0];    // 直接取命令行的第一个字符，'1' 或 '0'
    write(fd, buf, 1);      // 只写 1 个字节

    close(fd);
    return 0;
}