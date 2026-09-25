#include <Adafruit_GFX.h>
  #include <Adafruit_GrayOLED.h>
  #include <Adafruit_SPITFT.h>
  #include <Adafruit_SPITFT_Macros.h>
  #include <gfxfont.h>
  #include <SPI.h>
  #include <Adafruit_ILI9341.h>
  #include "SPI.h"
  #include "Adafruit_GFX.h"
  #include "Adafruit_ILI9341.h"
#define TFT_CLK 13
  #define TFT_MISO 12
  #define TFT_MOSI 11
  #define TFT_DC 10
  #define TFT_CS 8
  #define TFT_RST 9
#define B0 31   //音符
  #define C1 33
  #define CS1 35
  #define D1 37
  #define DS1 39
  #define E1 41
  #define F1 44
  #define FS1 46
  #define G1 49
  #define GS1 52
  #define A1 55
  #define AS1 58
  #define B1 62
  #define C2 65
  #define CS2 69
  #define D2 73
  #define DS2 78
  #define E2 82
  #define F2 87
  #define FS2 93
  #define G2 98
  #define GS2 104
  #define A2 110
  #define AS2 117
  #define B2 123
  #define C3 131
  #define CS3 139
  #define D3 147
  #define DS3 156
  #define E3 165
  #define F3 175
  #define FS3 185
  #define G3 196
  #define GS3 208
  #define A3 220
  #define AS3 233
  #define B3 247
  #define C4 262
  #define CS4 277
  #define D4 294
  #define DS4 311
  #define E4  330
  #define F4 349
  #define FS4 370
  #define G4 392
  #define GS4 415
  #define A4 440
  #define AS4 466
  #define B4 494
  #define C5 523
  #define CS5 554
  #define D5 587
  #define DS5 622
  #define E5 659
  #define F5 698
  #define FS5 740
  #define G5 784
  #define GS5 831
  #define A5 880
  #define AS5 932
  #define B5 988
  #define C6 1047
  #define CS6 1109
  #define D6 1175
  #define DS6 1245
  #define E6 1319
  #define F6 1397
  #define FS6 1480
  #define G6 1568
  #define GS6 1661
  #define A6 1760
  #define AS6 1865
  #define B6 1976
  #define C7 2093
  #define CS7 2217
  #define D7 2349
  #define DS7 2489
  #define E7 2637
  #define F7 2794
  #define FS7 2960
  #define G7 3136
  #define GS7 3322
  #define A7 3520
  #define AS7 3729
  #define B7 3951
  #define C8 4186
  #define CS8 4435
  #define D8 4699
  #define DS8 4978
const int debounceDelay= 10 ;
const int buttonAPin = 2; //將A鍵的Pin設在2
const int buttonBPin = 3; //將B鍵的Pin設在3
const int buttonCPin = 4; //將B鍵的Pin設在4
const int speakerPin = 6;
volatile boolean buttonAState = HIGH;  //宣告A鍵狀態變數，因為是在中斷程式中被更新的變數，所以在前面加上volatile
volatile boolean buttonBState = HIGH;  //宣告B鍵狀態變數，因為是在中斷程式中被更新的變數，所以在前面加上volatile
volatile boolean buttonCState = HIGH;  //宣告B鍵狀態變數，因為是在中斷程式中被更新的變數，所以在前面加上volatile

int BlueShowTime;   //計算藍色音符的出現時間
int RedShowTime;    //計算紅色音符的出現時間
int BlueScore;   //計算藍色分數
int RedScore;    //計算紅色分數


bool Bluestate=false;  //藍色音符是否有出現的狀態
bool Redstate=false;   //紅色音符是否有出現的狀態

bool note_time[5][80];
int note_time_size[5]={63,47,0,21,31};


int divide=8;
const int speed=80;
int startTime;
bool play;

int states[8];
uint16_t color_states[8];

int j;
int z;
int x;
int one_or_two;
bool menu_or_play; //false->menu  true->play

bool bg=false;


//設定顏色
const uint16_t background = 0x0f58;
const uint16_t text_color = ILI9341_BLACK;
const uint16_t text_bg_color = ILI9341_YELLOW;
const uint16_t note_orange = 0B1111100111100000;
const uint16_t note_blue = 0x0f3a;

