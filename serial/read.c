#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static int read_serial_data()
{
        int sd=open("/dev/ttySAC2",O_RDWR);
        char buf[1024];
        read(sd,buf,sizeof(buf));
        close(sd);
        return 0;
}

