#include<Servo.h>
#include<Arduino.h>

#include "Claw_Grab.h"
#include "Record_Replay.h"

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed;

byte mode=0;

void Button_Control(byte read){
  switch(read){
    case 1:
      switch(mode){
        case 0:
          Claw_A();
          break;

        case 1:
          Claw_B();
          break;

        case 2:
          Claw_C();
          break;
      }
      if(++mode==3){
        mode=0;
      }
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
