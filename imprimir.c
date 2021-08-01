#include "imprimir.h"
#include "lcd.h"
#include "keypad.h"
#include "bits.h"
#include "atraso.h"
#include "pegaSerial.h"

unsigned char tecl;

unsigned int quant(void) {
    static const char valor[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    //unsigned char tecla;
    unsigned int i, cont;
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("N DE PAGINAS");
    lcd_cmd(L_L2);
    lcd_str("(1)-AUMENTAR");
    lcd_cmd(0x90);
    lcd_str("(2)-DIMINUIR");
    lcd_cmd(0xD0);
    lcd_str("(4)-CONFIRMAR");
    unsigned int a;


    for (;;) {
        kpDebounce();
        if (tecl != kpRead()) {
            tecl = kpRead();
            
            if (bitTst(tecl, 3)) {
                a = 1;
                break;
            }
            if (bitTst(tecl, 7)) {
                a = 2;
                break;
            }
            if (bitTst(tecl, 2)) {
                a = 3;
                break;
            }
        }
    }
    return a;
}