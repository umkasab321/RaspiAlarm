#define BUFF_NUMBER 10
//sprintf使ってもプログラムメモリ溢れない時に使う(文字列だけなら平気)
// #define DEBUG_PRINT(fmt) char str[20];sprintf(str,fmt);putString(str);
// #define DEBUG_PRINT(...) char str[20];sprintf(str,__VA_ARGS__);putString(str);
#define DEBUG_PRINT(fmt) {char GKNElgnalj4la[20];sprintf(GKNElgnalj4la,fmt);putString(GKNElgnalj4la);}
void USARTinit(int baudrate);
void showData(char *dataRegister);
// void indexAdjust();
void buffClear();
void putChar (char c);
void putString(char *s);
void putInteger(unsigned long value); //9桁くらい送れるはず
