/*
 * bai10.c
 *
 * Created: 27/11/2025
 * Author: Admin
 */

#include <io.h>
#include <alcd.h>
#include <delay.h>

// Ð?nh nghia các nút b?m
#define BT1 PINB.2 // Ch?y T->P
#define BT2 PINB.3 // Ch?y P->T
#define BT3 PINB.0 // Xóa màn hình
#define LCD_BT PORTD.7 

// KHAI BÁO CHU?I VÀO B? NH? FLASH
const flash char message[] = "hello world"; 
const unsigned char msg_len = 11;     

// Bi?n mode d? luu tr?ng thái ho?t d?ng
unsigned char mode = 0;

// Hàm ki?m tra và x? lý nút b?m
void check_button(void)
{
    if (PINB.2 == 0) { delay_ms(50); if (PINB.2 == 0) { mode = 1; while (PINB.2 == 0); } }
    if (PINB.3 == 0) { delay_ms(50); if (PINB.3 == 0) { mode = 2; while (PINB.3 == 0); } }
    if (PINB.0 == 0) { delay_ms(50); if (PINB.0 == 0) { mode = 3; while (PINB.0 == 0); } }
}

void main(void)
{
    // Khai báo bi?n c?c b? t?i d?u hàm main()
    // S? d?ng INT cho i d? tránh l?i ép ki?u c?a CodeVision
    int i; 
    signed char k; // Ch? m?c ký t?

    // [Kh?i t?o I/O và LCD]
    DDRB.2 = 0; PORTB.2 = 1; 
    DDRB.3 = 0; PORTB.3 = 1; 
    DDRB.0 = 0; PORTB.0 = 1; 
    DDRD.7 = 1;
    LCD_BT = 1; 
    lcd_init(16);
    lcd_clear();

    while (1)
    {
        check_button(); 

        if (mode == 1) { // Ch?y T->P (Trái qua Ph?i)
            
            while (mode == 1) { 
                
                for (k = 0; k < msg_len; k++) { // L?p qua T?NG ký t?
                    char current_char = message[k];

                    // i ch?y t? -1 (v?a xu?t hi?n) d?n 16 (v?a bi?n m?t)
                    for (i = -1; i <= 16; i++) { 
                        lcd_clear();
                        if (i >= 0 && i < 16) {
                            lcd_gotoxy(i, 0); // Luôn in trên Dòng 0
                            lcd_putchar(current_char); 
                        }
                        delay_ms(200); 
                        check_button();
                        if (mode != 1) { break; } 
                    }
                    if (mode != 1) { break; } 
                }
            }
            
        } else if (mode == 2) { // Ch?y P->T (Ph?i qua Trái)
            
            while (mode == 2) { 
                
                for (k = 0; k < msg_len; k++) { // L?p qua T?NG ký t?
                    char current_char = message[k];
                    
                    // i ch?y t? 16 (v?a xu?t hi?n) d?n -1 (v?a bi?n m?t)
                    for (i = 16; i >= -1; i--) { 
                        lcd_clear();
                        
                        if (i >= 0 && i < 16) {
                            lcd_gotoxy(i, 0); // Luôn in trên Dòng 0
                            lcd_putchar(current_char);
                        }
                        
                        delay_ms(200);
                        check_button();
                        if (mode != 2) { break; } 
                    }
                    if (mode != 2) { break; } 
                }
            }
            
        } else if (mode == 3) { // Xóa màn hình
            lcd_clear();
            mode = 0; 
        }
    }
}