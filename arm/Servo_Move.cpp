#include<Servo.h>
#include<Arduino.h>

#include "Print.h"

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed; 

extern bool state;

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

void XYZ_Move(const double t_x,const double t_y,const double t_z){
  seread();
  if(!state)return;
  double x0=x, y0=y, z0=z;
  double dist=sqrt((t_x-x0)*(t_x-x0)+(t_y-y0)*(t_y-y0)+(t_z-z0)*(t_z-z0));
  int n=(int)(dist+0.5)*0.5;               
  if(n<1)n=1;
  if(n>200)n=200;  
  double step_x=(t_x-x0)/n;
  double step_y=(t_y-y0)/n;
  double step_z=(t_z-z0)/n;

  for(int i=1;i<=n;i++){
    if(Serial.available()){
      char temp=Serial.read();
      if(temp=='1'){
        temp=0;
        while(temp!='2'){
          if(Serial.available()){
            temp=Serial.read();
            Serial.readStringUntil('\n');
            if(temp=='3'){
              state=0;
              Claw.write(90);
              Right.write(90);
              Left.write(90);
              Base.write(90);
              return;
            }
          }
        }
      }else if(temp=='3'){
        state=0;
        Claw.write(90);
        Right.write(90);
        Left.write(90);
        Base.write(90);
        return;
      }
    }
    x=x0+step_x*i;
    y=y0+step_y*i;
    z=z0+step_z*i;
    sewrite();
    delay(10);
//    Serial.print(F("Base:"));
//    Serial.println(q1);
//    Serial.print(F("Right:"));
//    Serial.println(q2);
//    Serial.print(F("Left:"));
//    Serial.println(q3);
  }

  Serial.print(F("到达 ("));
  Serial.print(t_x); Serial.print(F(","));
  Serial.print(t_y); Serial.print(F(","));
  Serial.print(t_z); Serial.println(F(")"));
  seread();
}
