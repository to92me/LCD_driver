
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/delay.h>
#include "lpc24xx.h"
#include "hardverski_init.h"
#include <asm/uaccess.h>	/* for get_user and put_user */


#include "hardverski_init.h"
#include "ioctl-definicije.h" //isti h fajl mora biti i u device kodu i u program kodu
#include "softverski_init.h"
//#include "strip.h"// ubacujemo neku sliku

/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

#if 0
#define DPRINT(args...) printk(args)
#elsif
#define DPRINT(args...)
#endif
#define DPRINT(args...)

#define m_reg_read(reg) (*(volatile unsigned long *)(reg))
#define m_reg_write(reg, data) ((*(volatile unsigned long *)(reg)) = (volatile unsigned long)(data))
#define m_reg_bfs(reg, data) (*(volatile unsigned long *)(reg)) |= (data)
#define m_reg_bfc(reg, data) (*(volatile unsigned long *)(reg)) &= ~(data)

#define NAREDBE_BUFFER 10

enum cases { horizontal , vertical , b_alg };



signed int string_len; 
signed char* string_tmp;  

void swap(int* t1, int* t2);
void DrawLine(int x ,int y, int x1 , int y1);
int abs_new(int t);
void PlotLine(int x, int y, int x2 , int y2);

static int displej_open(struct inode* inode, 
                        struct file* filep);
static int displej_close(struct inode* inode, 
                         struct file* file);
int device_ioctl(struct inode *inode,	/* see include/linux/fs.h */
                 struct file *file,	/* ditto */
                 unsigned int ioctl_num,	/* number and param for ioctl */
                 unsigned long ioctl_param
                 );

/******************************************************************************
 * Local variables
 *****************************************************************************/
//Ubacujemo i ioctl metodu u file_operations strukturu
static struct file_operations displej_fops = {
    .owner   = THIS_MODULE,
    .open    = displej_open,
    .release = displej_close,
    .ioctl = device_ioctl
};

static struct displej_dev displej_device;	/* nas tip */
static signed char naredbe_data[NAREDBE_BUFFER]; /* Receive buffer */

#define MAX_MULTI 20
char multi_buff[MAX_MULTI];

tU16 const COLORS_TAB[16] = {BLACK,
                             NAVY,
                             DARK_GREEN,
                             DARK_CYAN,
                             MAROON,
                             PURPLE,
                             OLIVE,
                             LIGHT_GRAY,
                             DARK_GRAY,
                             BLUE,
                             GREEN,
                             CYAN,
                             RED,
                             MAGENTA,
                             YELLOW,
                             WHITE
                            };




tU16 foreground_colour = BLACK;
tU16 background_colour = WHITE; 

void print_circle(
        unsigned int x0,
        unsigned int y0,
        unsigned int radius);

void plotLine(
        int x0,
        int y0,
        int x1,
        int y1);

#define MAX_Y 320
#define MAX_X 240
unsigned int X_0 = 0;
unsigned int Y_0 = 0;
unsigned int X_1 = 0;
unsigned int Y_1 = 0;
unsigned int X_2 = 0;
unsigned int Y_2 = 0;
unsigned int XS_0 = 0;
unsigned int YS_0 = 0;




