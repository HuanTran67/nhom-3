/*
 * bai13.c
 *
 * Created: 2/12/2025 10:33:31 AM
 * Author: Admin
 */

#include <io.h>
#include <alcd.h>
#include <delay.h>

// --- DINH NGHIA PIN VA CHE DO HOAT DONG ---
#define BT3 PINB.0  // Nhap nhay (Blink)
#define BT2 PINB.3  // Tat (Off)  <--- ÐÃ S?A
#define BT1 PINB.2  // Sang (On)

#define LED_PIN PORTD4 
#define LCD_BACKLIGHT_PIN PORTD7

// Che do hoat dong cua LED
#define MODE_OFF    0
#define MODE_ON     1
#define MODE_BLINK  2

// Chu?i ch?y trên LCD
const flash char message[] = "hello-world"; 
const unsigned char msg_len = 11;
const unsigned char max_col = 16; 

// --- BIEN TOAN CUC ---
unsigned char led_mode = MODE_OFF; 

int scroll_position = 0; 
volatile bit lcd_update_flag = 0; 

// Cac bo dem cho Timer 0 ISR
unsigned char timer0_scroll_counter = 0; 
unsigned char timer0_blink_counter = 0;  
volatile bit blink_toggle = 0;          


// --- HAM KHOI TAO TIMER 0 (Gi? nguyên) ---
void timer0_init(void)
{
    // Clock/1024
    TCCR0 = 0x05; 
    TCNT0 = 0x00;
    TIMSK |= (1<<TOIE0); 
}

// --- DICH VU NGAT TRAN TIMER 0 (Gi? nguyên) ---
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    // --- 1. LOGIC CHAY CHU ---
    timer0_scroll_counter++;
    if (timer0_scroll_counter >= 36) { 
        timer0_scroll_counter = 0;
        scroll_position++;
        
        if (scroll_position > (max_col + msg_len)) {
            scroll_position = 0; 
        }
        lcd_update_flag = 1; 
    }
    
    // --- 2. LOGIC NHAP NHAY LED ---
    if (led_mode == MODE_BLINK) {
        timer0_blink_counter++;
        
        if (timer0_blink_counter >= 15) {
            timer0_blink_counter = 0;
            blink_toggle = !blink_toggle;
            
            if (blink_toggle) {
                PORTD |= (1<<LED_PIN); // SÁNG
            } else {
                PORTD &= ~(1<<LED_PIN); // T?T
            }
        }
    }
}

// --- HAM KIEM TRA CAC NUT BAM VA CHUYEN CHE DO ---
void check_buttons(void)
{
    // Kiem tra BT1 (SÁNG) - PINB.2
    if (BT1 == 0) {
        delay_ms(50); 
        if (BT1 == 0) {
            led_mode = MODE_ON;
            PORTD |= (1<<LED_PIN); 
            lcd_update_flag = 1;   
            while (BT1 == 0); 
        }
    }
    
    // Kiem tra BT2 (T?T) - PINB.3 <-- ÐÃ S?A
    if (BT2 == 0) {
        delay_ms(50); 
        if (BT2 == 0) {
            led_mode = MODE_OFF;
            PORTD &= ~(1<<LED_PIN); 
            lcd_update_flag = 1;    
            while (BT2 == 0); 
        }
    }

    // Kiem tra BT3 (NH?P NHÁY) - PINB.0
    if (BT3 == 0) {
        delay_ms(50); 
        if (BT3 == 0) {
            led_mode = MODE_BLINK;
            timer0_blink_counter = 0; 
            blink_toggle = 0;         
            PORTD &= ~(1<<LED_PIN);   
            lcd_update_flag = 1;      
            while (BT3 == 0); 
        }
    }
}

// --- HAM CHAY CHU TREN LCD (Gi? nguyên) ---
void scroll_text(void)
{
    int i;
    int col;
    
    lcd_gotoxy(0, 0);
    lcd_puts("LED Mode:       ");
    lcd_gotoxy(10, 0);
    
    if(led_mode == MODE_ON) lcd_puts("ON  ");
    else if(led_mode == MODE_OFF) lcd_puts("OFF ");
    else lcd_puts("BLINK");

    lcd_gotoxy(0, 1);
    lcd_puts("                "); 

    for (i = 0; i < msg_len; i++) {
        col = scroll_position - max_col + i; 
        if (col >= 0 && col < max_col) {
            lcd_gotoxy(col, 1); 
            lcd_putchar(message[i]);
        }
    }
    
    lcd_update_flag = 0; 
}


void main(void)
{
    // --- 1. KH?I T?O I/O ---
    
    // C?u hình BT3 (B.0), BT2 (B.3), BT1 (B.2) là Input kéo tr? lên
    // Ðã thêm B.3 (BT2)
    DDRB &= ~((1<<DDB0) | (1<<DDB3) | (1<<DDB2)); 
    PORTB |= (1<<PORTB0) | (1<<PORTB3) | (1<<PORTB2); 
    
    // LED (D.4) và LCD Backlight (D.7) là OUTPUT
    DDRD |= (1<<DDD4) | (1<<DDD7);
    
    // Ð?t tr?ng thái ban d?u
    PORTD &= ~(1<<LED_PIN);         
    PORTD |= (1<<LCD_BACKLIGHT_PIN); 
    
    // --- 2. KH?I T?O LCD & TIMER ---
    
    lcd_init(16);
    lcd_clear();

    timer0_init();
    
    #asm("sei")

    scroll_text(); 

    while (1)
    {
        check_buttons(); 
        
        if (lcd_update_flag) {
            scroll_text();
        }
    }
}
