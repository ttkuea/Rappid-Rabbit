//Comp Eng Ess Project
//GROUP 03
#include "FWB.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define outputA 5 //RotaryA
#define outputB 4 //RotaryB
#define touch 14 //Touch Sensor

#define LED_A 16
#define LED_B 0
#define LED_C 2

#define SW_A 12
#define SW_B 13
#define SW_C 15
//--------Global Variable----------------
//gameMode Variable
bool isGameStart = false; //TRUE if Touch is pressed another time
int gameMode = 0;
String gameModeName[2] = {"timer","speed"};
bool printStart = true;

//Rotary Encoder Variable
int counter = 0; //Use to set Max Score
int aState;
int aLastState; 

// AP Wi-Fi credentials
const char* ssid = "xxxxxxxx";
const char* password = "xxxxxxxxxx";
ESP8266WiFiMulti WiFiMulti;

//LED Variable
//int led_no[3] = [16, 0, 2];

int httpCode;
HTTPClient http;
String payload = "_";
//-------Extra Function-----------------
void setMaxScore(){
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState){     
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) { 
      counter ++;
    } else {
      counter --;
    }
    Serial.print("Max Score: ");
    Serial.println(counter);
    } 
  aLastState = aState;
}

void requestSetting(){
  if (gameMode == 0){
    http.begin("http://128.199.216.159:3113/SETTING?gamemode=timer&gameparam=" + String(counter));
    httpCode = http.GET();
    payload = http.getString();
    Serial.printf("HTPP code : %d\n",httpCode);
    Serial.println(payload);
  }
  if (gameMode == 1){
    http.begin("http://128.199.216.159:3113/SETTING?gamemode=speed&gameparam=" + String(counter));
    httpCode = http.GET();
    payload = http.getString();
    Serial.printf("HTPP code : %d\n",httpCode);
    Serial.println(payload);
  }
  http.end();
  digitalWrite(LED_A,LOW);
  digitalWrite(LED_B,LOW);
  digitalWrite(LED_C,LOW);
}

void requestStart(){
  Serial.println("STARTED");
  http.begin("http://128.199.216.159:3113/START");
  httpCode = http.GET();
  Serial.printf("HTPP code : %d\n",httpCode);
  
  http.end();
}

void requestPress(){
  Serial.println("PRESS");
  http.begin("http://128.199.216.159:3113/PRESS");
  httpCode = http.GET();
  Serial.printf("HTPP code press : %d\n",httpCode);
  http.end();
}


FWB bp;
boolean bpclick = false;
void OnClick(int pin){ //Change Gamemode
  bpclick = true;
  gameMode = (gameMode+1)%2;
  Serial.print("Gamemode : ");
  Serial.println(gameModeName[gameMode]);
  requestSetting();
}
void OnLongPress(int pin){//Starting Game
  bpclick = true;
  isGameStart = true;
  Serial.println("GAME is STARTING...");
}

void toggleLED(int led){ //LED_A = 0, LED_B = 1, LED_C = 2
  digitalWrite(led,HIGH);
}

//-------------------MAIN-----------------------------
void setup() {
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode (touch,INPUT);
  pinMode (LED_A,OUTPUT);
  pinMode (LED_B,OUTPUT);
  pinMode (LED_C,OUTPUT);
  pinMode (SW_A,INPUT);
  pinMode (SW_B,INPUT);
  pinMode (SW_C,INPUT);
  
  Serial.begin(9600);

  aLastState = digitalRead(outputA);

  bp.Configure(touch);
  bp.OnClick = OnClick;
  bp.OnLongPress = OnLongPress;

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("user", "pass");
}

void loop() {  
  if (!isGameStart){//Game isn't start -> Change GM, Start the game
    bp.CheckBP();
    setMaxScore();
  }else{
    //Game Start
    if (printStart){
      Serial.println("Game will start in...");
      for (int x = 3; x > 0; --x){
        Serial.println(x);
        delay(500);
      }
      printStart = false;
      requestStart();
      bpclick = false;
    }
    String seq = payload;
    int x = 0;

    const int LED[] = {0, LED_A, LED_B, LED_C};
    const int SWS[] = {0, SW_A, SW_B, SW_C};
    while (true) {
      bp.CheckBP();
      if (bpclick) {
        isGameStart = false;
        printStart = true;
        bpclick = false;
        requestSetting();
        break;
      }
      char c = seq[x];
      int num = int(c) - '0';

      if (num < 1 || 3 < num) {
        Serial.println("someting is wrong");
        break;
      }
      
      digitalWrite(LED_A, LOW);
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_C, LOW);
      digitalWrite(LED[num], HIGH);
      
      Serial.printf("require %c [%d]\n", c, num);
      
      if(digitalRead(SWS[num]) == HIGH) {
        x += 1;
        requestPress();
      }
      delay(100);
    }
    
  }
}
