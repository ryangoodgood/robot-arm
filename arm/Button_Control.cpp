#include<Servo.h>
#include<Arduino.h>

#include "Claw_Grab.h"
#include "Record_Replay.h"

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed;

void Button_Control(byte read){
  if(read==1){
    Claw_A();
    Claw_B();
    Claw_C();
  }else if(read==2){
    Record();
  }else if(read==3){
    Replay();
  }else if(read==4){
    Claw.write(90);
    Right.write(90);
    Left.write(90);
    Base.write(90);
  }
}