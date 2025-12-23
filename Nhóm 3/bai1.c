#include <io.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>
#define BT1 PINB.2
#define BT2 PINB.3
#define BT3 PINB.0
#define LED PORTD.5
int dem=0,speed=20,x=0;
void dkmotor(int chieu, int toc_do)
{
    if(chieu==1)
    {
        PORTB.4=1;
        OCR1A=255-255*toc_do/100;
    }
    if(chieu==0)
    {
        
        PORTB.4=0;
        OCR1A=255*toc_do/100;
    }
}
interrupt [TIM0_OVF] void timer0_ngat(void)
{
    TCNT0=0x06;
    dem=dem+1; 
    if(x==1)
    {   
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_putsf("Dang giat");
        PORTC.3=1;     
        if (dem==500)   
        {
                speed=speed+20; 
                dkmotor(0,speed);  
        } 
        if(dem%500==0 && speed<=100)
        {
                speed=speed+20; 
                dkmotor(0,speed);        
        }
        if(dem==3000) 
        {
            dem=0;
            x=2;      
        }
    }
    if(x==2)
        {
               lcd_clear();
               lcd_gotoxy(0,0);
               lcd_putsf("Dang xa");
               dkmotor(0,0);
               PORTC.2=1;        
               if(dem==500) 
               {
                   dem=0;
                   x=3; 
               }
        }
    if(x==3)
    {
        PORTC.2=0;        
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_putsf("Dang vat");
        dkmotor(1,100);
        if(dem==1500) 
        {
            dem=0;
            x=4;
        }
    }
    if(x==4)
     {
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_putsf("Xong");
        dkmotor(0,0);
        x=5;
        dem=0;
     }
}
void main(void)
{
    DDRB=0b00110000;
    PORTB=0b00101101;
    TCCR1A=(1<<WGM10|1<<COM1A1);      
    TCCR1B=(1<<WGM12|1<<CS11|1<CS10);      
    TCCR0=0x03;
    TCNT0=0x06;
    OCR0=0x00;
    TIMSK=0x01;
    #asm("sei")
    DDRD.7=1;
    PORTD.7=1;
    DDRC=1<<3|1<<2;
    lcd_init(16);
    lcd_gotoxy(0,0);
    lcd_putsf("1-giat 2-xa 3-vat");
while (1)
    {
        if(BT1==0)  
        {
            delay_ms(250);    
            x=1;
            dem=0;
        }
        if(BT2==0)  
        {
            delay_ms(250);    
            x=2;
            dem=0;
        }
        if(BT3==0)  
        {
            delay_ms(250);    
            x=3;  
            dem=0;
        }        
    }
}