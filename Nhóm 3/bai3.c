#include <io.h>
#include <ds1307.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>
#define servo_1 PORTC.7
#define BT1 PINB.2
#define BT2 PINB.3
#define BT3 PINB.0
char display_buffer[17];
unsigned char hour,min,sec,gio=13,phut=00;
int speed=75,dir=1,dem=0,vi_tri=100,x=0;
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
    TCNT0=0xB0;
    dem++;
    if (dem==2000) dem=0;                  
    if (dem<vi_tri) servo_1=1;              
    else servo_1=0;
}

void main(void)
{
    DDRD.7=1;
    PORTD.7=1;
    DDRC.7=1;
    TCCR1A=(1<<WGM10|1<<COM1A1);     
    TCCR1B=(1<<WGM12|1<<CS11|1<CS10);
    DDRB=(1<<4|1<<5);
    PORTB=0b00001111;
    TCCR0=0x01;                
    TCNT0=0xB0;
    TIMSK=1<<TOIE0;
    #asm("sei")
    lcd_init(16);
    i2c_init();
    rtc_init(3,1,0);
    rtc_set_time(12,59,50);
while (1)
      {   
          if(x==0)
          {
          rtc_get_time(&hour,&min,&sec);
          sprintf(display_buffer,"Time:%02d:%02d:%02d\n",hour,min,sec);
          lcd_clear();
          lcd_puts(display_buffer);
          if(hour==gio&&min==phut&&sec==00)
            {  
                lcd_gotoxy(0,1);
                lcd_putsf("Dang cho ca an");
                dkmotor(dir,speed);
                vi_tri=200;  
                delay_ms(1000);
                dkmotor(0,0);
            } 
          }
          if(BT1==0) x=1;
          if(x==1)
          {
            if(BT1==0) 
            {
                gio=gio+1;
                if(gio==24) gio=0;   
            }
            sprintf(display_buffer,"set hour:%02d",gio);
            lcd_clear();
            lcd_puts(display_buffer);
            delay_ms(200);      
          }
           if(BT2==0) x=2;
          if(x==2)
          {
            if(BT2==0) 
            {
                phut=phut+1;
                if(phut==60) phut=0;   
            }
            sprintf(display_buffer,"set minute:%02d",phut);
            lcd_clear();
            lcd_puts(display_buffer);
            delay_ms(200);      
          }
           if(BT3==0) x=0;    
      }                  
}