#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdint.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include <linux/fb.h>
#include <string.h>




// /home/disk2/PP1938-V2.4.1/customer_release# grep I2C_TENBIT . -r
// ./compile_scripts/linux-2.6.29-maximus/include/linux/i2c-dev.h
#define I2C_RETRIES    0x0701    /* number of times a device address should
be polled when not acknowledging */
#define I2C_TIMEOUT    0x0702    /* set timeout in units of 10 ms */

/* NOTE: Slave address is 7 or 10 bits, but 10-bit addresses
* are NOT supported! (due to code brokenness)
*/
#define I2C_SLAVE    0x0703    /* Use this slave address */
#define I2C_SLAVE_FORCE    0x0706    /* Use this slave address, even if it
is already in use by a driver! */
#define I2C_TENBIT    0x0704    /* 0 for 7 bit addrs, != 0 for 10 bit */

#define I2C_FUNCS    0x0705    /* Get the adapter functionality mask */

#define I2C_RDWR    0x0707    /* Combined R/W transfer (one STOP only) */

#define I2C_PEC        0x0708    /* != 0 to use PEC with SMBus */
#define I2C_SMBUS    0x0720    /* SMBus transfer */




//#define IC_ADDRESS (0x42>>1)
// OV3640 
#define IC_ADDRESS (0x78>>1)

#define RW_DELAY 10
#define I2C_OV3640 "/dev/i2c/1"
int i2c_fd;




// 函数名称：i2c_init
// 函数功能：打开I2C
// 参数列表：
// 返回值  ：
int i2c_init(char *fn)
{
    i2c_fd = open(fn, O_RDWR);

  //  printf("i2c_fd = %d\n", i2c_fd);

    if (i2c_fd < 0) 
    {
        printf("I2CLIB: error open device\n");

        return 0;
    }

    return i2c_fd;
}




// 函数名称：i2c_close
// 函数功能：关闭I2C
// 参数列表：
// 返回值  ：
int i2c_close(void)
{
    close(i2c_fd);    

    return 0;
}




// 0x3005
// ./test7725 r 12293
// 0x40=64
// ./test7725 w 12293 13
// 函数名称：i2c_write
// 函数功能：往I2C寄存器中读一个字节
// 参数列表：
// 返回值  ：
// void i2c_write(unsigned char addr,unsigned char value)
static int i2c_write(char  registers[])
{
    unsigned char temp[3]; 
		int addr = ((registers[0]<<8)&0xff00)+(registers[1]&0xff);
		char value = registers[2];

    temp[0] = addr >> 8;
    temp[1] = addr & 0xff;
    // temp[1] = value;
    temp[2] = value;

    // if (ioctl(i2c_fd, I2C_SLAVE, IC_ADDRESS) < 0) 
    if (ioctl(i2c_fd, I2C_SLAVE_FORCE, IC_ADDRESS) < 0) 
    {
				perror("ioctl");
        printf("I2CLIB_W: error set IC_ADDRESS\n");
				return -1;
    }
    
    // if(write(i2c_fd, temp, 2)!=2)
    if(write(i2c_fd, temp, 3)!=3)
    {
				perror("write");
        printf("I2C write failure???? addr=%04d,0x%04x\n", addr, addr);
				return -1;
    }
		return 0;
    
    
		#if 0
    // 写入之后再读出来。
    usleep(RW_DELAY*1000);

    // if(write(i2c_fd, temp, 2)!=1)
    if(write(i2c_fd, temp, 2)<0)
    {    
        // printf("TVIN:I2C Write Addr %x fail.\n",temp[0]);
        printf("TVIN:I2C Write Addr %x fail.\n", addr);
    }

    usleep(RW_DELAY*1000);

    unsigned char temp2 = 0;
    
    // if(read(i2c_fd, temp, 1)!=1)
    // if(read(i2c_fd, temp, 2)<0)
    if(read(i2c_fd, &temp2, 1)<0)
    {    
        // printf("TVIN:I2C Read Data[%d]=%x fail.\n",temp[0],temp[0]);
        printf("TVIN:I2C Read Register Address [%04x] Data fail.\n", addr);
    }
    
    printf("temp2=%03d, 0x%04x \n", temp2, temp2);

    usleep(RW_DELAY*1000);
		#endif
}




