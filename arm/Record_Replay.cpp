#include<Servo.h>
#include<Arduino.h>

#include "Servo_Move.h"
#include "Joystick_Control.h"

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed; 

byte total_time;

byte Base_State[255];
byte Left_State[255];
byte Right_State[255];
byte Claw_State[255];

void Record_Clear(){
  for(byte i=0;i<255;i++){
    Base_State[i]=90;
    Left_State[i]=90;
    Right_State[i]=90;
    Claw_State[i]=90;
  }
}

void Record(){
  Record_Clear();
  total_time=0;
  Serial.println(F("录制开始"));
  while(total_time<255){
    Joystick_Control();
    Base_State[total_time]=Base.read();
    Left_State[total_time]=Left.read();
    Right_State[total_time]=Right.read();
    Claw_State[total_time]=Claw.read();
    total_time++;
    delay(75);
    if(Serial.available()){
      if(Serial.read()=='2'){
        break;
      }
    }
  }
  Serial.println(F("录制结束"));
}

void Replay(){
  byte i=0;
  Serial.println(F("回放开始"));
  while(i<total_time){
    Base.write(Base_State[i]);
    Left.write(Left_State[i]);
    Right.write(Right_State[i]);
    Claw.write(Claw_State[i]);
    delay(75);
    i++;
  }
  Serial.println(F("回放结束"));
}