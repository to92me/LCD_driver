/*
 *  ioctl.c - the process to use ioctl's to control the kernel module
 */

#include "ioctl-definicije.h"// isti h fajl mora biti i u device kodu i u program kodu
#include <getopt.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#include "myPic.h" // slika predstavljena kao jednodimenzionalni niz
#define DEVICE "/dev/displej"
#define  NAREDBE_BUFFER   6
#define LPC
//#undef LPC // undef for home compiling and testing without LPC borad
int index_tmp;
char* next;
int index_count;
char *login[50];
unsigned int login_int[15];
int tmp;

int file_desc;
int ret_val;
char* word;
char* word_space;  
char* word_terminator; 
int i;

void PrintHelp();

int main( int argc, char **argv){

    int next_option, option_count;
    char* optarg_tmp;
    const char* const short_options = "s:p:hl:k:b:c:ft:a:";

    const struct option long_options[] = {
    {"sentence", required_argument, NULL, 's'},
    {"picture", no_argument, NULL, 'p'},
    {"help", no_argument, NULL, 'h'},
    {"plotline", required_argument, NULL, 'l'},
    {"plotsquare", required_argument, NULL, 'k'}, //TODO
    {"color", required_argument, NULL, 'b'},
    {"circle", required_argument, NULL, 'c'},
    {"fill", required_argument, NULL, 'f'},
    {"triangle", required_argument, NULL, 't'},
    {"filledsquare", required_argument, NULL, 'a'},
    {NULL, 0, NULL , 0}
};

#ifdef LPC
    file_desc = open(DEVICE, 0);
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
        PrintHelp();
        return 0;
    }

    switch (next_option) {

    case 's':{
        /*
         * STRING
         * done
         */
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
    case 'h':{
        /*
         * HELP
         * done
         */
        PrintHelp();
        break;
    }
    case 'l':{
        /*
         * PLOT LINE
         * done
         */
        printf("plot line \n");
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
        ret_val = ioctl(file_desc, linija, login_int);//ovde prosledjujemo sliku kao niz podataka,definisanu u myPic.h
        if (ret_val < 0){
            printf("ioctl_set_linija failed:%d\n", ret_val);
            exit(-1);
        }
#endif
        break;
    }
    case 'c':{
        /*
         * PLOT CIRCLE
         * done
         */
        printf("crtanje kruga\n");
        index_tmp = optind-1;
        while(index_tmp < argc){
            next = argv[index_tmp];
            index_tmp++;
            if(next[0] != '-'){
                if (index_count < 3) {
                    login_int[index_count++] = atoi(next);
                }
            }
            else break;
        }
        optind = index_tmp - 1;
        printf("arguments are: %d %d %d /n", login_int[0], login_int[1], login_int[2]);
#ifdef LPC
        ret_val = ioctl(file_desc,krug, login_int);
        if (ret_val < 0){
            printf("ioctl_set_string failed:%d\n", 0);
            exit(-1);
        };
#endif

        break;
    }
    case 'b': {
     /*
      * COLOR
      * done
      */
        printf("set color for screen\n");
        index_tmp = optind-1;
        while(index_tmp < argc){
            next = argv[index_tmp];
            index_tmp++;
            if(next[0] != '-'){
                if (index_count < 2) {
                    if ((atoi(next) > 15 )){
                        printf("argument must be in range ( 0 , 15 )\n");
                        abort() ;
                    }
                    login_int[index_count++] = atoi(next);
                }
            }
            else break;
        }
        optind = index_tmp - 1;
        printf("colors are: %d %d \n", login_int[0], login_int[1]);
#ifdef LPC
        ret_val = ioctl(file_desc, colour, login_int);
        if (ret_val < 0){
            printf("ioctl_set_string failed:%d\n", 0);
            exit(-1);
        };
#endif
        break;
    }
    case 'f':{
        /*
         * FILL SCREEEN WITH COLLOR
         * done
         */

        ret_val = ioctl(file_desc, fillscreen, login_int);
        if (ret_val < 0){
            printf("ioctl_set_string failed:%d\n", 0);
            exit(-1);
        };
        break;
    }
    case 't':{
        /*
         * TRIANGLE
         * done
         */

        index_tmp = optind-1;
        while(index_tmp < argc){
            next = argv[index_tmp];
            index_tmp++;
            if(next[0] != '-'){
                if (index_count < 6) {
                    login_int[index_count++] = atoi(next);
                }
            }
            else break;
        }
        optind = index_tmp - 1;
        printf("points  are: %d %d %d %d %d %d \n", login_int[0], login_int[1], login_int[2],login_int[3],login_int[4],login_int[5]);

        ret_val = ioctl(file_desc, trougao, login_int);
        if (ret_val < 0){
            printf("ioctl_set_string failed:%d\n", 0);
            exit(-1);
        };
        break;
    }
    case 'a':{
        /*
         * FILLED SQUARE
         * done
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
        printf("points  are: %d %d %d %d  \n", login_int[0], login_int[1], login_int[2],login_int[3]);

        ret_val = ioctl(file_desc, kocka, login_int);
        if (ret_val < 0){
            printf("ioctl_set_string failed:%d\n", 0);
            exit(-1);
        };
        break;
    }
    case -1:{
        PrintHelp();
        break;
    }
    default:
        PrintHelp();
        abort();
        break;
    }
    // ######################################################################################3
    //} while (next_option != -1);


    if(next_option == 's'){
        for (i=0; i< index_count; i++){
#ifdef LPC
            ret_val = ioctl(file_desc, print, login[i]);
            if (ret_val < 0) {
                printf("ioctl_set_sentence failed:%d\n", ret_val);
                exit(-1);
            }

#endif
        }
    }


    close(file_desc);

    return 0;
}

void PrintHelp(){
    printf("LCD display help: \n");
    printf("\t-s [sentence]\t\t\t: print sentence\n");
    printf("\t-l [x,y,x1,y1]\t\t\t: plot line\n");
    printf("\t-b [0-15,0-15]\t\t\t: set color foreground background\n");
    printf("\t-t [x,y,x1,y1,x2,y2]\t\t: plot triangle\n");
    printf("\t-c [x,y,r]\t\t\t: plot circle\n");
    printf("\t-f \t\t\t\t: fill screen with color\n");
    printf("\t-a [x,y,dx,dy]\t\t\t: filovana kocka na raznju\n");
    printf("\t-h \t\t\t\t: show help\n");
    return;
}

/* TREBA NAM
 * jos nesto mozda ?
 *
 * krug ok
 * linija ok
 * string TODO
 * slika ok
 * boja
 *
 */