//建立LCD class
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


String songs[] = {"IDOL", "Alex F", "B->C", "Nokia Ring", "Little Star"};
int num_songs= sizeof(songs) / sizeof(songs[0]);
int option;
int Option;
int Row_Show=3;  //設定菜單上一次要秀出幾首歌曲
String arrow="=>"; //指向當前被選擇的歌曲

//歌曲旋律和節奏
const short score[5][50] = {
    {E4, G4, A4, E5, A4, G4, A4, E5, A4, G4, A4,E4, G4, A4, C5, B4, G4, A4, B5, C5, D5, F5, E5,A4, E5, A4, G4, A4, E5, A4, G4, A4,E4, G4, A4, C5, B4, G4, A4, A4, G4, A4},
    {E4 ,G4,E4,E4,A4 ,E4, D4,    E4,B4,E4,E4,C5,B4 ,G4,     E4 ,B4, E5, E4,D4, D4,B3, FS4 ,E4 },
    {G3, D4, D4, D4, 00, C4, C4, C4, 00, G3, AS3, C4, AS3,00,
    D4, D4, D4, 00, C4, C4, C4, 00, G3, AS3, F4, D4, 00,
    D4,G4,A4,AS4,A4,00,D4,C4,AS3,F4,F4,D4,D4,
    G3,G3,G3,C4,AS3,00,F4,DS4,D4, },
    {E5, D5, FS4, GS4, CS5, B4, D4, E4, B4, A4, CS4, E4,A4},
    {C4 ,C4 ,G4 ,G4 ,A4 ,A4 ,G4 ,F4 ,F4 ,E4 ,E4 ,D4 ,D4 ,C4}
};

const byte beats[][50] = {
  {1,1,2,2,2,1,2,1,1,1,2,1,1,2,2,2,1,2,1,1,1,2,2,2,2,2,1,2,1,1,1,2,1,1,2,2,2,1,2,2,1,1},
  {4,3,2,1,1,2,2 ,4,3,2,1,1,2,2  ,2,2,2,1,2,1,2,2,4},
  {2, 2, 2, 4, 8, 8, 8, 8, 4, 4, 4, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 8, 8,
    8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 8, 2, 4,
    8, 8, 8, 8, 8, 8, 4, 8, 4,},
  {4,4,2,2,4,4,2,2,4,4,2,2,1},
  {1,1,1,1,1,1,2,1,1,1,1,1,1,2},
};

bool debounce(int pin){  
  bool state;
  bool previousState;
  previousState = digitalRead(pin);  //記錄開關狀態
  for(int counter=0 ; counter<debounceDelay ; counter++){  //設定連續幾次檢查，狀態都不變
    delay(1);   //等1毫秒
    state = digitalRead(pin);  //讀取
    if(state != previousState){  
      counter = 0;     //如果狀態變化，重設counter
      previousState = state;   //儲存當下狀態
    }
  }
  return state;   //當開關狀態穩定超過debounce period , 即回傳
}
//消除音符
void clearNote( int x) { 
  x=8-x; 
    tft.fillCircle(x*(tft.width()/divide),  tft.height()/2 , 25 , ILI9341_BLACK);
    //tft.drawRect( x*(tft.width()/8) + 20/2, tft.height()/2-60, 11, 60,  ILI9341_BLACK);
}

//顯示音符，可藉由參數來設定為藍色或是紅色
void ShowNote( int x , uint16_t color ){
  x=8-x; 
    tft.drawCircle(x*(tft.width()/divide),  tft.height()/2 , 20 , color);//殼
    tft.fillCircle(x*(tft.width()/divide),  tft.height()/2 , 18 , color);//臉皮
    tft.fillCircle(x*(tft.width()/divide)-10,  tft.height()/2-5 ,  4 , ILI9341_BLACK);//眼睛
    tft.fillCircle(x*(tft.width()/divide)+10,  tft.height()/2-5 ,  4 , ILI9341_BLACK);
    tft.fillCircle(x*(tft.width()/divide),  tft.height()/2+7 ,  6 , ILI9341_RED);//嘴
    tft.fillRect(x*(tft.width()/divide)-5,  tft.height()/2 ,  15 ,4, color);
    //tft.drawRect( x*(tft.width()/8) + 20/2, tft.height()/2-60, 11, 60,  color);
}

