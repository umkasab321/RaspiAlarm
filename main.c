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
		sendBitPort(0b00000001 & ((0b11111111 - digits[digit]) >> (7 - j)));
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
/* ATMEGA168P
 * The one's digit is C0.
 * The ten's digit is C1.
 * The hundred's digit is C2.
 * The thousand's digit is C3.
 * SER/DS is D2.
 * RCK/STCP is D3.
 * SCK/SHCP is D4.
 * ENCORDER A is D5.
 * ENCORDER B is D6.
 * ENCORDER Switch is D7.
 */
int main(void)
{

	{
		USARTinit(1200);
		/* LED7segArray[0] = '5'; */
		/* LED7segArray[1] = '1'; */
		/* LED7segArray[2] = '4'; */
		/* LED7segArray[3] = '8'; */
	//GPIO initilize
		setOutput(D,3);
		setOutput(C,0);
		setOutput(C,1);
		setOutput(C,2);
		setOutput(C,3);

		setInput(D,5); //ロータリーエンコーダA相
		setInput(D,6); //ロータリーエンコーダB相
		setInput(D,7); //ロータリーのスイッチ
		setPullUp(D,5);
		setPullUp(D,6);
		setPullUp(D,7);

		setOutput(D,2); //SER/DS シリアルデータ入力
		setOutput(D,3); //RCK/STCP 立ち上がり時に出力
		setOutput(D,4); //SCK/SHCP 立ち上がり時のデータを記録

		setOutL(D,2);
		setOutL(D,3);
		setOutL(D,4);


		setOutH(C,0);
		setOutH(C,1);
		setOutH(C,2);
		setOutH(C,3);


	}
	/* init(); */
	sei();
	DEBUG_PRINT("Heeeeeeeeeelo\r\n");
	int count = 0;
	int mode = 0; //0時計 1アラーム設定h1 2アラーム設定h2 3アラーム設定m1 4アラーム設定m2 5アラーム時刻
	uint8_t encCount[5] = {0,0,0,0,0}; //[encCount,D4flg,D5flg,D4Data,D5Data]
	uint8_t alarmTime[4] = {0,0,0,0};
	uint8_t chikachika[4] = {0,0,0,0};
	int pushSwitchFlg = 0;
	int preData = 0;
	int encData = 0;
	uint8_t tempAlarmTime = 0;
	while(2){
		/* if(mode == 0) showData(LED7segArray); */
		/* else if(mode == 1) showData(LED7segArray); */
		if(mode == 0){
			/* LED7segArray[0] = '4'; */
			/* LED7segArray[1] = '1'; */
			/* LED7segArray[2] = '5'; */
			/* LED7segArray[3] = '8'; */
			showData(LED7segArray);
			putChar(LED7segArray[0]);
		}else if(1 <=mode  && mode <= 5){
			LED7segArray[0] = '0' + alarmTime[0];
			LED7segArray[1] = '0' + alarmTime[1];
			LED7segArray[2] = '0' + alarmTime[2];
			LED7segArray[3] = '0' + alarmTime[3];
		}
		if(1 <=mode  && mode <= 4){
			int maxDigit[] = {2,9,5,9};
			tempAlarmTime = (alarmTime[mode - 1] + encCount[0] / 4) % (maxDigit[mode - 1] + 1);
			LED7segArray[mode - 1] = '0' + tempAlarmTime;
		}
		int i = 0;
		for(i = 0;i<4;i++){
			dispDigit(dig2int(LED7segArray[i]));
			if(!chikachika[i]){
				setOutH(C,i);
			}
			_delay_ms(1);
			setOutL(C,i);
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

		/* if(count >= 50)setOutL(A, 1); */
		/* else setOutH(A, 0); */
		//modeスイッチ
		if(pushSwitchFlg == 0 && !showInput(D,7))
		{
			pushSwitchFlg = 1;
			if(1 <= mode && mode <= 4){
				alarmTime[mode - 1] = tempAlarmTime;
			}
			tempAlarmTime=0;
			mode++;
			encCount[0] = 0;
		}
		if(pushSwitchFlg){
			pushSwitchFlg++;
			if(pushSwitchFlg >= 50)pushSwitchFlg=0;
		}
		mode = mode % 6;

		if(mode == 5 && count % 100 == 1){
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
