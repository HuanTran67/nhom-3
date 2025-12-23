#include <io.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#define BT1 PINB.2
#define BT2 PINB.3
#define BT3 PINB.0
int dir=0;
float speed=0;
unsigned int giatri;
char buffer[20];
void dkmotor(int chieu, int toc_do)
{
    if(chieu==1)
    {
        PORTB.5=1;
        OCR0=255-255*toc_do/100;
    }
    if(chieu==0)
    {
        
        PORTB.5=0;
        OCR0=255*toc_do/100;
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
    ASSR=0x00;
    TCCR0=0b01101011;       
    ADMUX=0x01;
    ADCSRA=(1<<ADEN|1<<ADPS1|1<<ADPS0);
while (1)
    {
        giatri=read_adc();
        speed=(float) giatri*100/1023;
        sprintf(buffer,"speed: %.0f%%",speed);
        lcd_gotoxy(1,0);
        lcd_puts(buffer);
        lcd_clear();
        dkmotor(dir,speed);
    }
}
