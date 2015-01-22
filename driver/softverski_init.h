
#ifndef _softverski_h
#define _softverski_h
#include <linux/cdev.h>

#define   BLACK			0x0000		/*   0,   0,   0 */
#define   NAVY			0x000F      /*   0,   0, 128 */
#define   DARK_GREEN	0x03E0      /*   0, 128,   0 */
#define   DARK_CYAN		0x03EF      /*   0, 128, 128 */
#define   MAROON		0x7800      /* 128,   0,   0 */
#define   PURPLE		0x780F      /* 128,   0, 128 */
#define   OLIVE			0x7BE0      /* 128, 128,   0 */
#define   LIGHT_GRAY	0xC618      /* 192, 192, 192 */
#define   DARK_GRAY		0x7BEF      /* 128, 128, 128 */
#define   BLUE			0x001F      /*   0,   0, 255 */
#define   GREEN			0x07E0      /*   0, 255,   0 */
#define   CYAN          0x07FF      /*   0, 255, 255 */
#define   RED           0xF800      /* 255,   0,   0 */
#define   MAGENTA		0xF81F      /* 255,   0, 255 */
#define   YELLOW		0xFFE0      /* 255, 255, 0   */
#define   WHITE			0xFFFF      /* 255, 255, 255 */

// definicije boja

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320
//definicije dimenzije displeja

typedef unsigned short lcd_color_t;

#ifndef NULL
#define NULL (0)
#endif

#define FALSE 0
#define TRUE  1



typedef unsigned char tU8;
typedef unsigned short tU16;
typedef unsigned int tU32;
typedef signed char tS8;
typedef signed short tS16;
typedef signed int tS32;
typedef unsigned char tBool;


//Slede prototipovi funckcija za iscrtavanje po displeju.
//x,y,x,x0,y0,x1 i y1 prestavljaju pozicije na displeju,moraju biti u granicama od 240x320,a color odredjuje boju kojom se crta

void lcd_movePen(unsigned short x, unsigned short y);

void lcd_fillScreen(lcd_color_t color);

void lcd_point(unsigned short x, unsigned short y, lcd_color_t color);

void lcd_drawRect(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, lcd_color_t color);

void lcd_fillRect(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, lcd_color_t color);

void lcd_line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, lcd_color_t color);

void lcd_circle(unsigned short x0, unsigned short y0, unsigned short r, lcd_color_t color);

unsigned long lcd_putChar(unsigned short x, unsigned short y, unsigned char ch);

void lcd_putString(unsigned short x, unsigned short y, unsigned char *pStr);

void lcd_fontColor(lcd_color_t foreground, lcd_color_t background);

void drawImage( int x, int y, unsigned short* image);


//Nasa cdev struktura,u sustini je ne koristimo posto nemamo potrebe,ali bi mogla biti korisna za bafere izmedju user spaca i karnela
struct displej_dev {
	char * naredbe_data_p; /* Pointer to receive buffer */
	unsigned long naredbe_size;         /* amount of data stored in receive buffer */
	struct cdev cdev;	    /* Char device structure		*/
           };

#endif /*  _softverski_h */

