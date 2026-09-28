#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc,char *argv[]){
    int fd;
    char buf[32]={0};
    char buf2[32]="hello!!!";
    fd=open("/dev/device_test",O_RDWR);
    if(fd<0){
        perror("open");
        return -1;
    }
    printf("open is ok\n");
   sleep(30);
    close(fd);
    printf("close is ok\n");
    return 0;
}