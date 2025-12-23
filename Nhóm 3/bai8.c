/*
 * bai 8.c
 *
 * Created: 27/11/2025 5:02:53 PM
 * Author: Admin
 */

#include <mega128.h>
#include <delay.h>

// ----------------- Hàm t?t t?t c? digit -----------------
void digit_all_off(void)
{
    PORTF =0b00000000; 
}

// ----------------- B?t t?ng digit -----------------
void digit1_on(void) { PORTF |= (1<<0); } // PF0
void digit2_on(void) { PORTF |= (1<<3); } // PF3
void digit3_on(void) { PORTF |= (1<<4); } // PF4
void digit4_on(void) { PORTF |= (1<<6); } // PF6

// ----------------- Các do?n a..g + dp -----------------
void segment_a(unsigned char want_on){
     if (want_on) PORTF &= ~(1<<1);
     else PORTF |= (1<<1); }
void segment_b(unsigned char want_on){
     if (want_on) PORTF &= ~(1<<5); 
     else PORTF |= (1<<5); }
void segment_c(unsigned char want_on){
     if (want_on) PORTA &= ~(1<<7); 
     else PORTA |= (1<<7); }
void segment_d(unsigned char want_on){
     if (want_on) PORTD &= ~(1<<1); 
     else PORTD |= (1<<1); }
void segment_e(unsigned char want_on){
     if (want_on) PORTD &= ~(1<<2); 
     else PORTD |= (1<<2); }
void segment_f(unsigned char want_on){
     if (want_on) PORTF &= ~(1<<2); 
     else PORTF |= (1<<2); }
void segment_g(unsigned char want_on){
     if (want_on) PORTD &= ~(1<<3); 
     else PORTD |= (1<<3); }
void segment_dp(unsigned char want_on){
     if (want_on) PORTF &= ~(1<<7); 
     else PORTF |= (1<<7); }

void segments_all_off(void)
{
    PORTF |= (1<<1); // a
    PORTF |= (1<<5); // b
    PORTA |= (1<<7); // c
    PORTD |= (1<<1); // d
    PORTD |= (1<<2); // e
    PORTF |= (1<<2); // f
    PORTD |= (1<<3); // g
    PORTF |= (1<<7); // dp
}

// ----------------- Hi?n th? 1 s? 0..9 -----------------
void show_one_digit(unsigned char n, unsigned char dp_on)
{
    switch(n)
    {
        case 0: segment_a(1); segment_b(1); segment_c(1); segment_d(1); segment_e(1); segment_f(1); segment_g(0); break;
        case 1: segment_a(0); segment_b(1); segment_c(1); segment_d(0); segment_e(0); segment_f(0); segment_g(0); break;
        case 2: segment_a(1); segment_b(1); segment_c(0); segment_d(1); segment_e(1); segment_f(0); segment_g(1); break;
        case 3: segment_a(1); segment_b(1); segment_c(1); segment_d(1); segment_e(0); segment_f(0); segment_g(1); break;
        case 4: segment_a(0); segment_b(1); segment_c(1); segment_d(0); segment_e(0); segment_f(1); segment_g(1); break;
        case 5: segment_a(1); segment_b(0); segment_c(1); segment_d(1); segment_e(0); segment_f(1); segment_g(1); break;
        case 6: segment_a(1); segment_b(0); segment_c(1); segment_d(1); segment_e(1); segment_f(1); segment_g(1); break;
        case 7: segment_a(1); segment_b(1); segment_c(1); segment_d(0); segment_e(0); segment_f(0); segment_g(0); break;
        case 8: segment_a(1); segment_b(1); segment_c(1); segment_d(1); segment_e(1); segment_f(1); segment_g(1); break;
        default: // 9
            segment_a(1); segment_b(1); segment_c(1); segment_d(1); segment_e(0); segment_f(1); segment_g(1); break;
    }
    if (dp_on) segment_dp(1); else segment_dp(0);
}

// ----------------- Hi?n th? 4 s? -----------------
void show_4digits_for_ms(unsigned char d1, unsigned char d2,
                         unsigned char d3, unsigned char d4,
                         unsigned int total_ms)
{
    unsigned int t;
    for (t=0; t<total_ms; t+=4)
    {
        digit_all_off();
        show_one_digit(d1, 0);
        digit1_on();
        delay_ms(1);

        digit_all_off();
        show_one_digit(d2, 0); // dp t?t
        digit2_on();
        delay_ms(1);

        digit_all_off();
        show_one_digit(d3, 0);
        digit3_on();
        delay_ms(1);

        digit_all_off();
        show_one_digit(d4, 0);
        digit4_on();
        delay_ms(1);
    }
}

// ----------------- CHUONG TRÌNH CHÍNH -----------------
void main(void)
{
    unsigned int value;       // s? nguyên d?m t? 10 -> 99
    unsigned char d1, d2, d3, d4;

    DDRF = 0xFF;   
    DDRA = 0xFF;   
    DDRD = 0xFF;   

    digit_all_off();
    segments_all_off();

    value = 10;   // b?t d?u t? 0010

    while (1)
    {
        d1 = (value/1000)%10;  
        d2 = (value/100)%10;   
        d3 = (value/10)%10;    
        d4 = value%10;         

        show_4digits_for_ms(d1, d2, d3, d4, 500); // 0.5 giây d?i 1 s?

        value++;
        if (value > 99) value = 10;
    }
}
