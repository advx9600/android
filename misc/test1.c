#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "common_key_def.h"

int main(int argc,char* argv[])
{
	if (argc !=4){
		printf("argc err!\n");
		printf("devicename status cmd\n");
		return -1;
	}

	const char* dev=argv[1];
	const unsigned  int status = atoi(argv[2]);
	const int cmd = atoi(argv[3]);
	KeyArg keyarg;

	int fd = open(dev,O_RDWR);
	if (fd < 0){
		perror("fopen");
		return -1;
	}

	keyarg.num=status;
	keyarg.cmd=1;
	if (ioctl(fd,IOCTL_WRITE_ADDR,&keyarg)){
		perror("ioctl");
	}
	keyarg.num=status;
	keyarg.cmd=0;
	if (ioctl(fd,IOCTL_WRITE_ADDR,&keyarg)){
		perror("ioctl");
	}
	
	close(fd);
	return 0;
}
