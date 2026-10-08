#include<Servo.h>
#include<Arduino.h>

#include "Print.h"

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed; 

double x,y,z;
double q1,q2,q3;

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
  Base.write(target);
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
  Left.write(target);
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
  Right.write(target);
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
  Claw.write(target);
}

void XYZ_Move(const int t_x,const int t_y,const int t_z){
  double x0=x, y0=y, z0=z;
  double dist=sqrt((t_x-x0)*(t_x-x0)+(t_y-y0)*(t_y-y0)+(t_z-z0)*(t_z-z0));
  int n=(int)(dist+0.5);                      
  if(n<1)n=1;
  if(n>200)n=200;
  double step_x=(t_x-x0)/n;
  double step_y=(t_y-y0)/n;
  double step_z=(t_z-z0)/n;

  for(int i=1;i<=n;i++){
    x=x0+step_x*i;
    y=y0+step_y*i;
    z=z0+step_z*i;
    sewrite();
    delay(50);
  }

  Serial.print(F("到达 ("));
  Serial.print(t_x); Serial.print(F(","));
  Serial.print(t_y); Serial.print(F(","));
  Serial.print(t_z); Serial.println(F(")"));
  seread();
}
