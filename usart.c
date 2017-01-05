#include "usart.h"
#include<string.h>
#include<avr/interrupt.h>
char rawData[BUFF_NUMBER];
char buff[BUFF_NUMBER];
int lastIndex;
void USARTinit(int baudrate)
{
	UCSRB = _BV(RXCIE) |_BV(TXEN)|_BV(RXEN);

	UCSRC = 0b00000110;

    UBRRH = 0;
    UBRRL = 51;    //1MHz1200bps

	lastIndex = 0;
	buff[0] = 6;
	buff[1] = 7;
	buff[2] = 8;
	buff[3] = 9;
}
void putChar (char c)
{
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
}
void putString(char *s)
{
	while(*s){
		putChar(*s);
		*(s++);
	}
}
ISR(USART_RX_vect)
{
	char ch = UDR;
	/* if(ch == '\r')rs_putc('R'); */
	/* else rs_putc(ch); */

	if(ch == '\r' || ch == '\n'){
		strcpy(buff,rawData);
		buffClear();
		lastIndex = 0;
	}else{
		rawData[lastIndex] = ch;
		lastIndex++;
	}
	indexAdjust();
}
void showData(char *dataRegister)
{
	strcpy(dataRegister,buff);
}
void indexAdjust()
{
	if(lastIndex > BUFF_NUMBER - 1)lastIndex -= BUFF_NUMBER;
}
void buffClear()
{
	int i = 0;
	for(i = 0;i < BUFF_NUMBER;i++){
		rawData[i] = 0;
	}
}
