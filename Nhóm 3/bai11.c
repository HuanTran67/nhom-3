/*
 * bai 11.c
 *
 * Created: 27/11/2025
 * Author: Admin
 */

#include <io.h>
#include <alcd.h>
#include <delay.h>

// Ð?nh nghia các nút b?m
#define BT1 PINB.2 // Ch?y t? Trái -> Ph?i (Hàng 0 -> Hàng 1, ch?y tu?n t?)
#define BT2 PINB.3 // Ch?y t? Ph?i -> Trái (Hàng 1 -> Hàng 0, ch?y LI?N M?CH)
#define BT3 PINB.0 // Xóa màn hình
#define LCD_BT PORTD.7 // Chân di?u khi?n dèn n?n LCD

// Ð?T CHU?I VÀO B? NH? FLASH (FIX l?i lcd_putsf)
const flash char message[] = "hello world"; 
const unsigned char msg_len = 11;     // Ð? dài chu?i "hello world"

// Bi?n mode d? luu tr?ng thái ho?t d?ng
unsigned char mode = 0;

// Hàm ki?m tra và x? lý nút b?m
void check_button(void)
{
    // Ki?m tra BT1 (Ch?y T->P)
    if (BT1 == 0) {
        delay_ms(50); 
        if (BT1 == 0) {
            mode = 1; 
            while (BT1 == 0); 
        }
    }
    // Ki?m tra BT2 (Ch?y P->T)
    if (BT2 == 0) {
        delay_ms(50); 
        if (BT2 == 0) {
            mode = 2; 
            while (BT2 == 0); 
        }
    }
    // Ki?m tra BT3 (Xóa màn hình)
    if (BT3 == 0) {
        delay_ms(50); 
        if (BT3 == 0) {
            mode = 3; 
            while (BT3 == 0); 
        }
    }
}

void main(void)
{
    // --- C?u hình I/O cho nút b?m ---
    DDRB.2 = 0; PORTB.2 = 1; // BT1
    DDRB.3 = 0; PORTB.3 = 1; // BT2
    DDRB.0 = 0; PORTB.0 = 1; // BT3

    // --- C?u hình LCD ---
    DDRD.7 = 1;
    LCD_BT = 1; 
    lcd_init(16);
    lcd_clear();

    while (1)
    {
        signed char i; 
        check_button(); 

        if (mode == 1) { // Ch?y T->P (Hàng 0 -> Hàng 1, v?n ch?y tu?n t?)
            while (mode == 1) { 
                
                // --- CH?Y HÀNG 0 (TRÁI QUA PH?I) ---
                for (i = 0; i < 16 - msg_len + 1; i++) {
                    lcd_clear();
                    lcd_gotoxy(i, 0); 
                    lcd_putsf(message);
                    delay_ms(200); 
                    check_button();
                    if (mode != 1) { break; } 
                }
                if (mode != 1) { break; } 

                // --- CH?Y HÀNG 1 (TRÁI QUA PH?I) ---
                for (i = 0; i < 16 - msg_len + 1; i++) {
                    lcd_clear();
                    lcd_gotoxy(i, 1); 
                    lcd_putsf(message);
                    delay_ms(200);
                    check_button();
                    if (mode != 1) { break; } 
                }
                 if (mode != 1) { break; } 
            }
            
        } else if (mode == 2) { // Ch?y P->T (Hàng 1 -> Hàng 0, LI?N M?CH)
            while (mode == 2) { 
                
                // Vòng l?p don ch?y li?n m?ch qua 2 hàng
                // i ch?y t? 15 (b?t d?u Hàng 1) d?n -12 (ch? ra kh?i Hàng 0)
                // T?ng c?ng 28 bu?c d?ch chuy?n
                for (i = 15; i >= -12; i--) { 
                    signed char col_row0 = i + 16; 
                    
                    lcd_clear();
                    
                    // 1. Hi?n th? ph?n trên Hàng 1
                    // N?u i n?m trong ph?m vi hi?n th? trên Hàng 1 [-11, 15]
                    if (i <= 15) { 
                        lcd_gotoxy(i, 1); // Dòng 1
                        lcd_putsf(message);
                    }
                    
                    // 2. Hi?n th? ph?n trên Hàng 0 (B?t d?u t? khi i < 0)
                    // N?u v? trí b?t d?u trên Hàng 0 (i + 16) n?m trong ph?m vi màn hình [0, 15]
                    if (col_row0 >= 0 && col_row0 <= 15) { 
                        lcd_gotoxy(col_row0, 0); // Dòng 0
                        lcd_putsf(message);
                    }
                    
                    delay_ms(200);
                    
                    check_button();
                    if (mode != 2) { break; } 
                }
            }
            
        } else if (mode == 3) { // Xóa màn hình
            lcd_clear();
            mode = 0; 
        }
    }
}