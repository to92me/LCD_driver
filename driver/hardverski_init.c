#include "lpc24xx.h"				/* LPC23xx/24xx definitions */
#include "hardverski_init.h"	
#include <linux/delay.h>
#include "softverski_init.h"			

#define m_reg_read(reg) (*(volatile unsigned long *)(reg))
#define m_reg_write(reg, data) ((*(volatile unsigned long *)(reg)) = (volatile unsigned long)(data))
#define m_reg_bfs(reg, data) (*(volatile unsigned long *)(reg)) |= (data)
#define m_reg_bfc(reg, data) (*(volatile unsigned long *)(reg)) &= ~(data)


void arm_lcd_init(void) // vrsi podesavanja registara arma,da bi mogli da komuniciramo sa LCD-om
                       // koristi se 16-bitni paralelni interfejs
                        //moguce da podesavanje nekih registara nije neophodno,ostalo iz starih verzija programa
                      //podesavanje jumpera u LPC2468 user guidu,mi koristimo default podesavanja,QVGA controler je SSd1289
{ unsigned int regVal;
   regVal = m_reg_read(PINSEL4);
  regVal &= 0x0FFFFFFF;
  regVal |= 0x50000000;
  m_reg_write(PINSEL4,regVal);
 

  regVal = m_reg_read(PINSEL5);
  regVal &= 0x0FFFFFFF;
  regVal |= 0x05050555;
  m_reg_write(PINSEL5,regVal);

  m_reg_write(PINSEL6,0x55555555);
  m_reg_write(PINSEL8,0x55555555);

 regVal = m_reg_read(PINSEL9);
  regVal &= 0x0F000000;
  regVal |=  0x50555555;
  m_reg_write(PINSEL9,regVal);

 // Initialize EMC for CS2
   m_reg_write(EMC_STA_CFG2,0x00000081);	
   m_reg_write( EMC_STA_WAITWEN2, 0x1);		
   m_reg_write(EMC_STA_WAITOEN2 , 0x2);		
   m_reg_write( EMC_STA_WAITRD2   , 0x10);	
   m_reg_write( EMC_STA_WAITPAGE2 , 0x1F);	
   m_reg_write( EMC_STA_WAITWR2   , 0x8);	
   m_reg_write( EMC_STA_WAITTURN2 , 0x5);	
   m_reg_write( EMC_STA_EXT_WAIT  , 0x0);	
   m_reg_write(FIO3DIR, BACKLIGHT_PIN);
   m_reg_write(FIO3CLR , BACKLIGHT_PIN);	
}


unsigned int lcd_init(void)// vrsi inicijalizaciju displeja,ukljucuje ga,podesava dimenzije ekrana,brzinu osvezavanja i slicno
                           // detalji u datasheetu SSD1289
{


  if (readFromReg(0) == 0x8989)//ocita komandu R00h,ako je ok,da 8989

  {
 
    
    /****************************************************************
     * Initialize Lcd controller (long sequence) 
     ****************************************************************/
    writeToReg(0x07, 0x0021);
    writeToReg(0x00, 0x0001);
    writeToReg(0x07, 0x0723);
    writeToReg(0x10, 0x0000);
    mdelay(200);
    writeToReg(0x07, 0x0033);
    writeToReg(0x11, 0x6830);
    writeToReg(0x02, 0x0600);
   writeToReg(0x0f, 0x0000);
    writeToReg(0x01, 0x2b3f);
    writeToReg(0x0b, 0x5308);
    writeToReg(0x25, 0xa000);
  
    return( TRUE );
  }
  return( FALSE );
}

//Salje naredbe,tj podesvanja LCD-u
 void writeToReg(unsigned short addr, unsigned short data)
{

    LCD_COMMAND_16 = addr; 
    LCD_DATA_16    = data;
    LCD_COMMAND_16 = 0x22;  //restore index register to GRAM,tu se ubacuje vrednost o boji tacke,podrazmjeva se da je svaki sledeci data boja tacke
  
 
}

//Cita stanje registara LCD-a

unsigned short readFromReg(unsigned char addr) 
{
    LCD_COMMAND_16 = addr;
    return LCD_DATA_16; 
}

//Upisuje podatke o boji trenutne tacke na ekranu na LCD-u,Svako upisivanje automatski inkrementuje trenutnu poziciju na ekranu
// void writeToReg(unsigned short addr, unsigned short data)
void
writeToDisp(unsigned short data)
{
LCD_DATA_16 = data;
}


