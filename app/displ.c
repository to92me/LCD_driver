/*
 *  ioctl.c - the process to use ioctl's to control the kernel module

 */

//#include "ioctl-definicije.h"// isti h fajl mora biti i u device kodu i u program kodu
#include <getopt.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
//#include "myPic.h" // slika predstavljena kao jednodimenzionalni niz
#define DEVICE "/dev/displej"
#define  NAREDBE_BUFFER   6
//#define LPC -- use for home compiling and testing without LPC borad

int index_tmp;
char* next;
int index_count;
char *login[50];
int login_int[15];
int tmp;

int file_desc;
int ret_val;
char* word;
char* word_space;  
char* word_terminator; 
int i;

int main( int argc, char **argv){

    int next_option, option_count;
    char* optarg_tmp;
    const char* const short_options = "f:s:p:hl:k:";

    const struct option long_options[] = {
    {"function", required_argument, NULL, 'f'},
    {"sentence", required_argument, NULL, 's'},
    {"picture", no_argument, NULL, 'p'},
    {"help", no_argument, NULL, 'h'},
    {"plotline", required_argument, NULL, 'l'},
    {"plotsquare", required_argument, NULL, 'k'},
    {NULL, 0, NULL , 0}
};

#ifdef LPC
    file_desc = open(DEVICE, 0); //pokrecemo drajver
    if (file_desc < 0) {
        printf("Can't open device file: %s\n", DEVICE);
        exit(-1);
    }
#endif

    //do {
    next_option = getopt_long ( argc, argv, short_options, long_options, NULL );
    optarg_tmp = optarg;
    option_count = getopt_long ( argc, argv, short_options, long_options, NULL );

    if (option_count != -1) {
        printf("Error: Use one option at a time\n");
        return 0;
    }

    switch (next_option) {
    case 'f':{
        printf("usao u f\n");
        tmp = atoi (optarg_tmp);
        printf("F option and argument is: %d \n", tmp );
#ifdef LPC
        ret_val=ioctl(file_desc, function, tmp);
        if (ret_val < 0) {
            printf("ioctl_set_function failed:%d\n", ret_val);
            exit(-1);
        }
#endif
        break;
    }
    case 's':{
        index_tmp = optind-1;
        while(index_tmp < argc){
            next = argv[index_tmp];
            index_tmp++;
            if(next[0] != '-'){
                if (index_count < 10) {
                    login[index_count++] = next;
                }
            }
            else break;
        }
        optind = index_tmp - 1;
        break;
    }
    case 'p':{
        tmp = atoi(optarg_tmp);
        printf("P option selected\n");
#ifdef LPC
        ret_val=ioctl(file_desc, picture, _myPic);
        if (ret_val < 0) {
            printf("ioctl_set_picture failed:%d\n", ret_val);
            exit(-1);
        }
#endif
        break;
    }
    case 'h':{
        printf("LCD display help: \n");
        printf("\t-f [option]\t\t\t: functions 1 - 4\n");
        printf("\t-s [sentence]\t\t\t: full sentence : print sentence\n");
        printf("\t-p \t\t\t\t: display picture\n");
        printf("\t-h \t\t\t\t: show help\n");
        printf("\t-l [x,y,x1,y1]\t\t\t: plot line\n");
        printf("\t-k [x,y,x1,y1,x2,y2]\t\t: plot square\n");
        printf("\t-c [x,y,x1,y1,x2,y2,collor]\t: fill square with collor\n");
        break;
    }
    case 'l':{
        /*
         * PLOT LINE
         */
        index_tmp = optind-1;
        while(index_tmp < argc){
            next = argv[index_tmp];
            index_tmp++;
            if(next[0] != '-'){
                if (index_count < 4) {
                    login_int[index_count++] = atoi(next);
                }
            }
            else break;
        }
        optind = index_tmp - 1;

        printf("corditantes are : %d %d %d %d ", login_int[0],login_int[1],login_int[2],login_int[3]);
#ifdef LPC
        ret_val=ioctl(file_desc, picture, _myPic);
        if (ret_val < 0) {
            printf("ioctl_set_picture failed:%d\n", ret_val);
            exit(-1);
        }
#endif
        break;

    }
    case -1:{
        break;
    }
    default:
        abort();
        break;
    }

    //} while (next_option != -1);
    if(next_option == 's'){
        for (i=0; i< index_count; i++){
            if( i != index_count-1){
#ifdef LPC
                ret_val = ioctl(file_desc, sentence, login[i]);
                if (ret_val < 0) {
                    printf("ioctl_set_sentence failed:%d\n", ret_val);
                    exit(-1);
                }
                ret_val = ioctl(file_desc, sentence, 'x');
                if (ret_val < 0) {
                    printf("ioctl_set_sentence failed:%d\n", ret_val);
                    exit(-1);
                }
#endif
            }else{
#ifdef LPC
                ret_val = ioctl(file_desc, sentence, login[i]);
                if (ret_val < 0) {
                    printf("ioctl_set_sentence failed:%d\n", ret_val);
                    exit(-1);
                }
#endif
            }
        }
    }

    close(file_desc);

    return 0;
}

