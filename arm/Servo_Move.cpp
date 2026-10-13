#include<Servo.h>
#include<Arduino.h>

#include "Print.h"

#define H 60
#define L1 80
#define L2 80
#define LEFT_MAX 100

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed; 

extern bool state;

double R,Z,r;
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

void sewrite(){
  double p1,p2,p3;
  if(!(x==0&&y==0)){
    q1=atan2(y,x)*180.0/M_PI;
  }
  if(q1<0.0||q1>180.0){
    return;
  }
  R=hypot(x,y);
  Z=abs(z-H);
  r=hypot(R,Z);
  if(r>L1+L2){
    return;
  }

  double c1=(L1*L1+r*r-L2*L2)/(2.0*r*L1);
  if(c1>1.0)c1=1.0;
  if(c1<-1.0)c1=-1.0;
  p1=acos(c1);

  double c3=(L1*L1+L2*L2-r*r)/(2.0*L1*L2);
  if(c3>1.0)c3=1.0;
  if(c3<-1.0)c3=-1.0;
  p3=acos(c3);

  if(z>H){
    p2=atan(Z/R);
  }else if(z==H){
    p2=0;
  }else{
    p2=atan(R/Z)-90.0*M_PI/180.0;
  }
  q2=180.0-(p1+p2)*180.0/M_PI;
  q3=(p1+p2+p3)*180.0/M_PI-90.0;
  if(q2<0.0||q2>180.0||q3<0.0||q3>LEFT_MAX){
    return;
  }
  Base.write(q1);
  Right.write(q2);
  Left.write(q3);
}

void seread(){
  int a=Base.read(),b=Right.read(),c=Left.read();

  double t1=a*M_PI/180.0;
  double a1=(180.0-b)*M_PI/180.0;
  double a2=(c-90.0)*M_PI/180.0;

  double Rh=L1*cos(a1)+L2*cos(a2);
  double Zh=L1*sin(a1)+L2*sin(a2);
  x=Rh*cos(t1);
  y=Rh*sin(t1);
  z=Zh+H;
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
    delay(25);
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