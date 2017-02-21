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
void init()
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
	setPullUp(D,6);//ロータリーのスイッチ
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


}
int main(void)
{
	init();
	sei();
	DEBUG_PRINT("Heeeeeeeeeelo\r\n");
	int count = 0;
	int mode = 0; //0時計 1アラーム設定h1 2アラーム設定h2 3アラーム設定m1 4アラーム設定m2 5アラーム時刻
	int encCount[5] = {0,0,0,0,0}; //[encCount,D4flg,D5flg,D4Data,D5Data]
	int alarmTime[4] = {0,0,0,0};
	int chikachika[4] = {0,0,0,0};
	int pushSwitchFlg = 0;
	int preData = 0;
	int encData = 0;
	while(2){
		/* if(mode == 0) showData(LED7segArray); */
		/* else if(mode == 1) showData(LED7segArray); */
		if(mode == 0){
			LED7segArray[0] = 5;
			LED7segArray[1] = 1;
			LED7segArray[2] = 6;
			LED7segArray[3] = 3;
		}else if(1 <=mode  && mode <= 5){
			/* LED7segArray[0] = alarmTime[0]; */
			/* LED7segArray[1] = alarmTime[1]; */
			/* LED7segArray[2] = alarmTime[2]; */
			/* LED7segArray[3] = alarmTime[3]; */
		}
		if(1 <=mode  && mode <= 4){
			/* LED7segArray[mode - 1] = alarmTime[mode - 1] + encCount[0] / 4; */
			/* LED7segArray[mode - 1] = 3; */
		}
		int i = 0;
		for(i = 0;i<4;i++){
			LED7segArray[0] = 5;
			LED7segArray[1] = 1;
			LED7segArray[2] = 6;
			LED7segArray[3] = 3;
			dispDigit(dig2int(LED7segArray[i]));
			if(!chikachika[i]){
				setOutH(B,i);
			}
			_delay_ms(1);
			setOutL(B,i);
		}

		int getENCdir = 0;
		int chatterCounter = 0;
		int encDatacpy = 1;
		
		while(chatterCounter < 5){
			encData = !showInput(D,4) * 2 + !showInput(D,5);
			if(encDatacpy == encData)chatterCounter++;
			else chatterCounter--;
			encDatacpy = encData;
		}
		if(preData != encData){
			getENCdir = (preData << 1) ^ encData;
			if((getENCdir & 0b10) >> 1)encCount[0]++;
			else encCount[0]--;
		}else{
			if(encData == 0){
				if(encCount[0] % 4 != 0){
					if(encCount[0] > 0) encCount[0] += 4 - (encCount[0] % 4);
					else if(encCount[0] < 0) encCount[0] -= (encCount[0] % 4);
				}
			}
		}
		preData = encData;

		//modeスイッチ
		if(pushSwitchFlg == 0 && !showInput(D,6))
		{
			pushSwitchFlg = 1;
			mode++;
			encCount[0] = 0;
		}
		if(pushSwitchFlg){
			pushSwitchFlg++;
			if(pushSwitchFlg >= 50)pushSwitchFlg=0;
		}
		mode = mode % 6;

		if(count % 100 == 1){
			putInteger(encCount[0] / 4);
			DEBUG_PRINT(",");
			DEBUG_PRINT("\r\n");
		}
		if(1 <=mode  && mode <= 4){
			int chikaNum = 0;
			for(chikaNum = 0; chikaNum < 4;chikaNum++){
				if(chikaNum == mode - 1 && count % 50 < 25)chikachika[mode - 1]=1;
				else chikachika[chikaNum]=0;
			}
		}else{
			chikachika[0] = 0;
			chikachika[1] = 0;
			chikachika[2] = 0;
			chikachika[3] = 0;
		}
		count >= 100 ? (count = 0) : (count++);
	}
}
