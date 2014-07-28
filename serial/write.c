#define LOGV myprintf
#define LOGI myprintf
#define LOGW myprintf
#define LOGE myprintf

#include <stdio.h>
#include <fcntl.h>
#define myprintf(format,...) \
do { \
  char print_buf[1024]; \
  static int fd_print =-1;\
  snprintf(print_buf,sizeof(print_buf),format,##__VA_ARGS__); \
  if (fd_print < 0) fd_print = open("/dev/ttySAC2",O_RDWR); \
        if (fd_print < 0) fd_print = open("/dev/s3c2410_serial2",O_RDWR); \
  strncat(print_buf,"\n",sizeof(print_buf)); \
  write(fd_print,print_buf,strlen(print_buf)); \
}while(0)
