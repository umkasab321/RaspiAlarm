#define t2313
#include <avr/io.h>
#include "gpio.h"

void setOutput(int port,int pin)
{
	if(port == 0){
//		DDRA |= (1 << pin);
	}else if(port == 1){
		DDRB |= (1 << pin);
	}else if(port == 2){
#ifndef t2313
		DDRC |= (1 << pin);
#endif
	}else if(port == 3){
		DDRD |= (1 << pin);
	}
}

void setInput(int port,int pin)
{
	if(port == 0){
//		DDRA |= (1 << pin);
	}else if(port == 1){
		DDRB &= ~(1 << pin);
	}else if(port == 2){
#ifndef t2313
		DDRC &= ~(1 << pin);
#endif
	}else if(port == 3){
		DDRD &= ~(1 << pin);
	}
}
void setOutValue(int port,int pin,int value)
{
	if(value){
		setOutH(port,pin);
	}else{
		setOutL(port,pin);
	}
}
void setOutH(int port,int pin)
{
	if(port == 0){
//		PORTA |= (1 << pin);
	}else if(port == 1){
		PORTB |= (1 << pin);
	}else if(port == 2){
#ifndef t2313
		PORTC |= (1 << pin);
#endif
	}else if(port == 3){
		PORTD |= (1 << pin);
	}
}

void setOutL(int port,int pin)
{
	if(port == 0){
//		DDRA |= (1 << pin);
	}else if(port == 1){
		PORTB &= ~(1 << pin);
	}else if(port == 2){
#ifndef t2313
		PORTC &= ~(1 << pin);
#endif
	}else if(port == 3){
		PORTD &= ~(1 << pin);
	}
}

inline void setPullUp(int port,int pin)
{
	return setOutH(port,pin);
}

uint8_t showInput(int port,int pin)
{
	switch(port){
		case 1:
			if(~PINB & (1 << pin)){
				return 0;
			}
			break;
#ifndef t2313
		case 2:
			if(~PINC & (1 << pin)){
				return 0;
			}
			break;
#endif
		case 3:
			if(~PIND & (1 << pin)){
				return 0;
			}
			break;
	}
	return 1;
}