int device_ioctl(struct inode *inode,	     
                 struct file *file,
                 unsigned int ioctl_num,
                 unsigned long ioctl_param
                 )
{

    unsigned int pprecnik = 0;
    unsigned int i = 0;
    unsigned char* pointer_funkcije;
    unsigned int* pointer_XY;
    int len = 0;
    unsigned int tmp = 0;
    /*
         * Switch according to the ioctl called
         */
    switch (ioctl_num){

    case colour:{
        pointer_XY = (unsigned int *)(ioctl_param);
        get_user(tmp, pointer_XY);
        DPRINT("\n fore %d \n", tmp);
        foreground_colour = COLORS_TAB[tmp];
        get_user(tmp, pointer_XY+1);
        DPRINT("\n back %d \n", tmp);
        background_colour = COLORS_TAB[tmp];
        break;
    }

    case print:{
        lcd_fontColor(foreground_colour,background_colour);
        pointer_funkcije = (signed char *)(ioctl_param);
        len = 0;
        DPRINT("Kopirao sam ");
        do{
            get_user(multi_buff[len], pointer_funkcije++);
            DPRINT("%c", multi_buff[len]);
        }while(multi_buff[len++] != '\0');
        DPRINT("\n");
        DPRINT("zavrsio kopiranje, len je %i \n", len);
        i = 0;
        while(i < len){  // ove maxove treba podesiti/preracunati
            DPRINT("pisem %c \n", multi_buff[i]);
            // fali y ogranicenje
            if(!lcd_putChar(XS_0, YS_0, multi_buff[i])){   // ovo mozda treba da se pomeri za pixel (1)

                DPRINT("upisao slovo %c \n", multi_buff[i]);
                XS_0 += 6;
                i++;

            }
            else{
                if( YS_0 < MAX_Y){ // ili ide 0 ??? tj obrnuto
                    DPRINT("nov red \n");
                    YS_0 += 8;
                    XS_0 = 0;
                }
                else{
                    lcd_fillScreen(background_colour);
                    YS_0 = 0;
                    XS_0 = 0;
                }
            }
            DPRINT("len je %i\n",len);
            DPRINT("y0 je %i \n",YS_0);

        }
        DPRINT("zavrsio ispis\n");

        break;
    }

    case krug:{
        pointer_XY = (unsigned int *)(ioctl_param);
        get_user(X_0, pointer_XY);
        get_user(Y_0, pointer_XY+1);
        get_user(pprecnik, pointer_XY + 2);
        DPRINT("X0 je %i \n", X_0);
        DPRINT("Y0 je %i \n", Y_0);
        DPRINT("pprecnik je %i \n", pprecnik);
        print_circle(X_0,Y_0,pprecnik);
        X_0 = 0;
        Y_0 = 0;
        break;
    }


    case linija:{  // ovo bi bilo najbolje linijama uraditi to cu promeniti
        pointer_XY = (unsigned int *)(ioctl_param);

        get_user(X_0, pointer_XY);
        get_user(Y_0, pointer_XY + 1);
        get_user(X_1, pointer_XY + 2);
        get_user(Y_1, pointer_XY + 3);

        DPRINT("X0 je %i \n", X_0);
        DPRINT("Y0 je %i \n", Y_0);
        DPRINT("X1 je %i \n", X_1);
        DPRINT("Y1 je %i \n", Y_1);

        //plotLine(X_0, Y_0, X_1, Y_1);
        PlotLine(X_0, Y_0, X_1, Y_1);
        X_0 = 0;
        Y_0 = 0;
        break;
    }


    case fillscreen:{
        lcd_fillScreen(background_colour);;
        break;
    }
    case trougao:{
        pointer_XY = (unsigned int *)(ioctl_param);

        get_user(X_0, pointer_XY);
        get_user(Y_0, pointer_XY + 1);
        get_user(X_1, pointer_XY + 2);
        get_user(Y_1, pointer_XY + 3);
        get_user(X_2, pointer_XY + 4);
        get_user(Y_2, pointer_XY + 5);
        PlotLine(X_0, Y_0, X_1, Y_1);
        PlotLine(X_1, Y_1, X_2, Y_2);
        PlotLine(X_0, Y_0, X_2, Y_2);
    }
    case kocka:{
        int i;
        pointer_XY = (unsigned int *)(ioctl_param);
        get_user(X_0, pointer_XY);
        get_user(Y_0, pointer_XY + 1);
        get_user(X_1, pointer_XY + 2);
        get_user(Y_1, pointer_XY + 3);
        for( i = X_0 ; i <= X_1; i++){
            PlotLine(i , Y_0, i, Y_0 + Y_1);
        }
        break;
    }
  }
    return 0; //kraj
}
//Klasicna funkcija koja se proziva kada se poziva drajver fajl
static int displej_open(struct inode* inode, 
                        struct file* filep)
{ 

    struct displej_dev *dev; /* device information */

