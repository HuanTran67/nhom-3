/*
 * bai 6.c
 *
 * Created: 25/11/2025 10:09:05 AM
 * Author: Admin
 */



#include <io.h>     
#include <delay.h> 
#include <alcd.h>  

int  keypad[3][3] = {0,1,2,3,4,5,6,7,8}; 
char pass[5]; // tao mang ki tu luu tru mat khau     
int  idx = 0; //  bien dem so ki tu    
int  attempts = 0; // tao bien dem khi nhap sai mk

// ham hien thi loi nhap mk
void hien_thi_loi_nhap(void)
{
    int k; // bien k
    lcd_clear();  
    lcd_gotoxy(0,0); 
    lcd_puts("Nhap MK:"); // hien thi NHAP MK tai hang 1
    lcd_gotoxy(0,1); // hien thi tai hang 2
    for (k=0; k<idx; k++) lcd_putchar('*'); // hien thi ki tu * tuong ung voi so da nhap
}

// hien thi khi nhap dung mat khau
void dung_mat_khau(void)
{
    lcd_clear(); // xoa toan bo LCD
    lcd_gotoxy(0,0);
    lcd_puts("Dung mat khau!"); // hien thi
    lcd_gotoxy(0,1); 
    lcd_puts("Relay1 ON");  // hien thi
}

// hien thi khi nhap sai mat khau
void sai_mat_khau(void)
{
    lcd_clear();// xoa toan bo LCD
    lcd_gotoxy(0,0); 
    lcd_puts("Sai! NHAP LAI:");  //hien thi
    lcd_gotoxy(0,1); 
    lcd_puts("");  // hien thi mat khau
}

// hien thi khi nhap sai mk qua 3 lan
void khoa_he_thong(void)
{
    lcd_clear();  // xoa toan bo LCD
    lcd_gotoxy(0,0); 
    lcd_puts("Sai 3 lan!"); // hien thi
    lcd_gotoxy(0,1); 
    lcd_puts("Relay2 ON"); // hien thi
}

//
void xu_ly_phim(int value)
{
    int t; 
    if (idx < 4) { // khi chuoi mk chua du 4 ki tu 
        pass[idx] = (char)('0' + value);   // ghi so da nhap vao mang pass(MK)
        idx++;    // so ki tu mk tang them 1

        lcd_gotoxy(idx-1,1);   // in ki tu tai hang 1 cot do
        lcd_putchar('*');      // hien thi gia tri * 
        delay_ms(500);         // chong doi
    }

    if (idx == 4) {    // khi chuoi mk du 4 ki tu
        pass[4] = '\0'; // them ki tu ket thuc 

        if (pass[0]=='8' && pass[1]=='8' && pass[2]=='8' && pass[3]=='8') { //kiem tra mk
            PORTC.3 = 1;      // mat khau dung, relay1 bat
            PORTD.4 = 0;      // LED tat
            dung_mat_khau();  // hien thi ra man hinh
            while(1){}    
        } else {  // mat khau sai
            // LED do nhay 3 lan
            for (t=0; t<3; t++){ 
            PORTD.4 = 1; 
            delay_ms(150); 
            PORTD.4 = 0; 
            delay_ms(150); }

            attempts++; // so lan nhap mk tang them 1
            idx = 0;  // so ki tu mk reset ve 0                     
            pass[0]=pass[1]=pass[2]=pass[3]='\0'; // xoa noi dung 4 ki tu vua nhap
            // kiem tra so lan nhap mk sai
            if (attempts >= 3) { // so lan nhap mk sai >=3 
                PORTC.2 = 1;   //relay 2 bat 
                khoa_he_thong();// hien thi 
                while(1){}  // vong lap mai mai neu dk dung
            } else {  // so lan nhap sai mk <3
                sai_mat_khau();  // hien thi
            }
        }
    }
}

void BUTTON(void)
{
    char a; // tao bien a kiem tra trang thai button
    int i, j;// hang i cot j
for (j=0; j<3; j++) {
        if (j==0) PORTF = 0b11111101;      // C1=0 muc thap (cot 1)
        if (j==1) PORTF = 0b11110111;      // C2=0 muc thap (cot 2)
        if (j==2) PORTF = 0b11011111;      // C3=0 muc thap  (cot 3)

        for (i=0; i<3; i++) {
            if (i==0) {                    // R1 = PF2
                a = PINF & 0x04;   // trang thai chua duoc nhan (=1)  
                // neu trang thai duoc nhan(=0), so duoc nhan tai hang cot do
                if (a != 0x04) { xu_ly_phim(keypad[i][j]); }
            }
            if (i==1) {                    // R2 = PF4
                a = PINF & 0x10;
                if (a != 0x10) { xu_ly_phim(keypad[i][j]); }
            }
            if (i==2) {                    // R3 = PF0
                a = PINF & 0x01;
                if (a != 0x01) { xu_ly_phim(keypad[i][j]); }
            }
        }
    }
}

void main(void)
{
    // LCD
    lcd_init(16);

    DDRF =  0b11101010; // C1,C2,C3 o trang thai output, R1,R2,R3 trang thai input
    PORTF = 0b00010101; // keo R1,R2,R3 pull up len 1

    // LED d? & Relay
    DDRD.4 = 1;  PORTD.4 = 0;              // LED do tat
    DDRC.3 = 1;  PORTC.3 = 0;              // Relay1 tat
    DDRC.2 = 1;  PORTC.2 = 0;              // Relay2 tat

    // ham cho nhap mk
    idx = 0;
    hien_thi_loi_nhap();

    while(1) {
        BUTTON(); // nhap mk
    }
}
