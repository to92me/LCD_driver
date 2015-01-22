/*
 *  chardev.h − the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */
#ifndef CHARDEV_H
#define CHARDEV_H
#include <linux/ioctl.h>

#define displej_MAJOR 241

#define print _IOR(displej_MAJOR, 0,char *) //maybe void 
#define linija _IOR(displej_MAJOR, 1, char*)
#define krug _IOR(displej_MAJOR, 2, char*)
#define colour _IOR(displej_MAJOR, 3, char* )
#define fillscreen _IOR(displej_MAJOR, 4,char* )
#define picture _IOR(displej_MAJOR, 6 , char* )
#define trougao _IOR(displej_MAJOR, 5, char* )
#define kocka _IOR(displej_MAJOR, 7,char* )

#define naredba2 _IOW(displej_MAJOR, 1, char *)

#define IOCTL_GET_NTH_BYTE _IOWR(displej_MAJOR, 2, int)

#endif

