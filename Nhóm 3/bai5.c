#include <io.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#define BT1 PINB.2
float speed=0;
unsigned int giatri;
char buffer[20];
bit b=0;
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
unsigned int read_adc()
{
    ADMUX=0x01;
    delay_us(10);
    ADCSRA|=1<<ADSC;
    while((ADCSRA & 0b00010000)==0);
    ADCSRA|=0x10;
    return ADCW;
}
void main(void)
{
    DDRD.7=1;
    PORTD.7=1;
    lcd_init(16);
    DDRB=0b00110000;
    PORTB=0b00101101;
    TCCR1A=(1<<WGM10|1<<COM1A1);      
    TCCR1B=(1<<WGM12|1<<CS11|1<CS10);    
    ADMUX=0x01;
    ADCSRA=(1<<ADEN|1<<ADPS1|1<<ADPS0);
while (1)
    {
        if(BT1==0) 
        {
            b=(!b); 
            delay_ms(500);
        }
        if(b==0)
        {   
            giatri=read_adc();
            speed=(float) giatri*100/1023-50;
            sprintf(buffer,"speed: %.0f %%",speed);
            lcd_gotoxy(1,0);
            lcd_puts(buffer);
            lcd_clear();
            if(speed>=0)
                dkmotor(1,speed);
            else 
                dkmotor(0,-speed);
        }
        if(b==1)  
        {
               lcd_putsf("speed: 100%");
               dkmotor(1,100);
               lcd_clear();
        }  
    }
}