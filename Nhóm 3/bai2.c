#include <io.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#define servo_1 PORTC.7
int tangx=1,tangy=1,dem=0,vi_tri=10;
char str[20];
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
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    TCNT0=0xB0;
    dem=dem+1;
    if (dem==2000) dem=0;                   
    if (dem<=vi_tri) servo_1=1;              
    else servo_1=0;
}
void button ()
{
    int i;
    for(i=0;i<3;i++)
        {
            if(i==0) PORTF=0b00111011;
            if(i==1) PORTF=0b11101111;
            if(i==2) PORTF=0b11111110;     
            if(PORTF==0b00111011)
            {
                if (PINF.1==0)  
                {
                    tangy=1;
                    delay_ms(100);    
                }
                if(PINF.3==0)
                {
                    tangy=2;
                    delay_ms(100);
                }
                if(PINF.5==0)
                {
                   tangy=3;
                   delay_ms(100);
                }
            }
            if(PORTF==0b11101111)
            { 
                if (PINF.1==0)
                {
                    tangy=4;
                    delay_ms(100);    
                }       
                else if(PINF.3==0)
                {
                    tangy=5;
                    delay_ms(100);    
                }   
                else if(PINF.5==0)
                {
                    lcd_clear();
                    lcd_gotoxy(0,0);
                    lcd_putsf("OPEN");
                    delay_ms(1000);    
                }   
            }
            if(PORTF==0b11111110)
            {
                if (PINF.1==0)
                {
                    lcd_clear();
                    lcd_gotoxy(0,0);
                    lcd_putsf("CLOSE");
                    delay_ms(1000);    
                }     
                else if(PINF.3==0)
                {
                    lcd_clear();
                    lcd_gotoxy(0,0);
                    lcd_putsf("EMERGENCY");
                    delay_ms(1000);   
                }   
            }  
        }
}
void main(void)
{
    TCCR1A=(1<<WGM10|1<<COM1A1);      
    TCCR1B=(1<<WGM12|1<<CS11|1<CS10); 
    DDRB=(1<<4|1<<5);
    ASSR=0x00;   
    TCCR0=0x01;
    TCNT0=0xB0;
    TIMSK=0x01;
    #asm("sei")
    DDRF=0b11010101;    
    DDRD.7=1;
    PORTD.7=1;
    DDRC.3=1;
    lcd_init(16);
while (1)
    {   
        button();
        if(tangx<tangy)
        {   
            lcd_clear();  
            sprintf(str,"level:%d  to:%d",tangx,tangy);                               
            lcd_gotoxy(0,0);
            lcd_puts(str);
            lcd_gotoxy(0,1);
            lcd_putsf("up");
            vi_tri=150;
            delay_ms(2000);
            vi_tri=10;
            dkmotor(1,100);
            tangx++;
            delay_ms(1000);  
        }
        else if(tangx>tangy)
        {
            lcd_clear();  
            sprintf(str,"level:%d  to:%d",tangx,tangy);                             
            lcd_gotoxy(0,0);
            lcd_puts(str);
            lcd_gotoxy(0,1);
            lcd_putsf("down");
            vi_tri=150;
            delay_ms(2000);
            vi_tri=10;
            dkmotor(0,90);
            tangx--;
            delay_ms(1000);  
        }
        else    
        {
        dkmotor(1,0);
        lcd_clear();  
        sprintf(str,"level: %d",tangx);                               
        lcd_gotoxy(0,0);
        lcd_puts(str);
        vi_tri=150;
        delay_ms(2000);
        vi_tri=10;
        delay_ms(1000);
        }
    }
}