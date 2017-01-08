#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<util/delay.h>
#include"gpio.h"
#include"usart.h"

char LED7segArray[10];

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
char int2dig(char digit)
{
	if(0 <= digit && digit <= 9)return digit + '0';
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
	LED7segArray[0] = '5';
	LED7segArray[1] = '1';
	LED7segArray[2] = '4';
	LED7segArray[3] = '8';
//GPIO initilize
	setOutput(B,0);
	setOutput(B,1);
	setOutput(B,2);
	setOutput(B,3);

	setInput(D,6);
	setInput(D,4); //ロータリーエンコーダA相
	setInput(D,5); //ロータリーエンコーダB相
	setPullUp(D,6);
	setPullUp(D,4);
	setPullUp(D,5);

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
	/* char str[] = "hello"; */
	/* putString(str); */
	/* char str[20]; */
	/* sprintf(str,"aaaHello\n"); */
	/* putString(str); */
	/* sprintf(str,"He%delo\r\n",3); */
	/* sprintf(str,"Heelo\r\n"); */
	/* putString(str); */
	/* DEBUG_PRINT("Hello%dWorld!\r\n",3); */
	DEBUG_PRINT("Heeeeeeeeeelo\r\n");
	putInteger(6789);
	int count = 0;
	int D4flg = 0;
	int encCount[5] = {0,0,0,0,0}; //[encCount,D4flg,D5flg,D4Data,D5Data]
	int preData = 0;
	int encData = 0;
	int encFlg= 0;
	int preEncFlg= 0;
	int sameDataCount = 0;
	while(2){
		/* showData(LED7segArray); */
		/* int i = 0; */
		/* for(i = 0;i<4;i++){ */
		/* 	dispDigit(dig2int(LED7segArray[i])); */
		/* 	setOutL(B,i); */
		/* 	setOutH(B,i); */
		/* } */

		int getENCdir = 0;
		int chatterCounter = 0;
		int encDatacpy = 1;
		_delay_ms(5);
		while(chatterCounter < 5){
			encData = !showInput(D,4) * 2 + !showInput(D,5);
			if(encDatacpy == encData)chatterCounter++;
			else chatterCounter--;
			encDatacpy = encData;
		}
		if(preData != encData){
			sameDataCount = 0;
			getENCdir = (preData << 1) ^ encData;
			encFlg = (getENCdir & 0b10) >> 1;
			/* if(encFlg == 0)encFlg = -1; */
			if((getENCdir & 0b10) >> 1)encCount[0]++;
			else encCount[0]--;
			/* if(encFlg > preEncFlg)encCount[0]++; */
			/* else if(encFlg < preEncFlg)encCount[0]--; */
			/* preEncFlg = encFlg; */
		}else{
			if(encData == 0){
				if(encCount[0] % 4 != 0){
					if(encCount[0] > 0) encCount[0] += 4 - (encCount[0] % 4);
					else if(encCount[0] < 0) encCount[0] -= (encCount[0] % 4);
				}
			}
		}
		preData = encData;

		if(count % 100 == 1){
			/* putChar('('); */
			/* putInteger(showInput(D,4)); */
			/* DEBUG_PRINT(","); */
			/* putInteger(showInput(D,5)); */
			/* putChar(')'); */
			/* DEBUG_PRINT("\r\n"); */
			putInteger(encCount[0]);
			DEBUG_PRINT(",");
			putInteger(sameDataCount);
			DEBUG_PRINT("\r\n");
		}
		/* int dataD4 = 0; */
		/* int dataD5 = 0; */
		/* dataD4 = showInput(D,4); */
		/* dataD5 = showInput(D,5); */
		/* if(encCount[1] == 0 && dataD4 == 1 && dataD5 == 0)encCount[0]++; */
		/* if(encCount[1] == 0 && dataD4 == 1 && dataD5 == 1)encCount[0]--; */
		/* encCount[1] = dataD4; */
		count >= 100 ? (count = 0) : (count++);
	}
}
