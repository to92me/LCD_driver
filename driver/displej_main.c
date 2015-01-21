
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

#if 1
#define DPRINT(args...) printk(args)
#else
#define DPRINT(args...)
#endif
#define xDPRINT(args...)

#define m_reg_read(reg) (*(volatile unsigned long *)(reg))
#define m_reg_write(reg, data) ((*(volatile unsigned long *)(reg)) = (volatile unsigned long)(data))
#define m_reg_bfs(reg, data) (*(volatile unsigned long *)(reg)) |= (data)
#define m_reg_bfc(reg, data) (*(volatile unsigned long *)(reg)) &= ~(data)

#define NAREDBE_BUFFER 10

static int ioctl_argv(struct inode* inode,
                      struct file* filep);
static int DisplayClose(struct inode* inode,
                        struct file* file);
int device_ioctl(struct inode *inode,	/* see include/linux/fs.h */
                 struct file *file,	/* ditto */
                 unsigned int ioctl_argc,	/* number and param for ioctl */
                 unsigned long ioctl_argv);

/******************************************************************************
 * Local variables
 *****************************************************************************/
//Ubacujemo i ioctl metodu u file_operations strukturu
static struct file_operations displej_fops = {
    .owner   = THIS_MODULE,
    .open    = DisplayOpen,
    .release = DisplayClose,
    .ioctl = device_ioctl,
};

static struct displej_dev display_device;	/* nas tip */
static signed char naredbe_data[NAREDBE_BUFFER]; /* Receive buffer */
unsigned short  slika_data[10000]; /* Receive buffer */

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


/******************************************************************************
 * Local functions
 *****************************************************************************/


//Funkcija koja se proziva kada neki proces vrsi ioctl poziv na nas drajver fajl.
//Dobijamo dva parametra od znacaja:ioctl_argc-broj ioctl-a koji nas proziva i parametra koji je prosledjen ioctl funkciji
//U nasem primeru koristimo samo dva broja:
// 1 je za pozivanje raznih funkcija za crtanje,parametar odredjuje koja je funkcija u pitanju
// 2 je za crtanje slike koju dobijamo iz user space-a

int device_ioctl(struct inode *inode,	        /* see include/linux/fs.h */
                 struct file *file,	        /* ditto */
                 unsigned int ioctl_argc,	/* number and param for ioctl */
                 unsigned long ioctl_argv)
{
    long i;
    unsigned short *temp_slika;
    signed char *pointer_funkcije;
    long piksel;
    unsigned short jedan_piksel;


    switch (ioctl_argc){
    case funkcije: {//primamo razne naredbe iz user space
        pointer_funkcije = (signed char *)ioctl_argv;

        for (i = 0; i < NAREDBE_BUFFER; i++, pointer_funkcije++)
            get_user(naredbe_data[i], pointer_funkcije);


        if(naredbe_data[1]==1) lcd_fillScreen(RED);//ispuni ce ekran crvenom bojom
        if(naredbe_data[1]==2) lcd_fillScreen(YELLOW);//ispuni ce ekran zutom bojom

        lcd_fontColor(BLACK,WHITE);//podesi boju teksta i pozadine

        if(naredbe_data[1]==3)      lcd_putChar( 200,200,'f');//ispise slovo f na lokaciji 200,200
        if(naredbe_data[1]==4)      lcd_point(10,10,RED);//nacrta crvenu tacku na koordinatama 10,10

        break;
    }
    case slika:{
        temp_slika = (unsigned short *)ioctl_argv;

        piksel=0;
        lcd_movePen(0, 0); //podesi tacku odakle krece da iscrtava displej
        for (i = 0; i < 78000; i++)//imamo 240x320=78000 tacaka,ako se salje slika drugih dimenzija,ovo se mora prepodesiti
        {
            get_user(jedan_piksel, temp_slika+i);//uzima piksel po bajt piksel iz user space niza
            writeToDisp(jedan_piksel); //iz hardverski init.c,iscrtava piksel po piksel na displej,posto se pozicija trenutnog piksela automatski 								//inkrementuje pri upisu
        }                           // ako prenosimo sliku drugih dimenzija,koristiti funkciju drawImage

        break;
    }
    case string:{
        temp_string = (signed char *)

    }
    }

    return 0;
}
//Klasicna funkcija koja se proziva kada se poziva drajver fajl
static int DisplayOpen(struct inode* inode,
                       struct file* filep)
{ 

    struct displej_dev *dev; /* device information */

    dev = container_of(inode->i_cdev, struct displej_dev, cdev);
    filep->private_data = dev; /* for other methods,da ne terebamo vise da koristimo container of */
    dev->naredbe_data_p = naredbe_data;//
    //dev->txdatap = txdata;

    return 0;
}

static int DisplayClose(struct inode* inode,
                        struct file* file)
{
    return 0;
}

//Klasicna funkcija za brisanje drajvera.
static void __exit displej_mod_exit(void)
{
    dev_t dev;
    struct displej_dev * pdisplej = &display_device;
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
    ret=displej_setup_cdev(&display_device);//display_device nam postaje cdev struktura

    return ret;
}

MODULE_LICENSE("GPL");

module_init(displej_mod_init);
module_exit(displej_mod_exit);



