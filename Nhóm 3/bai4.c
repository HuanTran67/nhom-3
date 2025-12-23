#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <delay.h>
#include <alcd.h>
#include <eeprom.h>
int mk[]={0,0,0},mk1,dem=0,dem1=0,nhapsai=2,kd=0,reset=0;
int EEMEM my_mk;
char str[15];
interrupt [TIM0_OVF] void timer0_ngat(void)
{
    TCNT0=0x06;
    if(nhapsai==0)
    {
        dem1=dem1+1;        
        if (dem1==1000)
        {   
            PORTD.5=1;
            PORTC.3=1;   
        }    
        if (dem1==2000)   PORTD.5=0; 
        if (dem1==3000)   PORTD.4=1;
        if (dem1==4000)   PORTD.4=0;
        if (dem1==5000)   PORTD.6=1;
        if (dem1==6000)   
        {
            PORTD.6=0;
            PORTC.3=0;
            dem1=0;
            nhapsai=4;
        }
    }
}
void mbutton()
{
    int i;
    for(i=0;i<3;i++)
        {
            if(i==0) PORTF=0b00111011;
            if(i==1) PORTF=0b00101111;
            if(i==2) PORTF=0b11111110;     
            if(PORTF==0b00111011)
            {
                if (PINF.1==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("0");  
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=0;
                    dem++;
                    delay_ms(1000);
                }
               if(PINF.3==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("1");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=1;
                    dem++;
                    delay_ms(1000);
                }
                if(PINF.5==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("2");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=2;
                    dem++;
                    delay_ms(1000);
                }
            }
            if(PORTF==0b00101111)
            { 
                if (PINF.1==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("3");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=3;
                    dem++;
                    delay_ms(1000);
                }
                else if(PINF.3==0)
                {   
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("4");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=4;
                    dem++;
                    delay_ms(1000);
                }
                else if(PINF.5==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("5");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=5;
                    dem++;
                    delay_ms(1000);
                }
            }
            if(PORTF==0b11111110)
            {
                if (PINF.1==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("6");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=6;
                    dem++;
                    delay_ms(1000);
                }
                else if(PINF.3==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("7");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=7;
                    dem++;
                    delay_ms(1000);
                }
                else if(PINF.5==0)
                {
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("8");
                    delay_ms(500);
                    lcd_gotoxy(dem+3,0);
                    lcd_putsf("*");
                    mk[dem]=8;
                    dem++;
                    delay_ms(1000);
                }
            }
        }  
}
void main(void)
{
    ASSR=0<<AS0;
    TCCR0=0x03;
    TCNT0=0X06;
    OCR0=0;
    TIMSK=0x01;
    #asm("sei")
    DDRF=0b11010101;
    DDRD=0b11110000;
    DDRC.3=1;
    PORTD.7=1;
    lcd_init(16);
    mk1=eeprom_read_word(&my_mk);
    if(mk1==-1) mk1=666;
while (1)
    {
        if(kd==0)
        {
            lcd_gotoxy(0,0);
            lcd_putsf("MK:");
            lcd_gotoxy(0,1); 
            sprintf(str,"mk cu:%d",mk1);
            lcd_puts(str); 
        }   
        mbutton();
        if (reset==0&&dem==3)
        {    if ((mk[0]*100+mk[1]*10+mk[2])==mk1)
            {   
                kd=1;
                reset=1;
                lcd_clear();
                lcd_gotoxy(0,0);
                lcd_putsf("Dung mk");
                dem=0;
                delay_ms(500);
                lcd_clear();
                lcd_gotoxy(0,0);
                lcd_putsf("new:");
            }
            else
            {   
                nhapsai--;
                lcd_clear();
                lcd_gotoxy(0,0);
                lcd_putsf("Sai MK");
                lcd_gotoxy(0,1);
                sprintf(str,"con %d lan",nhapsai);
                lcd_puts(str);
                delay_ms(1000);
                lcd_clear();
            }
            dem=0;
            if(nhapsai==0)
            {    
                kd=1;
                lcd_clear();
                lcd_gotoxy(0,0);
                lcd_putsf("Het luot");  
            }
        }
        if(reset==1)
        {   
            mbutton();
            if(dem==3)  mk1=mk[0]*100+mk[1]*10+mk[2];
            eeprom_update_word(&my_mk,mk1);
            if(dem==3)
            {
                lcd_clear();
                lcd_gotoxy(0,0);
                lcd_putsf("Da luu");
            }
        }      
    }
}