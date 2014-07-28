#ifndef __COMMON_KEY_DEF_H
#define __COMMON_KEY_DEF_H

#define REMAIN_PIN_SET          0

#define	IOCTL_WRITE_ADDR    0x378
#define	IOCTL_READ_ADDR    0X379

enum  {
        BUTTON_BACK = 0,
        BUTTON_MENU,
        BUTTON_HOMEPAGE,
        BUTTON_VOLUMEUP,
        BUTTON_VOLUMEDOWN,
        BUTTON_UP,
        BUTTON_DOWN,
        BUTTON_LEFT,
        BUTTON_RIGHT,
        BUTTON_POWER,
        BUTTON_ENTER,
        BUTTONE_MAX
};

typedef struct {
        int num;
        int cmd;
}KeyArg;

#endif
