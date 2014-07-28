#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#define I2C_RETRIES 0x0701
#define I2C_TIMEOUT 0x0702
#define I2C_RDWR 0x0707

#define LOGE printf
#define LOGI printf
/*********定义struct i2c_rdwr_ioctl_data和struct i2c_msg，要和内核一致*******/

struct i2c_msg {
	unsigned short addr;
	unsigned short flags;
#define I2C_M_TEN 0x0010
#define I2C_M_RD 0x0001
	unsigned short len;
	unsigned char *buf;
};
struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;
	int nmsgs;
/* nmsgs这个数量决定了有多少开始信号，对于“单开始时序”，取1*/
};
static int write_iic_data(int fd, int dev_addr, int addr, int val) {
	int ret;
	struct i2c_rdwr_ioctl_data e2prom_data;
	e2prom_data.nmsgs = 1;
	e2prom_data.msgs = (struct i2c_msg*) malloc(
			e2prom_data.nmsgs * sizeof(struct i2c_msg));
	if (!e2prom_data.msgs) {
		LOGE("iic malloc err");
		return -1;
	}
	(e2prom_data.msgs[0]).buf = (unsigned char*) malloc(7);

	e2prom_data.nmsgs = 1;
	(e2prom_data.msgs[0]).len = 7;
	(e2prom_data.msgs[0]).addr = dev_addr;
	(e2prom_data.msgs[0]).flags = 0;
	(e2prom_data.msgs[0]).buf[0] = 0xfc;
	(e2prom_data.msgs[0]).buf[1] = 0xf3;
	(e2prom_data.msgs[0]).buf[2] = 0x3B;
	(e2prom_data.msgs[0]).buf[3] = ((addr >> 8) & 0xff);
	(e2prom_data.msgs[0]).buf[4] = (addr & 0xff);
	(e2prom_data.msgs[0]).buf[5] = ((val >> 8) & 0xff);
	(e2prom_data.msgs[0]).buf[6] = (val & 0xff);
	ret = ioctl(fd, I2C_RDWR, (unsigned long) &e2prom_data);
	if (ret < 0) {
		perror("ioctl");
		LOGE("ioctl write failed!");
		return -1;
	}

	free((e2prom_data.msgs[0]).buf);
	free(e2prom_data.msgs);
}
static int read_iic_data(int fd, int dev_addr, int addr) {
	int ret, high_val, low_val;
	struct i2c_rdwr_ioctl_data e2prom_data;
	e2prom_data.nmsgs = 2; // at least 2,need to add malloc and free
	e2prom_data.msgs = (struct i2c_msg*) malloc(
			e2prom_data.nmsgs * sizeof(struct i2c_msg));
	if (!e2prom_data.msgs) {
		LOGE("iic malloc err");
		return -1;
	}
	(e2prom_data.msgs[0]).buf = (unsigned char*) malloc(7);
	(e2prom_data.msgs[1]).buf = (unsigned char*) malloc(7);

	e2prom_data.nmsgs = 1;
	(e2prom_data.msgs[0]).len = 5;
	(e2prom_data.msgs[0]).addr = dev_addr;
	(e2prom_data.msgs[0]).buf[0] = 0xfc;
	(e2prom_data.msgs[0]).buf[1] = 0xf3;
	(e2prom_data.msgs[0]).buf[2] = 0x37;
	(e2prom_data.msgs[0]).buf[3] = ((addr >> 8) & 0xff);
	(e2prom_data.msgs[0]).buf[4] = (addr & 0xff);
	ret = ioctl(fd, I2C_RDWR, (unsigned long) &e2prom_data);
	if (ret < 0) {
		perror("ioctl cmd 0x37");
		LOGE("ioctl cmd 0x37 failed!");
		return -1;
	}

	e2prom_data.nmsgs = 2;
	(e2prom_data.msgs[0]).len = 4;
	(e2prom_data.msgs[0]).addr = dev_addr;
	(e2prom_data.msgs[0]).flags = 0;
	(e2prom_data.msgs[0]).buf[0] = 0xfc;
	(e2prom_data.msgs[0]).buf[1] = 0xf3;
	(e2prom_data.msgs[0]).buf[2] = 0x60;
	(e2prom_data.msgs[0]).buf[3] = 0x26;

	(e2prom_data.msgs[1]).len = 1;
	(e2prom_data.msgs[1]).addr = dev_addr;
	(e2prom_data.msgs[1]).flags = I2C_M_RD;
	(e2prom_data.msgs[1]).buf[0] = 0x00;
	ret = ioctl(fd, I2C_RDWR, (unsigned long) &e2prom_data);
	if (ret < 0) {
		perror("ioctl read step 1");
		LOGE("ioctl read step 1 failed!");
		return -1;
	}
	high_val = (((e2prom_data.msgs[1]).buf[0] << 8) & 0xff00);

	e2prom_data.nmsgs = 2;
	(e2prom_data.msgs[0]).len = 4;
	(e2prom_data.msgs[0]).addr = dev_addr;
	(e2prom_data.msgs[0]).flags = 0;
	(e2prom_data.msgs[0]).buf[0] = 0xfc;
	(e2prom_data.msgs[0]).buf[1] = 0xf3;
	(e2prom_data.msgs[0]).buf[2] = 0x60;
	(e2prom_data.msgs[0]).buf[3] = 0x25;

	(e2prom_data.msgs[1]).len = 1;
	(e2prom_data.msgs[1]).addr = dev_addr;
	(e2prom_data.msgs[1]).flags = I2C_M_RD;
	(e2prom_data.msgs[1]).buf[0] = 0x00;
	ret = ioctl(fd, I2C_RDWR, (unsigned long) &e2prom_data);
	if (ret < 0) {
		perror("ioctl read step 2");
		LOGE("ioctl read step 2 failed!");
		return -1;
	}
	low_val = ((e2prom_data.msgs[1]).buf[0] & 0xff);

	free((e2prom_data.msgs[0]).buf);
	free((e2prom_data.msgs[1]).buf);
	free(e2prom_data.msgs);
	return high_val + low_val;
}
int main() {
#define I2C_1E3A_ERR -2
	const char* DEV = "/dev/i2c/1";
	const int loopTime = 1;
	int fd, i, j, ret;
//	LOGI("boot test");
//	system("reboot");

	int addr_data[] = { 0x1E30, 0x0231, 0x1E34, 0x006B, 0x1E3D, 0x0100, 0x1E3E,
			0x0101, 0x1E41, 0x0101, 0x1E44, 0x0081, 0x1E45, 0x03CF, 0x1E46,
			0x0011, 0x1E47, 0x3000, 0x1E48, 0x1000, 0x1E49, 0x0880, 0x1E4D,
			0x0350, 0x1E52, 0x0013, 0x1E58, 0x000C, 0x1E60, 0x0001, 0x1E63,
			0x0003, 0x1E70, 0x05C0, 0x1E86, 0x000B, 0x1E87, 0x0009, 0x1E88,
			0x3800, 0x1E89, 0x0001, 0x1E8B, 0x0080, 0x1E8C, 0x0010, 0x1E92,
			0x7800, 0x1EA0, 0x4000, 0x1EA1, 0x3300, 0x1EA2, 0x3200, 0x1EBC,
			0x6800, 0x1EBD, 0x0100, 0x1EBF, 0x7000, 0x1EC0, 0x2680, 0x1EC1,
			0x1080, 0x1EC5, 0x2B06, 0x1EC6, 0x0C1F, 0x1EC8, 0x2879, 0x1EC9,
			0x65AB, 0x1ECA, 0x4026, 0x1ECB, 0x7FFF, 0x1ECC, 0x7FFE, 0x1EF8,
			0x0400, 0x1EF9, 0x0100, 0x1EFF, 0x4B00, 0x1F00, 0x7FFF, 0x1F0A,
			0x0A00, 0x1F0C, 0x0100, 0x1F0D, 0x7800, 0x1E3A, 0x0000 };
	fd = open(DEV, O_RDWR);
	if (fd < 0) {
		LOGE("open %s failed!", DEV);
		return -1;
	}
	if (ioctl(fd, I2C_TIMEOUT, 1) || ioctl(fd, I2C_RETRIES, 2)) {
		LOGE("ioctl I2C_TIMEOUT or I2C_RETRIES failed!");
		return -1;
	}

	ioctl(fd, I2C_TIMEOUT, 1);/* 超时时间*/
	ioctl(fd, I2C_RETRIES, 2);/*重复次数*/
	/***write data to e2prom**/
#define ARRAYSIZE(a) sizeof(a)/sizeof(a[0])
	int dev_addr = (0xc0 >> 1);
	for (i = 0; i < ARRAYSIZE(addr_data); i = i + 2) {
			if (write_iic_data(fd, dev_addr, addr_data[i], addr_data[i + 1])){
				printf("write 0x%x:0x%04x success!\n",addr_data[i],addr_data[i+1]);
			}else{
				printf("write 0x%x:0x%04x failed!\n",addr_data[i],addr_data[i+1]);
			}
	}
	close(fd);
	return 0;
}