// 函数名称：i2c_read 
// 函数功能：从I2C寄存器中读一个字节
// 参数列表：
// 返回值  ：
int i2c_read(int start_add)
{
    unsigned char temp[2];

    temp[0] = start_add >> 8;
    temp[1] = start_add & 0xff;

    // printf("temp[0]=%x, temp[1]=%x\n", temp[0], temp[1]);
    // printf("address=0x%02x%02x\n", temp[0], temp[1]);

    //DBG_TVP("Running i2c_read_register().\n");

    // if (ioctl(i2c_fd, I2C_SLAVE, IC_ADDRESS) < 0)
    if (ioctl(i2c_fd, I2C_SLAVE_FORCE, IC_ADDRESS) < 0) 
    {
        printf("I2CLIB_R1: error set IC_ADDRESS =0x%04x \n", IC_ADDRESS);
        return -1;
    }

    if (ioctl(i2c_fd, I2C_RETRIES, 1) < 0) 
    {
        printf("I2CLIB_R2: error set IC_ADDRESS\n");
        return -1;
    }

    usleep(RW_DELAY*1000);

    // if(write(i2c_fd, temp, 2)!=1)
    if(write(i2c_fd, temp, 2)<0)
    {    
        // printf("TVIN:I2C Write Addr %x fail.\n",temp[0]);
        printf("TVIN:I2C Write Addr %x fail.\n",start_add);
    }

    usleep(RW_DELAY*1000);

    // if(read(i2c_fd, temp, 1)!=1)
    // if(read(i2c_fd, temp, 2)<0)
    if(read(i2c_fd, temp, 1)<0)
    {    
        // printf("TVIN:I2C Read Data[%d]=%x fail.\n",temp[0],temp[0]);
        printf("TVIN:I2C Read Register Address [%04x] Data fail.\n", start_add);
    }

    usleep(RW_DELAY*1000);

    return temp[0];
}





// 函数名称：main
// 函数功能：
// 参数列表：
// 返回值  ：
#include <stdlib.h>
#include "ov3640.h"
#include "ov3640_firmware.c"

int main(int argc, char **argv)
{

	int i=0;
	int wrongTimes=0;
	char* write_reg;
	int val_count=0;
#define OV3640_INIT 0
#define OV3640_AFC 1

	int cur_pos=0;
	val_count = 0;

#if OV3640_INIT
	val_count += sizeof(ov3640_init_reg); 
#endif

#if OV3640_AFC
	val_count += sizeof(ov3640_afc_init_reg);
#endif

	write_reg=(char*)malloc(val_count);

#if OV3640_INIT
	memcpy(write_reg+cur_pos,ov3640_init_reg,sizeof(ov3640_init_reg));
	cur_pos += sizeof(ov3640_init_reg);
#endif

#if OV3640_AFC
	memcpy(write_reg+cur_pos,\
			ov3640_afc_init_reg,sizeof(ov3640_afc_init_reg)),
	cur_pos += sizeof(ov3640_afc_init_reg);
#endif

	i2c_init(I2C_OV3640);
	for (i=0;i<val_count/3;i++)
	{
		if (i2c_write(&write_reg[i*3])){
			if (wrongTimes >3){
				printf("i2c_write failed!\n\n\n");
				break;
			}else{
				usleep(10);
				wrongTimes++;
				i--;
			}
		}else{
			wrongTimes =0;
		}
		usleep(10);
	}

	i2c_close();
	free(write_reg);
	return 0;
}
