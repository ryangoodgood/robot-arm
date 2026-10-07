#include<Servo.h>
#include<Arduino.h>
#include<math.h>

#include "Servo_Move.h"
#include "Joystick_Control.h"

#define H 15
#define L1 70
#define L2 70
#define LEFT_MAX 120

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

double q1,q2,q3;
double p1,p2;
double R,Z,r;

void sewrite(const int &x,const int &y,const int &z){
  q1=atan2((double)y,(double)x)*180.0/M_PI;
  if(q1<0.0||q1>180.0){
    Serial.println(F("方位超出底座范围"));
    return;
  }
  R=hypot((double)x,(double)y);
  Z=z-H;
  r=hypot(R,Z);
  if(r>L1+L2){
    Serial.println(F("超出可达范围"));
    return;
  }
  // 肩部双舵机反向安装: 每个舵机直接决定一个连杆的绝对方位角
  //   a1 = 180 - Right (大臂)      a2 = 270 - Left (小臂)
  double cos_d=(r*r-L1*L1-L2*L2)/(2.0*L1*L2);
  if(cos_d>1.0)cos_d=1.0;
  if(cos_d<-1.0)cos_d=-1.0;
  double dd=acos(cos_d);
  double th=atan2(Z,R);
  int ok=0;
  for(int s=1;s>=-1&&!ok;s-=2){
    double d=dd*(double)s;
    double a1=th-atan2(L2*sin(d),L1+L2*cos(d));
    double a2=a1+d;
    double tr=180.0-a1*180.0/M_PI;
    double tl=90.0+a2*180.0/M_PI;
    if(tr>=0.0&&tr<=180.0&&tl>=0.0&&tl<=LEFT_MAX){
      q2=tr; q3=tl; ok=1;
    }
  }
  if(!ok){
    Serial.println(F("该点无有效关节解"));
    return;
  }
  Serial.print(F("Base:"));
  Serial.println(q1);
  Serial.print(F("Right:"));
  Serial.println(q2);
  Serial.print(F("Left:"));
  Serial.println(q3);
  Base.write(q1);
  Right.write(q2);
  Left.write(q3);
}

void Straight_Draw(){
  int x=80,y=20,z=100;
  delay(500);
  for(int i=0;i<30;i++){
    sewrite(x,y,z);
    x-=2;
    y+=2;
    delay(100);
  }
}

void seread(){
  int a=Base.read(),b=Right.read(),c=Left.read();
  double x,y,z;

  double t1=a*M_PI/180.0;
  double a1=(180.0-b)*M_PI/180.0;
  double a2=(c-90.0)*M_PI/180.0;

  double Rh=L1*cos(a1)+L2*cos(a2);
  double Zh=L1*sin(a1)+L2*sin(a2);
  x=Rh*cos(t1);
  y=Rh*sin(t1);
  z=Zh+H;

  Serial.print(F("x:"));
  Serial.println(x);
  Serial.print(F("y:"));
  Serial.println(y);
  Serial.print(F("z:"));
  Serial.println(z);
}