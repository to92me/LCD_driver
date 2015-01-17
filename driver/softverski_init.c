#include "lpc24xx.h"				/* LPC23xx/24xx definitions */
#include "hardverski_init.h"	
#include <linux/delay.h>
#include "softverski_init.h"
#include <asm/uaccess.h>	/* for get_user and put_user */
#include "font5x7.h"

static lcd_color_t  foregroundColor = WHITE;
static lcd_color_t  backgroundColor = BLACK;
static unsigned char const  font_mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
/******************************************************************************
** Function name:		lcd_movePen-pomeri tacku
**
** Descriptions:		Move the pen to a particular location.		
**
** parameters:			pixel x and y
** Returned value:		None
** 
******************************************************************************/
void lcd_movePen(unsigned short x, unsigned short y)
{
  
    writeToReg(0x4e, x & 0xff);
    writeToReg(0x4f, y & 0x1ff);
  
 
  return;
}
/******************************************************************************
** Function name:		lcd_point
**
** Descriptions:		Draw a point at {x0, y0} on the LCD		
**				if {x0,y0} is out of range, display nothing.
** parameters:			x0, y0, color
** Returned value:		None
** 
******************************************************************************/
void lcd_point(unsigned short x, unsigned short y, lcd_color_t color)
{
  if( x >= DISPLAY_WIDTH )  
  {
	return;
  }
  
  if(y >= DISPLAY_HEIGHT)
  {
	return;
  }
  lcd_movePen(x, y);
  writeToDisp(color);
  return;
}

/******************************************************************************
** Function name:		lcd_fillScreen
**
** Descriptions:		Fill the LCD screen with color		
**
** parameters:			Color
** Returned value:		None
** 
******************************************************************************/
void lcd_fillScreen(lcd_color_t color)
{
  unsigned short i = 0;
  unsigned short j = 0;

  
 
  lcd_movePen(0, 0);

  for(i=0; i < DISPLAY_HEIGHT; i++)
  {
    for(j=0; j<DISPLAY_WIDTH; j++)
    {
      writeToDisp(color);
    }
  }
  return;
}

/******************************************************************************
** Function name:		lcd_putChar
**
** Descriptions:		Put one chacter on the LCD for display		
**
** parameters:			pixel X and Y, and the character
** Returned value:		TRUE or FALSE, if the pixels given are out of range,
**						nothing will be written.
** 
******************************************************************************/
unsigned long lcd_putChar(unsigned short x, unsigned short y, unsigned char ch)
{  
  unsigned char data = 0;
  unsigned char i = 0, j = 0;
  
  lcd_color_t color = BLACK;

  if((x >= (DISPLAY_WIDTH - 8)) || (y >= (DISPLAY_HEIGHT - 8)) )
  {
	return( FALSE );
  }

  if( (ch < 0x20) || (ch > 0x7f) )
  {
	ch = 0x20;		/* unknown character will be set to blank */
  }
   
  ch -= 0x20;
  for(i=0; i<8; i++)
  {
    data = font5x7[ch][i];
    for(j=0; j<6; j++)
    {
	    if( (data&font_mask[j])==0 )
	    {  
		    color = backgroundColor;
	    }
	    else
	    {
		    color = foregroundColor;
	    }
	    lcd_point(x, y, color);       
	    x++;
    }   
    y++;
    x -= 6;
  }
  return( TRUE );
}

/******************************************************************************
** Function name:		lcd_fontColor
**
** Descriptions:		foreground and back ground color setting		
**
** parameters:			foreground color and background color
** Returned value:		None
** 
******************************************************************************/

void lcd_fontColor(lcd_color_t foreground, lcd_color_t background)
{
  foregroundColor = foreground;
  backgroundColor = background;
  return;
}





