#include<Servo.h>
#include<Arduino.h>
#include<math.h>

#include "Servo_Move.h"
#include "Joystick_Control.h"

#define H 60
#define L1 80
#define L2 80
#define LEFT_MAX 100

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

double R,Z,r;

extern double x,y,z;        
extern double q1,q2,q3;

void sewrite(){
  if(!(x==0&&y==0)){
    q1=atan2(y,x)*180.0/M_PI;
  }
  if(q1<0.0||q1>180.0){
    return;
  }
  R=hypot(x,y);
  Z=z-H;
  r=hypot(R,Z);
  if(r>L1+L2){
    return;
  }

  double cos_d=(r*r-L1*L1-L2*L2)/(2.0*L1*L2);
  if(cos_d>1.0)cos_d=1.0;
  if(cos_d<-1.0)cos_d=-1.0;
  double dd=acos(cos_d);
  double th=atan2(Z,R);
  double a1=th+atan2(L2*sin(dd),L1+L2*cos(dd));
  double a2=a1-dd;
  q2=180.0-a1*180.0/M_PI;
  q3=90.0+a2*180.0/M_PI;
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

void Straight_Draw(){
  // 画一条直线: (80,20,100) -> (22,78,100)
  // 只改 x 和 y, 高度 z=100 保持不变 (两个坐标变化)
  // 实测: Right 91~99, Left 60.0~60.8, 全程可达, 每步约 1mm
  XYZ_Move(37,33,60);
  delay(2000);
  Claw.write(130);

  XYZ_Move(37,33,50);   
  delay(2000);
  
  XYZ_Move(-21,46,50);      // 再直线画出去
  Claw.write(90);
}