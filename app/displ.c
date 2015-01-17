/*
 *  ioctl.c - the process to use ioctl's to control the kernel module

 */

#include "ioctl-definicije.h"// isti h fajl mora biti i u device kodu i u program kodu

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#include "myPic.h" // slika predstavljena kao jednodimenzionalni niz
#define DEVICE "/dev/displej"
#define  NAREDBE_BUFFER   6


int main(int argc, char **argv)
{
int i;
signed char naredbe[NAREDBE_BUFFER];
int file_desc, ret_val;


   for (i = 1; i < NAREDBE_BUFFER; i++)
        naredbe[i-1] = (int)strtol(argv[i], NULL, 0);//strtol(argv[1]-tekst koji prebacujemo, NULL-da ne trazimo kraj brojki, 0-da bude double int);
       
        printf("start displ \n");

	file_desc = open(DEVICE, 0); //pokrecemo drajver
   if (file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE);
		exit(-1);
	                   }   
   if (naredbe[0]== 1) 
      {
           ret_val=ioctl(file_desc, funkcije,naredbe);//"file_desc" povezuje iocstl naredbu sa nasim drajverom,"funkcije" su definisane u ioctl-definicije.h,"naredbe" je podatak koji saljemo drajveru

               if (ret_val < 0) {
		                  printf("ioctl_set_boje failed:%d\n", ret_val);
		                  exit(-1);
	                         }
     }
    if (naredbe[0]== 2) 
    {
              ret_val=ioctl(file_desc, slika, _myPic);//ovde prosledjujemo sliku kao niz podataka,definisanu u myPic.h
             if (ret_val < 0){
	                   	printf("ioctl_set_sliku failed:%d\n", ret_val);
	                     	exit(-1);
	                     }
     }         


	close(file_desc);
   return 0;
}

