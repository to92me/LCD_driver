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

#define funkcije _IOR(displej_MAJOR, 0, signed char *)//IOR-za citanje iz drajvera
#define slika _IOR(displej_MAJOR, 1, unsigned short *)//IOR-za citanje iz drajvera

#define naredba2 _IOW(displej_MAJOR, 1, char *)

#define IOCTL_GET_NTH_BYTE _IOWR(displej_MAJOR, 2, int)

#endif

