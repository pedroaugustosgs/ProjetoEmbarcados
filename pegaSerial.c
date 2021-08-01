#include "pegaSerial.h"
#include "serial.h"

unsigned char serial(void){
    unsigned char tmp;
    tmp = serial_rx(10000);
    return tmp;
}
