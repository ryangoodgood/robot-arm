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
  switch(read){
    case 1:
      Claw_A();
      Claw_B();
      Claw_C();
      break;

    case 2:
      Record();
      break;

    case 3:
      Replay();
      break;

    case 4:
      Claw.write(90);
      Right.write(90);
      Left.write(90);
      Base.write(90);
      break;

  }
}