    dev = container_of(inode->i_cdev, struct displej_dev, cdev);
    filep->private_data = dev; /* for other methods,da ne terebamo vise da koristimo container of */
    dev->naredbe_data_p = naredbe_data;//
    //dev->txdatap = txdata;

    return 0;
}

static int displej_close(struct inode* inode, 
                         struct file* file)
{
    return 0;
}

//Klasicna funkcija za brisanje drajvera.
static void __exit displej_mod_exit(void)
{
    dev_t dev;
    struct displej_dev * pdisplej = &displej_device;
    DPRINT("displej_mod_exit\n");

    dev = MKDEV(displej_MAJOR, 0);
    cdev_del(&pdisplej->cdev);
    unregister_chrdev_region(dev, 1);
}
//Inicijalizujemo strukturu.U ovom drajveru ne koristimo cdev_alloc,vec sami pravimo cdev strukturu 
static int displej_setup_cdev(struct displej_dev *dev)
{
    int err;
    int devno = MKDEV(displej_MAJOR, 0);

    cdev_init(&dev->cdev, &displej_fops);//initialize the structure
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &displej_fops;
    err = cdev_add (&dev->cdev, devno, 1);
    /* Fail gracefully if need be */
    if (err)
        DPRINT("Error %d adding displej driver\n", err);
    else
        DPRINT("Successfuly added displej driver..\n");
    return err;
}
//Klasicna funkcija za inijalizaciju drajvera.Poziva funkcije arm_lcd_init() i lcd_init() iz hardverski_init.c koja inicijalizije  arm-a i lcd-a
static int __init displej_mod_init(void)
{
    int ret;
    dev_t dev;

    DPRINT("displej_mod_inicijalizacija\n");
    lcd_fillScreen(background_colour);
    dev = MKDEV(displej_MAJOR, 0);

    arm_lcd_init();

    if ( lcd_init() != TRUE )//iz hardverski_init
    {
        DPRINT("\nERROR: Failed to identify displej!");
        while( 1 );		/* Display fatal error */
    }  else  DPRINT("LCD-displej radi\n");

    ret = register_chrdev_region(dev, 1, "displej");
    //imamo samo jedan device,pa zauzimamo samo jedan minor broj

    if(ret)
    {
        DPRINT("displej: failed to register char device\n");
        return ret;
    }
    ret=displej_setup_cdev(&displej_device);//displej_device nam postaje cdev struktura

    return ret;
}

void print_circle(
        unsigned int x0,
        unsigned int y0,
        unsigned int radius)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    lcd_point(x0, y0 + radius, foreground_colour);
    lcd_point(x0, y0 - radius, foreground_colour);
    lcd_point(x0 + radius, y0, foreground_colour);
    lcd_point(x0 - radius, y0, foreground_colour);

    while(x < y)
    {
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;
        lcd_point(x0 + x, y0 + y, foreground_colour);
        lcd_point(x0 - x, y0 + y, foreground_colour);
        lcd_point(x0 + x, y0 - y, foreground_colour);
        lcd_point(x0 - x, y0 - y, foreground_colour);
        lcd_point(x0 + y, y0 + x, foreground_colour);
        lcd_point(x0 - y, y0 + x, foreground_colour);
        lcd_point(x0 + y, y0 - x, foreground_colour);
        lcd_point(x0 - y, y0 - x, foreground_colour);
    }
}







void swap(int* t1, int* t2 ){
    int tmp;
    tmp = *t1;
    *t1 = *t2;
    *t2 = tmp;
    return;
}

int abs_new(int t){
    if (t < 0){
        return -t;
    }else{
        return t;
    }
}


void PlotLine(int x, int y, int x2 ,int y2){

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
    //printk("b alg \n\n");
    longest = abs_new(w) ;
    shortest = abs_new(h) ;
    if (!(longest>shortest)) {
        longest = abs_new(h) ;
        shortest = abs_new(w) ;
        if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;
        dx2 = 0 ;
    }
    numerator = longest >> 1 ;
    for ( i=0;i<=longest;i++) {
        lcd_point(x,y, foreground_colour) ; // ## TODO ##
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


MODULE_LICENSE("GPL");

module_init(displej_mod_init);
module_exit(displej_mod_exit);
