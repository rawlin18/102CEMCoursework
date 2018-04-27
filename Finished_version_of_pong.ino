#include <stdarg.h>
#include "LedControl.h"
#include <LiquidCrystal.h>
LedControl lc=LedControl(12,11,10,1);
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);
int xp=0;

// INT CODE FOR BUTTONS/SCORE --------------------------------------------------------------
int ButtonL1=2; // player 1 Left button on pin 2
int ButtonR1=3; // player 1 Right button on pin 3
int ButtonL2=4; // player 2 Left button on pin 4
int ButtonR2=5; // player 2 Right button on pin 5


//All bat parameters are in arrays, where the first parameter is bat 0 and the secodn is bat 1
int batSize[]={3,3};
int batPos[]={0,0};
int height=8; //Height (and width) of the matrix


//Score for player 0 and 1
int score[]={0,0};


//Position of the ball as X,Y
int ballPos[]={3,4};

//Direction of the ball as dX,dY
int ballVec[]={1,1};

//Useful function for formatting strings
void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


int moveBat(int batNum, int dir){
   //moves a bat
   //batNum is 0 or 1, 0 for left bat, 1 for right
   //dir is a number. -1 or 1 will move the bat up or down. Other numbers will be ignored
   //Function returns -1 if the bat hit an edge and couldn't move the full distance
   //   0 on success
   //   1 when given an incorrect value for dir
   if(dir==-1 || dir==1){
     batPos[batNum]+=dir;
     if(batPos[batNum]<0){
       batPos[batNum]=0;
       return -1;
     }
     if(batPos[batNum]+batSize[batNum]>height){
       batPos[batNum]=height-batSize[batNum];  
       return -1;
     }
     return 0;
   }
   return 1;
  
}


//Move the ball one step, bouncing if necessary
//returns -1 unless the ball hits a left or right edge
//If it hits the left-hand edge it returns 0
//If it hits the right-hand edge it returns 1
//If it hits a bat, it returns -1
int ballStep(){
  int next[]={0,0};
  next[0]=ballPos[0]+ballVec[0];
  next[1]=ballPos[1]+ballVec[1];
  int edgeNum=-1;
  
  
  //Check top/bottom
  if(next[1]<0 || next[1]>=height){
    p("Hit top/bottom\n");
    ballVec[1]*=-1;
  }
  //Now check bats...
  if(next[0]==0 && ballPos[1]>=batPos[0] && ballPos[1]<(batPos[0]+batSize[0])){
    ballVec[0]*=-1;
    Serial.println("Hit bat 0");
  }else if(next[0] < 0){
    p("Left edge hit\n");
    ballVec[0]*=-1;
    edgeNum=0;
  }
  if(next[0]==height-1 && ballPos[1]>=batPos[1] && ballPos[1]<batPos[1]+batSize[1]){
    ballVec[0]*=-1;
    Serial.println("Hit bat 1");
  }else if(next[0] >= height){
    p("Right edge hit\n");
    ballVec[0]*=-1;
    edgeNum=1;
  }
   
  ballPos[0]+=ballVec[0];
  ballPos[1]+=ballVec[1];
  return edgeNum;;
}

void drawGame(){
 lc.clearDisplay(0);

 //Bats
 for(int i=0; i<2; i++)
   for(int j=0; j<batSize[i];j++)
     lc.setLed(0,i*(height-1),j+batPos[i],true);
 
  //Ball
  lc.setLed(0,ballPos[0],ballPos[1],true);

}

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  Serial.begin(9600);

  //Input code for buttons -----------------------------------------------------------
  pinMode(ButtonL1,INPUT);
  pinMode(ButtonR1,INPUT);
  pinMode(ButtonL2,INPUT);
  pinMode(ButtonR2,INPUT);
  lcd.begin(16, 2); //width,height
}


void loop(){
  drawGame();
  int result;
  result=ballStep();
  if(result==0)
    score[1]+=1;
  if(result==1)
   score[0]+=1;
  p("Score is [%d,%d]\n",score[0],score[1]);

  //CODE FOR SCOREBOARD -------------------------------------------------------------
    lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("P2:");
  lcd.setCursor(4, 1);
  lcd.print((    score[0]));

   lcd.setCursor(0, 0);
  lcd.print("P1:");
  lcd.setCursor(4, 0);
  lcd.print(( score[1]));
 

  // Actual code for Bat movement -----------------------------------------------------------

if(digitalRead(ButtonR1) == HIGH){
    (moveBat(1, -1));
}
if(digitalRead(ButtonL1) == HIGH){
    moveBat(1, 1);
}
if(digitalRead(ButtonL2) == HIGH){
    moveBat(0,-1);
}
if(digitalRead(ButtonR2) == HIGH){
    moveBat(0, 1);
}
 
  delay(230);

}





