/*
 * test9.c
 *
 * Created: 27/11/2025
 * Author: Admin
 */


#include <io.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h> 

// --- DINH NGHIA PIN VA CHÂN ---
#define BT1 PINB.2
#define BT2 PINB.3
#define LED_PIN PORTD5        // Chân D.5 (LED)
#define ROLE1_PIN PORTC3      // Chân C.3 (Relay 1)
#define ROLE2_PIN PORTC2      // Chân C.2 (Relay 2)
#define LCD_BACKLIGHT_PIN PORTD7 // Chân D.7 (Ðèn n?n LCD)

// Khai báo bi?n toàn c?c
int dem = 0; 
char buf[10]; 
unsigned char last_btn_state = 0; 

// --- HAM KIEM TRA NUT BAM VA LOGIC DEM (Gi? nguyên) ---
void check_button_sequence(void)
{
    // Logic d?m TANG/GI?M
    if (BT1 == 0) {
        delay_ms(5); 
        if (BT1 == 0) {
            if (last_btn_state == 2) {
                dem--;
                last_btn_state = 0; 
            } else if (last_btn_state == 0) {
                last_btn_state = 1;
            }
            while (BT1 == 0); 
        }
    }

    if (BT2 == 0) {
        delay_ms(5); 
        if (BT2 == 0) {
            if (last_btn_state == 1) {
                dem++;
                last_btn_state = 0; 
            } else if (last_btn_state == 0) {
                last_btn_state = 2;
            }
            while (BT2 == 0); 
        }
    }
}


// --- HAM KIEM TRA DIEU KHIEN RELAY VA LED (ACTIVE-LOW) ---
void check_control_logic(void)
{
    // --- 1. LED (D.5) và ROLE 1 (C.3) ---
    if (dem > 0) {
        // BAT LED (D.5)
        PORTD |= (1<<LED_PIN);            
        
        // DONG ROLE 1 (C.3) - Active-Low: LOW
        PORTC &= ~(1<<ROLE1_PIN);         
    } 
    else {
        // TAT LED (D.5)
        PORTD &= ~(1<<LED_PIN);           
        
        // NHA ROLE 1 (C.3) - Active-Low: HIGH
        PORTC |= (1<<ROLE1_PIN);          
        
        if (dem < 0) {
            dem = 0; 
        }
    }

    // --- 2. ROLE 2 (C.2) ---
    if (dem > 5) {
        // Ðóng Role 2
        PORTC &= ~(1<<ROLE2_PIN);         // DONG ROLE 2 (Active-Low: LOW)
    }
    else {
        // Nh? Role 2
        PORTC |= (1<<ROLE2_PIN);          // NHA ROLE 2 (Active-Low: HIGH)
    }
}

// --- HAM HIEN THI LCD (Gi? nguyên) ---
void display_lcd(void)
{
    itoa(dem, buf); 
    
    lcd_clear();
    lcd_gotoxy(0, 0);
    lcd_puts("SU KIEN:");
    
    lcd_gotoxy(9, 0);
    lcd_puts(buf);
}

void main(void)
{
    // --- 1. C?U HÌNH PORT ---
    // Nút b?m (PORTB: B.2, B.3) - Input, Kéo tr? lên
    DDRB &= ~((1<<DDB2) | (1<<DDB3)); 
    PORTB |= (1<<PORTB2) | (1<<PORTB3); 
    
    // Kh?i t?o I/O cho PORTD (Ch? LCD và LED D.5)
    DDRD = 0xA0; // D.7 (LCD), D.5 (LED) là Output
    PORTD = 0xA0;
    
    // Kh?i t?o PORTC cho RELAY
    // D.3 (ROLE1) và D.2 (ROLE2) là Output
    DDRC |= (1<<DDC3) | (1<<DDC2);
    
    // --- 2. KH?I T?O LCD ---
    lcd_init(16);
    lcd_clear();

    // --- 3. ÐI?U CH?NH TR?NG THÁI BAN Ð?U ---
    
    // ROLE1 (C.3) và ROLE2 (C.2) v? HIGH (Nh? cho Active-Low)
    PORTC |= (1<<ROLE1_PIN) | (1<<ROLE2_PIN);
    
    // LED (D.5) v? LOW (T?t)
    PORTD &= ~(1<<LED_PIN);             
    
    // Ð?m b?o D.7 (LCD_BACKLIGHT) LUÔN HIGH
    PORTD |= (1<<LCD_BACKLIGHT_PIN);
    
    // Hi?n th? tr?ng thái ban d?u
    display_lcd();
    
    while (1)
    {
        check_button_sequence();
        check_control_logic();
        display_lcd();

        delay_ms(100); 
    }
}