#include "usart.h"
#include<string.h>
#include<avr/interrupt.h>
char buff[BUFF_NUMBER];
int lastIndex;
int startDataByte;
int endDataByte;
void USARTinit(int baudrate)
{
	UCSRB = _BV(RXCIE) |_BV(TXEN)|_BV(RXEN);

	UCSRC = 0b00000110;

    UBRRH = 0;
    UBRRL = 51;    //1MHz1200bps

	startDataByte= 0;
	endDataByte= 1;
	lastIndex = 0;
	buff[0] = '\0';
	buff[1] = '\0';
}
ISR(USART_RX_vect)
{
	char ch = UDR;
	if(ch == 'a'){
		startDataByte = endDataByte + 1;
		endDataByte = lastIndex;
		if(startDataByte == endDataByte)startDataByte++;
	}

	buff[lastIndex] = ch;
	lastIndex++;
	if(lastIndex > BUFF_NUMBER - 1)lastIndex -= BUFF_NUMBER;
	if(startDataByte > BUFF_NUMBER - 1)startDataByte -= BUFF_NUMBER;
	if(endDataByte > BUFF_NUMBER - 1)endDataByte -= BUFF_NUMBER;
}
void showData(char *dataRegister)
{
	char Data[BUFF_NUMBER];
	int i = startDataByte;
	int DataIndex = 0;
	while(1){
		Data[DataIndex] = buff[i];
		if(i == endDataByte)break;
		i++;
		DataIndex++;
	}
	strcpy(dataRegister,Data);
}
