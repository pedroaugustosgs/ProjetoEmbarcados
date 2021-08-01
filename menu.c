#include "lcd.h"
#include "menu.h"

void menu(void){
    //lcd_init();
    lcd_cmd(L_L1);
    
    lcd_str("SELECIONE:");
    lcd_cmd(L_L2);
    
    lcd_str("(1)-IMPRIMIR");
    lcd_cmd(0x90);
    
    lcd_str("(2)-XEROCAR\n");
    lcd_cmd(0xD0);
    
    lcd_str("(3)-SCANNEAR\n");
    
    
}
