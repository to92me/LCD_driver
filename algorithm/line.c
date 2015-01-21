#include "line.h"
#include <stdlib.h>
#include <stdio.h>

enum cases { horizontal , vertical , b_alg };
void swap(int* t1, int* t2);
void DrawLine(int x ,int y, int x1 , int y1);
int abs(int t);
void b_aloritham(int x, int y, int x2 , int y2);

int main(){



}


void DrawLine(int x ,int y, int x1 , int y1){
    enum cases line_case;
    int i;
    int dx, dy, tmp ;

    if (x = x1){
        line_case = horizontal;
    }else if( y = y1 ){
        line_case = vertical;
    }else{
        line_case = b_alg;
    }

    switch (line_case) {
    case horizontal:{
        if (y > y1){
            swap(&y,&y1);
        }
        dx = x;
        dy = y;
        while(dy<y1){
            dy++;
            //ret_val = ioctl(file_desc, dot , param_vect);
        }
        break;
    }
    case vertical:{
        if (x > x1){
            swap(&x , &x1);
        }
        dx = x;
        dy = y;
        while(dx < x1){
            dx++;
            //ret_val = ioctl(file_desc, dot , param_vect);
        }
        break;
    }
    case b_alg:{
        b_aloritham(x,y1,x1,y1);
        break;
    }
    default:
        break;
    }

}

void swap(int* t1, int* t2 ){
    int tmp;
    tmp = *t1;
    *t1 = *t2;
    *t2 = tmp;
    return;
}

int abs(int t){
    if (t < 0){
        return -t;
    }else{
        return t;
    }
}


void b_aloritham(int x, int y, int x2 ,int y2){
    int numerator;
    int longest;
    int shortest;
    int i;
    int w = x2 - x ;
    int h = y2 - y ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
    if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;
    if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;
    if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;
    longest = abs(w) ;
    shortest = abs(h) ;
    if (!(longest>shortest)) {
        longest = abs(h) ;
        shortest = abs(w) ;
        if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;
        dx2 = 0 ;
    }
    numerator = longest >> 1 ;
    for ( i=0;i<=longest;i++) {
        // putpixel(x,y,color) ; ## TODO ##
        numerator += shortest ;
        if (!(numerator<longest)) {
            numerator -= longest ;
            x += dx1 ;
            y += dy1 ;
        } else {
            x += dx2 ;
            y += dy2 ;
        }
    }
}

