/*
 * bai12.c
 *
 * Created: 2/12/2025 10:19:46 AM
 * Author: Admin
 */

/*
 * RGB_Timer_PORTD_Final_FIXED.c
 * FIX: T?c d? ch? ch?y CH?M HON (Timer 0 Counter = 36) và LED T?T/LCD hi?n OFF ban d?u.
 * BT1: PINB.2
 * LED: R(D.4), G(D.5), B(D.6)
 */

#include <io.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h> 

// --- DINH NGHIEM PIN VA HANG SO ---
#define BT1 PINB.2 

// LED RGB trên PORTD
#define R_PIN PORTD4
#define G_PIN PORTD5
#define B_PIN PORTD6
#define LCD_BACKLIGHT_PIN PORTD7

// Chu?i ch?y trên LCD
const flash char message[] = "hello-world"; 
const unsigned char msg_len = 11;
const unsigned char max_col = 16; 

// --- BIEN TOAN CUC ---
// led_state = 3: Tr?ng thái T?T/OFF ban d?u.
unsigned char led_state = 3; 
int scroll_position = 0; 
volatile bit lcd_update_flag = 0; 
unsigned char timer0_counter = 0;


// --- HAM KHOI TAO TIMER 0 ---
void timer0_init(void)
{
    // Clock/1024
    TCCR0 = 0x05; 
    TCNT0 = 0x00;
    TIMSK |= (1<<TOIE0); 
}

// --- DICH VU NGAT TRAN TIMER 0 ---
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    timer0_counter++;
    
    // S?A: Tang h? s? d?m lên 36 d? ch?y ch? ch?m hon (~1.17s/bu?c)
    if (timer0_counter >= 36) { 
        timer0_counter = 0;
        scroll_position++;
        
        if (scroll_position > (max_col + msg_len)) {
            scroll_position = 0; 
        }
        lcd_update_flag = 1; 
    }
}

// --- HAM KIEM TRA NUT BAM VA THAY DOI MAU LED (COMMON CATHODE) ---
void check_button(void)
{
    if (BT1 == 0) {
        delay_ms(50); 
        if (BT1 == 0) {
            
            // L?n b?m d?u tiên (led_state=3) -> led_state=4 -> led_state=0 (RED)
            led_state++;
            if (led_state > 2) {
                led_state = 0;
            }
            
            // T?t t?t c? LED (Ð?t LOW cho D.4, D.5, D.6)
            PORTD &= ~((1<<R_PIN) | (1<<G_PIN) | (1<<B_PIN));
            
            // B?t LED theo tr?ng thái m?i (Ð?t HIGH)
            if (led_state == 0) {
                PORTD |= (1<<R_PIN); // RED
            } else if (led_state == 1) {
                PORTD |= (1<<G_PIN); // GREEN
            } else {
                PORTD |= (1<<B_PIN); // BLUE
            }
            
            while (BT1 == 0); 
        }
    }
}

// --- HAM CHAY CHU TREN LCD ---
void scroll_text(void)
{
    int i;
    int col;
    
    // Xóa hàng 1 (dòng ch? ch?y)
    lcd_gotoxy(0, 1);
    lcd_puts("                "); 

    // C?p nh?t tr?ng thái LED ? Hàng 0 (Dòng trên)
    lcd_gotoxy(0, 0);
    lcd_puts("LED State:      ");
    lcd_gotoxy(11, 0);
    
    // Hi?n th? tr?ng thái LED hi?n t?i (S?a d? hi?n th? "OFF" ban d?u)
    if(led_state == 0) lcd_puts("RED ");
    else if(led_state == 1) lcd_puts("GREEN");
    else if(led_state == 2) lcd_puts("BLUE");
    else lcd_puts("OFF "); // led_state = 3
    

    // Logic ch?y ch? trên Hàng 1
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
    
    // BT1 (B.2) là Input kéo tr? lên
    DDRB &= ~(1<<DDB2); 
    PORTB |= (1<<PORTB2); 
    
    // LED D.4, D.5, D.6 là OUTPUT
    DDRD |= (1<<DDD4) | (1<<DDD5) | (1<<DDD6);
    
    // D.7 (LCD Backlight) là OUTPUT
    DDRD |= (1<<DDD7);
    
    // Ð?t tr?ng thái ban d?u:
    // T?T C? LED T?T (LOW)
    PORTD &= ~((1<<R_PIN) | (1<<G_PIN) | (1<<B_PIN)); 
    // LCD Backlight B?T (HIGH)
    PORTD |= (1<<LCD_BACKLIGHT_PIN);
    
    // --- 2. KH?I T?O LCD & TIMER ---
    
    lcd_init(16);
    lcd_clear();

    timer0_init();
    
    #asm("sei")

    // LED T?T ban d?u. L?n b?m BT1 d?u tiên s? b?t dèn Ð?.

    while (1)
    {
        check_button(); 
        
        if (lcd_update_flag) {
            scroll_text();
        }
    }
}