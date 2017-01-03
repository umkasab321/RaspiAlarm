#include <avr/io.h>
enum PORT {A,B,C,D};
void initGPIO();
void setInput(int port,int pin);//set Input
void setOutput(int port,int pin); //set Output
void setOutH(int port,int pin);//set High
void setOutL(int port,int pin);//set Low
void setOutValue(int port,int pin,int value);
void setPullUp(int port,int pin);
uint8_t showInput(int port,int pin);
