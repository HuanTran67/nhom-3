/*
 * bai 7.c
 *
 * Created: 25/11/2025 10:13:25 AM
 * Author: Admin
 */


#include <io.h>
#include <delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <alcd.h>

int keypad[3][3] = {1,2,3,4,5,6,7,8,9};
char data[16];
void BUTTON() {
    char a;
    int i,j;
    for(j=0; j<3; j++){
        if(j == 0)
            PORTF = 0b11111101;
        if (j == 1)
            PORTF = 0b11110111;
        if (j == 2)
            PORTF = 0b11011111; 
            for (i=0; i<3; i++) {
                if (i == 0) {
                    a = PINF&0x04;
                    if( a!=0x04){
                    lcd_gotoxy(0,0);
                    sprintf(data, "%u", keypad[i][j]);
                    lcd_puts(data);
                    delay_ms(500);
                    }}
                if (i == 1) {
                    a = PINF&0x10;
                    if( a!=0x10) {
                    lcd_gotoxy(0,0);
                    sprintf(data, "%u", keypad[i][j]);
                    lcd_puts(data);
                    delay_ms(500);
                    }}
                if (i == 2) {
                    a = PINF&0x01;
                    if (a != 0x01){
                    lcd_gotoxy(0,0);
                    sprintf(data, "%u", keypad[i][j]);
                    lcd_puts(data);
                    delay_ms(500);
                    }}
    }
    }
    }                      
        

void main()
{
    DDRD = 0xFF;
    PORTD = 0xB6;
    lcd_init(16);
    DDRF = 0b11101010;
    PORTF = 0b00010101;
    
while (1)
    {
    BUTTON();
    lcd_clear();

    }
}

