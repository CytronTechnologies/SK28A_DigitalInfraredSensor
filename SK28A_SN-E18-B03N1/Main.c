/*
 * File:   Main.c
 * Author: Hatake_Chin
 *
 * Created on July 3, 2012, 12:08 PM
 */

// Include all neccessary header files
#include <htc.h>

//=============================================================================
//DEVICE CONFIGURATION WORDS
//=============================================================================
//__CONFIG(        HS &			// High Speed Crystal.
//		 WDTDIS &		// Disable Watchdog Timer.
//		 PWRTEN &		// Enable Power Up Timer.
//		 BORDIS &		// Disable Brown Out Reset.
//		 LVPDIS);		// Disable Low Voltage Programming.
__CONFIG(0x3F32);
//=============================================================================
//Defination
//=============================================================================
#define lcd_RS  RB2
#define lcd_en  RB3
#define lcd_P4  RB4
#define lcd_P5  RB5
#define lcd_P6  RB6
#define lcd_P7  RB7
#define _XTAL_FREQ  2e6

//==============================================================================
// PRIVATE FUNCTION PROTOTYPES
//==============================================================================
void LCD_initialize();
void LCD_4bitsend(unsigned char data);
void LCD_command (unsigned char var);
void LCD_senddata(unsigned char var);
void LCD_putstr(char* csz_string);
void LCD_clear();
void LCD_goto(unsigned char row, unsigned char column);
//==============================================================================
// Main Function
//==============================================================================
int main(void)
{
    TRISC3 = 1;
    TRISB = 0;
    LCD_initialize();
    LCD_clear();
    while(1)
	{
	LCD_goto(0,0);
    	if(RC3 == 1)
    	{
        	LCD_putstr("Block!  "); 
    	}
   	 else if(RC3 == 0)
   	{
            LCD_putstr("Unblock.");
    	}
 	}   
}

//routine to send data to LCD via nibbles in 4-bit mode
void LCD_4bitsend(unsigned char data)
{
    lcd_P7 = 1 & (data >> 7);
    lcd_P6 = 1 & (data >> 6);
    lcd_P5 = 1 & (data >> 5);
    lcd_P4 = 1 & (data >> 4);
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
	

    lcd_P7 = 1 & (data >> 3);
    lcd_P6 = 1 & (data >> 2);
    lcd_P5 = 1 & (data >> 1);
    lcd_P4 = 1 & data;
    lcd_en = 1;//               //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
    //__delay_us(400);            //wait for ~400us
}

//Routine to send command to LCD
void LCD_command (unsigned char var)
{
    lcd_RS = 0;                 //Selected command register
    LCD_4bitsend(var);          //send command via 2 nibbles
}

void LCD_initialize()
{
    __delay_ms(30);
	//set lcd to congfiguration mode
	lcd_en = 1;
	__delay_ms(20); 
	lcd_en = 0;
    lcd_RS = 0;         //selected command register
    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 1;
	lcd_en = 1;
	__delay_ms(2);   
    lcd_en = 0;
              //wait for ~5ms

    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 1;
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
    __delay_ms(2);         //wait for ~100us

    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 1;
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
    __delay_ms(2);          //wait for ~100us

    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 0;
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);  
    lcd_en = 0;
    __delay_ms(2);            //wait for ~100us

    //LCD configuration
    LCD_command (0x28);         //Function set: 4 bit mode, 2 lines, 5x7 font
    LCD_command (0x0C);         //Display on, no cursor & no blinking
    LCD_command (0x01);         //Clear LCD
    LCD_command (0x06);         //Entry mode, auto increment with no shift
}


void LCD_senddata(unsigned char var)
{
    lcd_RS = 1;
    LCD_4bitsend(var);
}

/*******************************************************************************
* PUBLIC FUNCTION: lcd_putstr
*
* PARAMETERS:
* ~ csz_string	- The null terminated string to display.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Display a string on the LCD.
*
*******************************************************************************/
void LCD_putstr(char* csz_string)
{
	// Loop until the end of string.
	while (*csz_string != '\0')
        {
		LCD_senddata(*csz_string);
		csz_string++;      //// Point to next character.
	}
}

void LCD_clear()
{
    LCD_command(0b00000001);
}

void LCD_goto(unsigned char row,unsigned char column)
{
    if(row == 0)
        LCD_command(0x80 | column);
    else
        LCD_command(0x80 | 0x40 | column);
}

