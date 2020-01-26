//Comp Eng Ess Project
//GROUP 03
#include "FWB.h"

//#define BP 14

#define outputA 5 //RotaryA
#define outputB 4 //RotaryB
#define touch 14

//gameMode Variable
//bool isGameSelect = false; //TRUE if RotarySW is pressed
bool isGameStart = false; //TRUE if Touch is pressed another time
int gameMode = 0;
String gameModeName[2] = {"timer","speed"};

//Rotary Encoder Variable
int counter = 0; 
int aState;
int aLastState; 

//Touch Sensor Variable
//int buttonState = 0;     // current state of the button
//int lastButtonState = 0; // previous state of the button
//int startPressed = 0;    // the time button was pressed
//int endPressed = 0;      // the time button was released
//int timeHold = 0;        // the time button is hold
//int timeReleased = 0;    // the time button is released

//void changeGameMode(){
//  if (digitalRead(touch)==HIGH){
//    Serial.println("TOUCHED");
//    gameMode = (gameMode+1)%2;
//    Serial.print("Gamemode : ");
//    Serial.println(gameModeName[gameMode]);
//  }
//  delay(300);
//}

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

//void checkStart(){
//  if (digitalRead(touch)==HIGH){
//    isGameStart = true;
//  }
//}

FWB bp;
void OnClick(int pin){ //Change Gamemode
  gameMode = (gameMode+1)%2;
  Serial.print("Gamemode : ");
  Serial.println(gameModeName[gameMode]);
}
void OnLongPress(int pin){//Starting Game
  isGameStart = true;
  Serial.println("GAME is STARTING...");
  Serial.println("GAME START!!!");
}

//-------------------MAIN-----------------------------
void setup() {
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode (touch,INPUT);
  
  Serial.begin(9600);

  aLastState = digitalRead(outputA);

  bp.Configure(touch);
  bp.OnClick = OnClick;
  bp.OnLongPress = OnLongPress;
}

bool gameRunning = false;
void loop() {
  if (!isGameStart){//Game isn't start -> Change GM, Start the game
    bp.checkBP();
    setMaxScore();
  }else{
    //Game Start
    switch (gameMode){
      case 0: //timer
        break;
      case 1: //speed
        break;
    }
  }
}
