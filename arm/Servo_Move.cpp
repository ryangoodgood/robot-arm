#include<Servo.h>
#include<Arduino.h>

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed; 

void Base_Move(byte target,byte MoveSpeed){
  byte now=Base.read();
  if(now<target){
    while(now<target){
      now=Base.read();
      Base.write(now+MoveSpeed);
      delay(100);
    }
  }else{
    while(now>target){
      now=Base.read();
      Base.write(now-MoveSpeed);
      delay(100);
    }
  }
}

void Left_Move(byte target,byte MoveSpeed){
  byte now=Left.read();
  if(now<target){
    while(now<target){
      now=Left.read();
      Left.write(now+MoveSpeed);
      delay(100);
    }
  }else{
    while(now>target){
      now=Left.read();
      Left.write(now-MoveSpeed);
      delay(100);
    }
  }
}

void Right_Move(byte target,byte MoveSpeed){
  byte now=Right.read();
  if(now<target){
    while(now<target){
      now=Right.read();
      Right.write(now+MoveSpeed);
      delay(100);
    }
  }else{
    while(now>target){
      now=Right.read();
      Right.write(now-MoveSpeed);
      delay(100);
    }
  }
}

void Claw_Move(byte target,byte MoveSpeed){
  byte now=Claw.read();
  if(now<target){
    while(now<target){
      now=Claw.read();
      Claw.write(now+MoveSpeed);
      delay(100);
    }
  }else{
    while(now>target){
      now=Claw.read();
      Claw.write(now-MoveSpeed);
      delay(100);
    }
  }
}
