#include <ds1307.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>
#define BT1 PINB.2
int dem=0,x=0,y=0;
char display_buffer[17];
unsigned char hour,min,sec,hour1=12,min1=12,sec1=12;
interrupt [TIM0_OVF] void timer0_ngat(void)
{
    TCNT0=0x06;
    dem=dem+1;       
    if (dem==3000)
    {
           x=0; y=0;
           dem=0;
    }
}
void main(void)
{
    DDRD.7=1;
    PORTD.7=1;
    DDRB.2=0;
    PORTB.2=1;
    TCCR0=0x03;
    TCNT0=0x06;
    TIMSK=0x01;
    #asm("sei")    
    lcd_init(16);
    i2c_init();
    rtc_init(3,1,0);
    rtc_set_time(hour1,min1,sec1);   
while (1)
      {   
        if(x==0)
        {
          rtc_get_time(&hour,&min,&sec);           
          sprintf(display_buffer,"Time:%02d:%02d:%02d\n",hour,min,sec);
          lcd_clear();
          lcd_gotoxy(0,0);
          lcd_puts(display_buffer);
        }
        if(BT1==0)    
        {
            x=1; 
            if(dem>1000)
            {
               y++;
               if(y==3) y=0;
            }    
        }
        if(x==1)
          {
             if(y==0)
             {
                  if(BT1==0)    {hour1++; dem=0;}
                  if(hour1==24) hour1=0;                  
                  sprintf(display_buffer,"change hour:%02d",hour1);
                  lcd_clear();
                  lcd_gotoxy(0,0);
                  lcd_puts(display_buffer);
                  rtc_set_time(hour1,min1,sec1);
                  delay_ms(800);   
             }
             if(y==1)
             {
                  if(BT1==0)    {min1++; dem=0;} 
                  if(min1==60)  min1=0;                  
                  sprintf(display_buffer,"change min:%02d",min1);
                  lcd_clear();
                  lcd_gotoxy(0,0);
                  lcd_puts(display_buffer);
                  rtc_set_time(hour1,min1,sec1);
                  delay_ms(800);
             }
             if(y==2)
             {
                  if(BT1==0)    {sec1++; dem=0;} 
                  if(sec1==60)  sec1=0;                  
                  sprintf(display_buffer,"change sec:%02d",sec1);
                  lcd_clear();
                  lcd_gotoxy(0,0);
                  lcd_puts(display_buffer);
                  rtc_set_time(hour1,min1,sec1);
                  delay_ms(800);
             }
          } 
      }                  
}


  