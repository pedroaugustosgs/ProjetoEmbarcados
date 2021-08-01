#include "lerTecla.h"
#include "keypad.h"

unsigned char tecla(void) {
    kpInit();
    unsigned char tecla;
    for (;;) {
        kpDebounce();
        if (tecla != kpRead()) {
            tecla = kpRead();
            
            return tecla;
        }
        
        
    }
}
