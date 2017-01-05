#include "usart.h"
#include<string.h>
#include<avr/interrupt.h>
char rawData[BUFF_NUMBER];
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
    /* UBRRL = 12;    //1MHz4800bps */

	startDataByte= 0;
	endDataByte= 0;
	lastIndex = 0;
	buff[0] = 6;
	buff[1] = 7;
	buff[2] = 8;
	buff[3] = 9;
}
void rs_putc (char c)
{
    loop_until_bit_is_set(UCSRA, UDRE); //UDREƒrƒbƒg‚ª1‚É‚È‚é‚Ü‚Å‘Ò‚Â
    UDR = c;
}
ISR(USART_RX_vect)
{
	/* char ch = UDR; */
	/* if(ch == 'a'){ */
	/* 	startDataByte = endDataByte; */
	/* 	endDataByte = lastIndex; */
	/* 	//if(startDataByte == endDataByte)startDataByte++; */
	/* }else{ */
	/* 	buff[lastIndex] = ch; */
	/* 	lastIndex++; */
	/* } */
	/* indexAdjust(); */

	char ch = UDR;
	if(ch == '\r')rs_putc('R');
	else rs_putc(ch);

	/* if(ch == 'b' || ch == '\r' || ch == '\n'){ */
	if(ch == '\r' || ch == '\n'){
	/* if(ch == 'b'){ */
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
	/* char Data[BUFF_NUMBER]; */
	/* Data[0] = 6; */
	/* Data[1] = 7; */
	/* Data[2] = 8; */
	/* Data[3] = 9; */
	/* int i = startDataByte; */
	/* int DataIndex = 0; */
	/* while(1){ */
	/* 	if(i == endDataByte)break; */
	/* 	Data[DataIndex] = buff[i]; */
	/* 	i++; */
	/* 	DataIndex++; */
	/* 	if(i > BUFF_NUMBER - 1)i -= BUFF_NUMBER; */
	/* 	indexAdjust(); */
	/* } */
	/* strcpy(dataRegister,Data); */
	strcpy(dataRegister,buff);
}
void indexAdjust()
{
	if(lastIndex > BUFF_NUMBER - 1)lastIndex -= BUFF_NUMBER;
	if(startDataByte > BUFF_NUMBER - 1)startDataByte -= BUFF_NUMBER;
	if(endDataByte > BUFF_NUMBER - 1)endDataByte -= BUFF_NUMBER;
}
void buffClear()
{
	int i = 0;
	for(i = 0;i < BUFF_NUMBER;i++){
		rawData[i] = 0;
	}
}
