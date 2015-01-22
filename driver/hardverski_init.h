
#ifndef _LCD_HW_
#define _LCD_HW_


/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/
#define LCD_COMMAND_16  (*((volatile unsigned short *) 0x82000000)) 
#define LCD_DATA_16     (*((volatile unsigned short *) 0x82000002))



#define BACKLIGHT_PIN 0x10000000  //P3.28

extern void 	      arm_lcd_init( void );
extern unsigned int   lcd_init(void);
extern void           writeToDisp(unsigned short data);
extern void           writeToReg(unsigned short data, unsigned short addr);
extern unsigned short readFromReg(unsigned char addr);


#endif /* _LCD_HW_ */
