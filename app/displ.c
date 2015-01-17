/*
 *  ioctl.c - the process to use ioctl's to control the kernel module

 */

#include "ioctl-definicije.h"// isti h fajl mora biti i u device kodu i u program kodu
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#include "myPic.h" // slika predstavljena kao jednodimenzionalni niz
#define DEVICE "/dev/displej"
#define  NAREDBE_BUFFER   6



/*
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
*/

char* program_name;
int func_option;
int brkic_option;
char * string_one;
int index_tmp;
char* next;
int index_count;
char *login[50];
int tmp;

int main( int argc, char **argv){

    int next_option;
    const char* const short_options = "f:s:b:h";

    const struct option long_options[] = {
    {"function", required_argument, NULL, 'f'},
    {"sentece", required_argument, NULL, 's'},
    {"brkic_options", no_argument, NULL, 'b'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL , 0}
};
    program_name = argv[0];

    do{
        next_option = getopt_long ( argc, argv, short_options, long_options, NULL );

        switch (next_option) {
        case 'f':{
            tmp = atoi (optarg);
            printf("F option and argument is: %d \n", tmp );
            // TODO
            break;
        }

        case 's':{
            index_tmp = optind-1;
            while(index_tmp < argc){
                next = argv[index_tmp];
                index_tmp++;
                if(next[0] != '-'){
                    if (index_count < 10){
                        login[index_count++] = next;
                    }
                    printf("argument is : %s \n", next);
                }
                else break;
            }
            optind = index_tmp - 1;
            break;
        }

        case 'b':{
            tmp = atoi(optarg);
            printf("brkic options is selected :P argv is : %d\n", tmp );
            //TODO
            break;
        }

        case 'h':{
            /*
             * TODO
             * PrintHelp();
            */
            break;
        }

        case -1:{

            break;
        }
        default:
            abort();
            break;
        }

    }while(next_option != -1);
    return 0;
}




