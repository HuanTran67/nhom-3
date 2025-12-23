#include <io.h>
#include <delay.h>
#include <stdio.h>
#include <alcd.h>
#define BT3 PINB.0
unsigned int giatri,t=0;
unsigned char buffer[10];
unsigned int read_adc()
{
    ADMUX=0x00;
    delay_us(10);
    ADCSRA|=1<<ADSC;
    while((ADCSRA & 0b00010000)==0);
    ADCSRA|=0x10;
    return ADCW;
}
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
void main(void)
{
    DDRD.7=1;
    DDRC.3=1;
    PORTD.7=1;
    TCCR1A=(1<<WGM10|1<<COM1A1);      
    TCCR1B=(1<<WGM12|1<<CS11|1<CS10); 
    DDRB=(1<<4|1<<5);
    lcd_init(16);
    ADMUX=0x00;
    ADCSRA=(1<<ADEN|1<<ADPS1|1<<ADPS0);
while (1)
    {
        giatri=read_adc();
        sprintf(buffer,"%d",giatri);
        lcd_gotoxy(0,1);
        lcd_puts(buffer);
        delay_ms(100);
        if (giatri<500&&t==1)
        {               
            lcd_clear();
            lcd_gotoxy(0,0);
            lcd_putsf("troi mua");         
            PORTC.3=1;
            dkmotor(1,100);
            delay_ms(2000);
            t=0;
            dkmotor(1,0);
        }
        else if(giatri>500&&t==0)
        {   
            lcd_clear();
            lcd_gotoxy(0,0);
            lcd_putsf("troi nang");
            PORTC.3=0;
            dkmotor(0,100);
            delay_ms(2000);
            t=1;
            dkmotor(0,0);
        }        
    }
}
