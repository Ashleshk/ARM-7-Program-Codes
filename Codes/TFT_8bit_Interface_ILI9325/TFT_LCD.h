#ifndef TFT_LCD_H
#define TFT_LCD_H

#define MAX_X  240
#define MAX_Y  320   


#define TFT_RST		1<<16
#define TFT_CS		1<<17
#define TFT_RS		1<<18 	
#define TFT_WR  	1<<19
#define TFT_RD		1<<20


#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0


void WriteByte(unsigned char);
void WriteIndex(unsigned int);
void WriteData(unsigned int );
void WriteReg(unsigned int, unsigned int);
void LCD_initialise(void);
void delay(unsigned int);
//void LCD_color(unsigned int );
unsigned char ReadByte(void);
unsigned int  ReadData(void);
//unsigned int ReadReg(unsigned int);
#endif