//顯示藍色音符的分數
void show_BlueScore(){
  tft.setTextColor(ILI9341_BLUE);
        tft.setCursor( tft.width()/4    , tft.height()/5*4 );
        tft.fillRect(  tft.width()/4*1  , tft.height()/5*4  ,40 , 40, ILI9341_ORANGE);
        tft.setTextSize(3);
        tft.println(String(BlueScore));
}
//顯示紅色音符的分數
void show_RedScore(){
  tft.setTextColor(ILI9341_RED);
        tft.setCursor( tft.width()/4*3 , tft.height()/5*4  );
        tft.fillRect(  tft.width()/4*3 , tft.height()/5*4  , 40 , 40, ILI9341_ORANGE);
        tft.setTextSize(3);
        tft.println(String(RedScore));
}

void showFace(int x,int y,uint16_t color){
            tft.fillCircle(x,  y , 25 , ILI9341_WHITE); //殼
            tft.fillCircle(x,y , 18 , color);//臉皮
            tft.fillCircle(x-10,  y-5 ,  4 , ILI9341_BLACK);//眼睛
            tft.fillCircle(x+10,  y-5 ,  4 , ILI9341_BLACK);
            tft.fillCircle(x,  y+7 ,  6 , ILI9341_RED); //嘴
            tft.fillRect(x-5,  y ,  15 ,4, color);
}


