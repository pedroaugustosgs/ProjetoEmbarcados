#include <xc.h>
#include "atraso.h"
#include "bits.h"
#include "config.h"
#include "io.h"
#include "keypad.h"
#include "lcd.h"
#include "ssd.h"
#include "pic18f4520.h"
#include "menu.h"
//#include "lerTecla.h"
#include "serial.h"
#include "pegaSerial.h"
//#include "imprimir.h"
//#include "xerox.h"
#include "pwm.h"
#include "i2c.h"
#include "adc.h"
#include "teclado.h"

#define DISP1 PIN_A2
#define DISP2 PIN_A3
#define DISP3 PIN_A4
#define DISP4 PIN_A5

void itoa(unsigned int val, char* str);

void main(void) {
    //static const char valor[] = {0x6D, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char tecla = 0;
    unsigned char opcao = 0, posicao;
    unsigned int nquant = 0, cont = 1, aux = 1, k, j, etapa = 0;
    char str[6];
    int i;
    lcd_init();
    kpInit();
    ssdInit();
    /*TRISA = 0xC3;
    TRISB = 0x03;
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;*/
    //ADCON1 = 0x06;
    bitClr(INTCON2, 7); //liga pull up
    serial_init();
    pwmInit();

    TRISCbits.TRISC7 = 1; //RX
    TRISCbits.TRISC6 = 0; //TX
    PORTB = 0;

    lcd_cmd(0x40);
    menu();


    
    for (;;) {
        if (opcao != aux) {
            opcao = serial();
            etapa = 0; 
            aux = opcao;
        }

        if (opcao == '0') {
            lcd_cmd(0x40);
            menu();
        }
        if (opcao == '1') {
            if (etapa == 0) {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);          //Primeira etapa
                lcd_str("PREPARANDO...");
                etapa++;
            } else {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("IMPRIMINDO...");       //Segunda etapa
                etapa++;
            }
            if (etapa == 6 || (etapa%5==0 && etapa > 6)) {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("FINALIZADO");
                nquant++;
                
                lcd_cmd(L_L2);
                lcd_str("FOLHAS - "); //Contagem de Folhas
                lcd_cmd(L_L2 + 10);
                lcd_dat(((nquant / 1000) % 10) + 48);
                lcd_dat(((nquant / 100) % 10) + 48);
                lcd_dat(((nquant / 10) % 10) + 48);
                lcd_dat(((nquant / 1) % 10) + 48);
                //aux = 0;
                atraso_ms(5000);
            }
            /// LIGA

            pwmSet1(96);
            PORTCbits.RC0 ^= 1;     //Liga cooler e reles
            PORTEbits.RE0 ^= 1;
            atraso_ms(1000);

            unsigned char contrB = 0x01;
            PORTD = contrB;
            for (k = 0; k < 9; k++) {
                PORTD = contrB << k;
                pwmFrequency(2000);
                pwmSet2(50);            //LEds, buzzer e ssd
                atraso_ms(200);
                pwmSet2(0);
                atraso_ms(800);
            }

            //DESLIGA


            PORTCbits.RC0 = 0;
            atraso_ms(1000);    //desliga reles
            PORTEbits.RE0 = 0;
            atraso_ms(1000);

            
            ssdUpdate();        //update no ssd
            atraso_ms(100);
        }
        if (opcao == '2') {
            if (etapa == 0) {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("PREPARANDO...");
                etapa++;
            } else {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("XEROCANDO...");
                etapa++;
            }
            if (etapa == 6 || (etapa%5==0 && etapa > 6)) {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("FINALIZADO");
                nquant = nquant + 4;
                lcd_cmd(L_L2);              //folhas xerocadas
                lcd_str("FOLHAS - ");
                lcd_cmd(L_L2 + 10);
                lcd_dat(((nquant / 1000) % 10) + 48);
                lcd_dat(((nquant / 100) % 10) + 48);
                lcd_dat(((nquant / 10) % 10) + 48);
                lcd_dat(((nquant / 1) % 10) + 48);
                atraso_ms(3000);
                //aux = 0;
            }
            /// LIGA

            pwmSet1(30);
            PORTCbits.RC0 ^= 1;
            PORTEbits.RE0 ^= 1;
            atraso_ms(1000);

            unsigned char contrB = 0x01;
            PORTD = contrB;
            for (k = 0; k < 9; k++) {
                PORTD = contrB << k;
                pwmFrequency(10000);
                pwmSet2(50);            
                atraso_ms(75);   //bips rapidos
                pwmSet2(0);
                atraso_ms(500);
                
            }

            //DESLIGA


            PORTCbits.RC0 = 0;
            atraso_ms(1000);
            PORTEbits.RE0 = 0;
            atraso_ms(1000);

            ssdUpdate();
            atraso_ms(50);
        }
        if (opcao == '3') {
            if (etapa == 0) {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("PREPARANDO...");
                etapa++;
            } else {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("SCANNEANDO...");
                etapa++;
                nquant = nquant + 20;
                
                lcd_cmd(L_L2);          //Porcentagem de progresso
                lcd_str("STATUS - ");
                lcd_cmd(L_L2 + 10);
                lcd_dat(((nquant / 100) % 10) + 48);
                lcd_dat(((nquant / 10) % 10) + 48);
                lcd_dat(((nquant / 1) % 10) + 48);
                lcd_cmd(L_L2 + 14);
                lcd_str("%");
                //aux = 0;
                atraso_ms(100);
            }
            if (etapa == 6 || (etapa%5==0 && etapa > 6)) {
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("FINALIZADO");
                nquant = 20;
            }
            /// LIGA

            pwmSet1(64);
            PORTCbits.RC0 ^= 1;
            PORTEbits.RE0 ^= 1;
            atraso_ms(1000);
            pwmFrequency(20000);
            pwmSet2(50);
            atraso_ms(100);
            
            unsigned char contrB = 0x01;
            PORTD = contrB;
            for (k = 0; k < 9; k++) {
                PORTD = contrB << k;
                pwmSet2(0);             //pulso de bip
                atraso_ms(100);
            }

            //DESLIGA


            PORTCbits.RC0 = 0;
            atraso_ms(1000);
            PORTEbits.RE0 = 0;
            atraso_ms(1000);

            
            ssdUpdate();
            atraso_ms(25);
        }
    }
}
