#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<util/delay.h>
#include"gpio.h"
#include"usart.h"

int buffIndex = 0;
char buffer[20];
char LED7segArray[20];

void sendBitPort(int value)
{
		setOutValue(B,4,value);
		setOutH(B,6);
		setOutL(B,6);
		setOutValue(B,4,0);
}
void dispDigit(int digit)
{
	const char digits[10] = {
		0b11101011, // 0
		0b00101000, // 1
		0b10110011, // 2
		0b10111010, // 3
		0b01111000, // 4
		0b11011010, // 5
		0b11011011, // 6
		0b11101000, // 7
		0b11111011, // 8
		0b11111010, // 9
	};

	int j = 0;
	setOutL(B,5);
	for(j=0;j<8;j++){
		sendBitPort(0b00000001 & ((digits[digit]) >> (7 - j)));
	}
	setOutH(B,5);
}
int dig2int(char digit)
{
	if('0' <= digit && digit <= '9')return digit - '0';
	else return 0;
}
/* The one's digit is B0.
 * The ten's digit is B1.
 * The hundred's digit is B2.
 * The thousand's digit is B3.
 * SER/DS is D2.
 * RCK/STCP is D3.
 * SCK/SHCP is D4.
 */
int main(void)
{
	USARTinit(1200);
	LED7segArray[0] = '9';
	LED7segArray[1] = '0';
	LED7segArray[2] = '4';
	LED7segArray[3] = '7';
//GPIO initilize
	setOutput(B,0);
	setOutput(B,1);
	setOutput(B,2);
	setOutput(B,3);

	setOutput(B,4); //SER/DS シリアルデータ入力
	setOutput(B,5); //RCK/STCP 立ち上がり時に出力
	setOutput(B,6); //SCK/SHCP 立ち上がり時のデータを記録

	setOutL(B,4);
	setOutL(B,5);
	setOutL(B,6);

	setOutH(B,0);
	setOutH(B,1);
	setOutH(B,2);
	setOutH(B,3);

	sei();
	while(1){
			showData(LED7segArray);
			int i = 0;
			for(i = 0;i<4;i++){
				dispDigit(dig2int(LED7segArray[i]));
				setOutL(B,i);
				_delay_ms(1);
				setOutH(B,i);
			}
	}
}
