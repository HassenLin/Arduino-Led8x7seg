#include "Led8x7seg.h"
int dataIn = 2;
int load = 3;
int clk = 4;

Led8x7seg leds(dataIn, load, clk);


void setup () {
Serial.begin(115200);
while(!Serial);
leds.ShowInt(123456);
delay(3000);  
leds.ShowInt(-123456);
delay(3000);  
leds.ShowFloat(1234.12);
delay(3000);  
leds.ShowFloat(-1234.56);
delay(3000);  

}

void loop () {
 delay(100);

}
