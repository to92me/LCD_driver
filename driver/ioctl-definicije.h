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
/*
 * The major device number. We can't rely on dynamic
 * registration any more, because ioctls need to know
 * it.
 */
#define displej_MAJOR 241
/*
 * Set the message of the device driver
 */
/*
#define funkcije _IOR(displej_MAJOR, 0, signed char *)//IOR-za citanje iz user space
#define slika _IOR(displej_MAJOR, 1, unsigned short *)//IOR-za citanje iz user space
#define string_io _IOR(displej_MAJOR, 2, unsigned short *)
*/

#define print _IOR(displej_MAJOR, 0,char *) //maybe void 
#define linija _IOR(displej_MAJOR, 1, char*)
#define krug _IOR(displej_MAJOR, 2, char*)
#define colour _IOR(displej_MAJOR, 3, char* )
#define fillscreen _IOR(displej_MAJOR, 4,char* )
#define trougao _IOR(displej_MAJOR, 5, char* )
#define kocka _IOR(displej_MAJOR, 7,char* )

/*
 * _IOR means that we're creating an ioctl command
 * number for passing information from a user process
 * to the kernel module.
 *
 * The first arguments, MAJOR_NUM, is the major device
 * number we're using.
 *
 * The second argument is the number of the command
 * (there could be several with different meanings).
 *
 * The third argument is the type we want to get from
 * the process to the kernel.
 */
/*
 * Get the message of the device driver
 */
#define naredba2 _IOW(displej_MAJOR, 1, char *)
/*
 * This IOCTL is used for output, to get the message
 * of the device driver. However, we still need the
 * buffer to place the message in to be input,
 * as it is allocated by the process.
 */
/*
 * Get the n'th byte of the message
 */
#define IOCTL_GET_NTH_BYTE _IOWR(displej_MAJOR, 2, int)
/*
 * The IOCTL is used for both input and output. It
 * receives from the user a number, n, and returns
 * Message[n].
 */
/*
 * The name of the device file
 */
#define DEVICE_FILE_NAME "displej"
#endif

