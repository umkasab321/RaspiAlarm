#include "usart.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<avr/interrupt.h>
char rawData[BUFF_NUMBER];
char buff[BUFF_NUMBER];
char sendbuff[BUFF_NUMBER];
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
void putInteger(unsigned long value){
	char valueString[20];
	putString(itoa(value,valueString,10));
}
ISR(USART_RX_vect)
{
	char ch = UDR;
	putChar('R');
	/* if(ch == '\r')putChar('R'); */
	/* else putChar(ch); */

	if(ch == '\r' || ch == '\n' || ch == 'a'){
		strcpy(buff,rawData);
		/* buffClear(); */
		int i = 0;
		for(i = 0;i < BUFF_NUMBER;i++){
			rawData[i] = 0;
		}
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
/* void buffClear() */
/* { */
/* 	#<{(| int i = 0; |)}># */
/* 	#<{(| for(i = 0;i < BUFF_NUMBER;i++){ |)}># */
/* 	#<{(| 	rawData[i] = 0; |)}># */
/* 	#<{(| } |)}># */
/* 	rawData[0] = 0; */
/* 	rawData[1] = 0; */
/* 	rawData[2] = 0; */
/* 	rawData[3] = 0; */
/* 	rawData[4] = 0; */
/* 	#<{(| rawData[5] = 0; |)}># */
/* 	#<{(| rawData[6] = 0; |)}># */
/* 	#<{(| rawData[7] = 0; |)}># */
/* 	#<{(| rawData[8] = 0; |)}># */
/* 	#<{(| rawData[9] = 0; |)}># */
/* } */
