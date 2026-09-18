#include<stdio.h>
#include<sys/syscall.h>
#include<fcntl.h>
#include<unistd.h>

#define finit_module(fd,uargs,flags) syscall(__NR_finit_module,fd,uargs,flags) //用到finit_module的宏定义

int main(int argc,char **argv){
    int fd;
    int ret;
    fd=open(argv[1],O_RDONLY|O_CLOEXEC);
    if(fd<0){
        printf("open error\n");
        return -1;
    }
    ret=finit_module(fd,"",0);
    return ret;
}