void show_final_score(){
  tft.fillScreen(ILI9341_ORANGE);
  tft.setTextSize(3);
  tft.setCursor(1, tft.height()/3);
  tft.setTextColor(ILI9341_YELLOW);
  tft.println("   the score is");
  
  if(BlueScore<=9&&RedScore<=9){
    for(int i=0; i<=5; i++){
      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(tft.width()/4, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_RED);
       tft.setCursor(tft.width()*3/4, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
      tft.setTextColor(ILI9341_YELLOW);
      tft.setCursor(tft.width()/4, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_YELLOW);
       tft.setCursor(tft.width()*3/4, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
    }
  }
  else if(BlueScore>=10&&RedScore<=9){
    for(int i=0; i<=5; i++){
      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(tft.width()/4-20, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_RED);
       tft.setCursor(tft.width()*3/4, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
      tft.setTextColor(ILI9341_YELLOW);
      tft.setCursor(tft.width()/4-20, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_YELLOW);
       tft.setCursor(tft.width()*3/4, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
    }
  }
  else if(BlueScore<=9&&RedScore>=10){
    for(int i=0; i<=5; i++){
      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(tft.width()/4, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_RED);
       tft.setCursor(tft.width()*3/4-20, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
      tft.setTextColor(ILI9341_YELLOW);
      tft.setCursor(tft.width()/4, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_YELLOW);
       tft.setCursor(tft.width()*3/4-20, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
    }
  }
  else {
  for(int i=0; i<=5; i++){
    tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(tft.width()/4-20, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_RED);
       tft.setCursor(tft.width()*3/4-20, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
      tft.setTextColor(ILI9341_YELLOW);
      tft.setCursor(tft.width()/4-20, tft.height()*2/3);
      tft.setTextSize(5);
      tft.print(BlueScore);
      tft.setTextColor(ILI9341_YELLOW);
       tft.setCursor(tft.width()*3/4-20, tft.height()*2/3);
      tft.print(RedScore);
      delay(300);
   }
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(buttonAPin,INPUT);
  pinMode(buttonBPin,INPUT);
  pinMode(buttonCPin,INPUT);
  pinMode(speakerPin,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(buttonAPin),Apress,FALLING); //buttonAPin為負緣觸發，並執行Apress()
  attachInterrupt(digitalPinToInterrupt(buttonBPin),Bpress,FALLING); //buttonBPin為負緣觸發，並執行Bpress()
  attachInterrupt(digitalPinToInterrupt(buttonCPin),Cpress,FALLING); //buttonCPin為負緣觸發，並執行Cpress()
  Serial.println("ILI9341 Test!"); 
  tft.begin();//若輸出皆為0x0，請RESET arduino並確認接線是否有誤
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);

  tft.setRotation(3);

  play=true;
  option=102;
  z=1;
  j=0;
  bg=false;
  menu_or_play=false;
    BlueShowTime=0;   //計算藍色音符的出現時間
    RedShowTime=0;    //計算紅色音符的出現時間
    BlueScore=0;   //計算藍色分數
    RedScore=0;    //計算紅色分數


    //秀出菜單
tft.fillScreen(background);
  showFace(tft.width()/8  ,tft.height()/6  , ILI9341_ORANGE);
  showFace(tft.width()/8*7,tft.height()/6  , ILI9341_PURPLE);
  showFace(tft.width()/8  ,tft.height()/6*5, ILI9341_GREEN);
  showFace(tft.width()/8*7,tft.height()/6*5, 0xea13);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);


  tft.setCursor(0, (tft.height() / 2) + (2-(num_songs-1)/2) * 24);
  tft.print("=>");

  for (int i = 0 , j = option-(Row_Show/2) ; i < Row_Show ,j <= option+(Row_Show/2) ; i++ ,j++ ) {
              int textWidth = songs[j%5].length() * 6 * 2; // 文本的宽度，每个字符占12像素
              int x = (tft.width() - textWidth) / 2; //使文本置中
              int y = (tft.height() / 2) + (i-(Row_Show-1)/2) * 24; // 設定Y軸座標，讓每首歌曲都出現在適當的位置
              tft.setCursor(x, y);
              tft.println(songs[j%5]);    //秀出
 
            }
  

  states[0]=true;
  for(int i=1 ; i<=7 ; i++){
     states[i]=false;  
  }

   one_or_two=random(0,100);
  if(one_or_two%2==0){color_states[0]=note_blue;}
  else               {color_states[0]=note_orange; }

note_time[0][0]= true;
        note_time[0][1]= true;
        note_time[0][2]= true;
        note_time[0][3]= false;
        note_time[0][4]= true;
        note_time[0][5]= false;
        note_time[0][6]= true;
        note_time[0][7]= false;
        note_time[0][8]= true;
        note_time[0][9]= true;
        note_time[0][10]= false;
        note_time[0][11]= true;
        note_time[0][12]= true;
        note_time[0][13]= true;
        note_time[0][14]= true;
        note_time[0][15]= false;
        note_time[0][16]= true;
        note_time[0][17]= true;
        note_time[0][18]= true;
        note_time[0][19]= false;
        note_time[0][20]= true;
        note_time[0][21]= false;
        note_time[0][22]= true;
        note_time[0][23]= false;
        note_time[0][24]= true;
        note_time[0][25]= true;
        note_time[0][26]= false;
        note_time[0][27]= true;
        note_time[0][28]= true;
        note_time[0][29]= true;
        note_time[0][30]= true;
        note_time[0][31]= false;
        note_time[0][32]= true;
        note_time[0][33]= false;
        note_time[0][34]= true;
        note_time[0][35]= false;
        note_time[0][36]= true;
        note_time[0][37]= false;
        note_time[0][38]= true;
        note_time[0][39]= false;
        note_time[0][40]= true;
        note_time[0][41]= true;
        note_time[0][42]= false;
        note_time[0][43]= true;
        note_time[0][44]= true;
        note_time[0][45]= true;
        note_time[0][46]= true;
        note_time[0][47]= false;
        note_time[0][48]= true;
        note_time[0][49]= true;
        note_time[0][50]= true;
        note_time[0][51]= false;
        note_time[0][52]= true;
        note_time[0][53]= false;
        note_time[0][54]= true;
        note_time[0][55]= false;
        note_time[0][56]= true;
        note_time[0][57]= true;
        note_time[0][58]= false;
        note_time[0][59]= true;
        note_time[0][60]= false;
        note_time[0][61]= true;
        note_time[0][62]= true;
note_time[1][0]= true;
    note_time[1][1]= false;
    note_time[1][2]= false;
    note_time[1][3]= false;
    note_time[1][4]= true;
    note_time[1][5]= false;
    note_time[1][6]= false;
    note_time[1][7]= true;
    note_time[1][8]= false;
    note_time[1][9]= true;
    note_time[1][10]= true;
    note_time[1][11]= true;
    note_time[1][12]= false;
    note_time[1][13]= true;
    note_time[1][14]= false;
    note_time[1][15]= true;
    note_time[1][16]= false;
    note_time[1][17]= false;
    note_time[1][18]= false;
    note_time[1][19]= true;
    note_time[1][20]= false;
    note_time[1][21]= false;
    note_time[1][22]= true;
    note_time[1][23]= false;
    note_time[1][24]= true;
    note_time[1][25]= true;
    note_time[1][26]= true;
    note_time[1][27]= false;
    note_time[1][28]= true;
    note_time[1][29]= false;
    note_time[1][30]= true;
    note_time[1][31]= false;
    note_time[1][32]= true;
    note_time[1][33]= false;
    note_time[1][34]= true;
    note_time[1][35]= false;
    note_time[1][36]= true;
    note_time[1][37]= true;
    note_time[1][38]= false;
    note_time[1][39]= true;
    note_time[1][40]= true;
    note_time[1][41]= false;
    note_time[1][42]= true;
    note_time[1][43]= false;
    note_time[1][44]= true;
    note_time[1][45]= false;
    note_time[1][46]= false;
    note_time[1][47]= false;

  note_time[2][1]= false;
  note_time[2][2]= false;
  note_time[2][3]= false;
  note_time[2][4]= true;
  note_time[2][5]= false;
  note_time[2][6]= false;
  note_time[2][7]= true;
  note_time[2][8]= false;
  note_time[2][9]= true;
  note_time[2][10]= true;
  note_time[2][11]= true;
  note_time[2][12]= false;
  note_time[2][13]= true;
  note_time[2][14]= false;
  note_time[2][15]= true;
  note_time[2][16]= false;
  note_time[2][17]= false;
  note_time[2][18]= false;
  note_time[2][19]= true;
  note_time[2][20]= false;
  note_time[2][21]= false;
  note_time[2][22]= true;
  note_time[2][23]= false;
  note_time[2][24]= true;
  note_time[2][25]= true;
  note_time[2][26]= true;
  note_time[2][27]= false;
  note_time[2][28]= true;
  note_time[2][29]= false;
  note_time[2][30]= true;
  note_time[2][31]= false;
  note_time[2][32]= true;
  note_time[2][33]= false;
  note_time[2][34]= true;
  note_time[2][35]= false;
  note_time[2][36]= true;
  note_time[2][37]= true;
  note_time[2][38]= false;
  note_time[2][39]= true;
  note_time[2][40]= true;
  note_time[2][41]= false;
  note_time[2][42]= true;
  note_time[2][43]= false;
  note_time[2][44]= true;
  note_time[2][45]= false;
  note_time[2][46]= false;
  note_time[2][47]= false;


note_time[3][0]= true;
  note_time[3][1]= false;
  note_time[3][2]= true;
  note_time[3][3]= false;
  note_time[3][4]= true;
  note_time[3][5]= true;
  note_time[3][6]= true;
  note_time[3][7]= false;
  note_time[3][8]= true;
  note_time[3][9]= false;
  note_time[3][10]= true;
  note_time[3][11]= true;
  note_time[3][12]= true;
  note_time[3][13]= false;
  note_time[3][14]= true;
  note_time[3][15]= false;
  note_time[3][16]= true;
  note_time[3][17]= true;
  note_time[3][18]= true;
  note_time[3][19]= false;
  note_time[3][20]= false;
  note_time[3][21]= false;

note_time[4][0]= true;
  note_time[4][1]= false;
  note_time[4][2]= true;
  note_time[4][3]= false;
  note_time[4][4]= true;
  note_time[4][5]= false;
  note_time[4][6]= true;
  note_time[4][7]= false;
  note_time[4][8]= true;
  note_time[4][9]= false;
  note_time[4][10]= true;
  note_time[4][11]= false;
  note_time[4][12]= true;
  note_time[4][13]= false;
  note_time[4][14]= false;
  note_time[4][15]= false;
  note_time[4][16]= true;
  note_time[4][17]= false;
  note_time[4][18]= true;
  note_time[4][19]= false;
  note_time[4][20]= true;
  note_time[4][21]= false;
  note_time[4][22]= true;
  note_time[4][23]= false;
  note_time[4][24]= true;
  note_time[4][25]= false;
  note_time[4][26]= true;
  note_time[4][27]= false;
  note_time[4][28]= true;
  note_time[4][29]= false;
  note_time[4][30]= false;
  note_time[4][31]= false;


      /*
      x=0;
      note_time[0]=true;beatsCount[x]--;
          for(int i=1; i<=63; i++){
        if(beatsCount[x]!=0){
          beatsCount[x]--;
          note_time[i]=false;
        }
        else if (beatsCount[x]==0){
          x++;
          note_time[i]=true;
          beatsCount[x]--;
        }
      }
      */
}

int num=3;
void loop() {

  if(menu_or_play==false){
      
      //歌單上移，選項也上移
      if(num==1){
          option--;
          for (int i = 0 , j = option-(Row_Show/2) ; i < Row_Show ,j <= option+(Row_Show/2) ; i++ ,j++ ) {
              int textWidth = songs[j%5].length() * 6 * 2; // 文本的宽度，每个字符占12像素
              int x = (tft.width() - textWidth) / 2; //使文本置中
              int y = (tft.height() / 2) + (i-(Row_Show-1)/2) * 24; // 設定Y軸座標，讓每首歌曲都出現在適當的位置
              tft.fillRect(arrow.length()*6*2, y,500,20,background); //先清除
              tft.setCursor(x, y);
              tft.println(songs[j%5]);    //再秀出
            }
           num=3;
      }

      //歌單下移，選項也下移
       if(num==0){  
          option++;
          for (int i = 0 , j = option-(Row_Show/2) ; i < Row_Show ,j <= option+(Row_Show/2) ; i++ ,j++ ) {
              int textWidth = songs[j%5].length() * 6 * 2; // 文本的宽度，每个字符占12像素
              int x = (tft.width() - textWidth) / 2; //使文本置中
              int y = (tft.height() / 2) + (i-(Row_Show-1)/2) * 24; // 設定Y軸座標，讓每首歌曲都出現在適當的位置
              tft.fillRect(arrow.length()*6*2, y,500,20,background); //先清除
              tft.setCursor(x, y);
              tft.println(songs[j%5]);    //再秀出
            }
            num=3;
      }


      //輸入2開始遊戲
      if(num==2){
         menu_or_play=true;
         tft.fillScreen(background);
         num=3;
         startTime=millis();
         Option=option%5;
      }
  }





  else {//玩遊戲
    if(bg==false){
            tft.fillScreen(ILI9341_BLACK);
            tft.fillRect(0,  tft.height()/2 -36,  tft.width() ,8, ILI9341_WHITE);//輸出白線
            tft.fillRect(0,  tft.height()/2 +33,  tft.width() ,8, ILI9341_WHITE);

            tft.fillRect(0,  0,  tft.width() ,tft.height()/2 -36, ILI9341_RED);//輸出背景
            tft.fillRect(0,  tft.height()/2 +41,  tft.width() ,tft.height()-tft.height()/2 -41, ILI9341_ORANGE);

            showFace(50,50,note_orange);//臉
            bg=true;
          }

    tft.setTextSize(1);
 tft.drawCircle( tft.width()/divide,  tft.height()/2 , 25 , ILI9341_YELLOW);//得分框框

  buttonAState = debounce(buttonAPin); //讀取A鍵狀態
  buttonBState = debounce(buttonBPin); //讀取B鍵狀態
  buttonCState = debounce(buttonCPin); //讀取C鍵狀態

  int duration=millis()-startTime;
  if(duration >= speed  ){//每過一個周期就要讓音符重新設定  
        //位移音符譜
        for(int i=7;i>=1;i--){ 
          states[i]=states[i-1];
          color_states[i]=color_states[i-1];
        }
    
        for(int i=7;i>=1;i--){ 
          if(states[i]==false){clearNote(i);}
        }
        for(int i=7;i>=1;i--){ 
          if(states[i]==true){ShowNote(i,color_states[i]);}
        }

        if(states[7]==true){tone(speakerPin,score[Option][j]);j++;}//音符到達得分框框時，會彈奏對應的音高

        //第一個位置另外處理
        if(note_time[Option][z]==false ){
           states[0]=false;
           clearNote(0);
        }
        else  if(note_time[Option][z]==true){
            states[0]=true;
            one_or_two=random(0, 100);
            if(random(0, 100) % 2 == 0){color_states[0]=note_blue; }
            else                       {color_states[0]=note_orange; }
            ShowNote(0,color_states[0]);
        }
        z++;
        startTime=millis();
  }
  if(buttonAState==LOW && color_states[7]==ILI9341_BLUE && states[7]==true ){ //如果藍色音符到得分框框且又按下A鍵
        BlueScore++;
        show_BlueScore();
        Bluestate=false;
   }

   if (buttonBState==LOW && color_states[7]==ILI9341_RED && states[7]==true ){ //如果紅色音符到得分框框且又按下B鍵
        RedScore++;
        show_RedScore();
        Redstate=false;
   }


   show_BlueScore();
   show_RedScore();
   if( z>=note_time_size[Option]+8 ){noTone(6);delay(1000);show_final_score();setup();}
  }
}

void Apress() {    //按下A鍵時，執行此中斷處理程式，讓buttonAState為LOW，再回loop中進行判斷
     buttonAState=LOW;
}
void Bpress() {    //按下B鍵時，執行此中斷處理程式，讓buttonBState為LOW，再回loop中進行判斷
    buttonBState=LOW;
}
void Cpress() {    //按下C鍵時，執行此中斷處理程式，讓buttonCState為LOW，再回loop中進行判斷
    buttonCState=LOW;
}









/* 這塊不用理他
//顯示BlueShowTime字串和次數
void show_BlueShowTime(){
  tft.setTextColor(ILI9341_BLUE);
            tft.setCursor(0,250);
            tft.println("BlueShowTime:");
            tft.setCursor( BlueShowTimeStr.length()*1*6+2 , 250 );
            tft.fillRect(BlueShowTimeStr.length()*1*6+2 , 250 ,15, 15, ILI9341_BLACK);
              tft.println(String(BlueShowTime));
}
void sshow_BlueScore(){
  tft.setTextColor(ILI9341_BLUE);
            tft.setCursor(0,270);
            tft.print("BlueScore:");
            tft.setCursor( BlueScoreStr.length()*1*6+2 , 270 );
            tft.fillRect(BlueScoreStr.length()*1*6+2 , 270 ,15, 15, ILI9341_BLACK);
            tft.println(String(BlueScore));
}
//顯示RedShowTime字串和次數
void show_RedShowTime(){
  tft.setTextColor(ILI9341_RED);
        tft.setCursor(tft.width()-RedShowTimeStr.length()*1*6-30,250);
        tft.print("RedShowTime:");
        tft.setCursor( tft.width()-28 , 250 );
        tft.fillRect(tft.width()-28 , 250 ,15, 15, ILI9341_BLACK);
        tft.println(String(RedShowTime));
}
void sshow_RedScore(){
  tft.setTextColor(ILI9341_RED);
        tft.setCursor(tft.width()-RedShowTimeStr.length()*1*6-30,270);
        tft.print("RedScore:");
        tft.setCursor( tft.width()-28 , 270 );
        tft.fillRect(tft.width()-28 , 270 ,15, 15, ILI9341_BLACK);
        tft.println(String(RedScore));
}


String BlueShowTimeStr="BlueShowTime:";   
String BlueScoreStr="BlueScore:";
String RedShowTimeStr="RedShowTime:";
String RedScoreStr="RedScore:";


//得分效果
void effect(uint16_t color){   
  for(int i=27; i<=30 ; i++){
    tft.drawCircle( tft.width()/divide,  tft.height()/2 , i , color);
  }     
}

//消掉得分效果
void clear_effect(){
  for(int i=27; i<=30 ; i++){
    tft.drawCircle( tft.width()/divide,  tft.height()/2 , i , ILI9341_BLACK);
  }     
}
*/