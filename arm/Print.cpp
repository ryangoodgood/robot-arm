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

int point[5][3];              // 示教点坐标(mm), int 省内存
int times[5];

double m[5][3];               // 二阶导, 必须是浮点
int h[4];
double dec[3];

bool state;

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
  state=1;
  XYZ_Move(0,100,53);
  delay(2000);
  Claw.write(130);

  XYZ_Move(0,100,50);   
  delay(2000);
  
  XYZ_Move(0,40,50);  
  delay(2000);
}

void V_Draw(){
  state=1;
  XYZ_Move(-30,120,53);
  delay(2000);
  Claw.write(130);

  XYZ_Move(-30,120,50);
  delay(2000);
  XYZ_Move(0,50,50);
  delay(2000);
  XYZ_Move(30,120,50);
  delay(2000);
}

void Point_Record(){
  byte i=0;
  int time=0;
  while(i<5){
    Joystick_Control();
    if(Serial.available()){
      if(Serial.read()=='1'){
        seread();
        point[i][0]=x;
        point[i][1]=y;
        point[i][2]=z;
        times[i]=time;
        time=0;
        i++;
      }
      Serial.readStringUntil('\n');
    }
    time++;
    delay(50);
  }
}

void Point_l_Replay(){
  state=1;
  Claw.write(90);
  Right.write(90);
  Left.write(90);
  Base.write(90);
  delay(3000);
  byte i=0;
  while(i<5){
    XYZ_Move(point[i][0],point[i][1],point[i][2]);
    i++;
    delay(500);
  }
}

void data_handling(byte j){
  m[0][j]=0;
  m[4][j]=0;
  for(byte i=1;i<=3;i++){
    dec[i-1]=6.0/(h[i-1]+h[i])*((point[i+1][j]-point[i][j])/(double)h[i]-(point[i][j]-point[i-1][j])/(double)h[i-1]);
  }
  double aa[3]={0, 1.0*h[1]/(h[1]+h[2]), 1.0*h[2]/(h[2]+h[3])};
  double cc[3]={1.0*h[1]/(h[0]+h[1]), 1.0*h[2]/(h[1]+h[2]), 0};
  double cp[3],dp[3];
  cp[0]=cc[0]/2.0;         
  dp[0]=dec[0]/2.0;
  for(byte i=1;i<3;i++){
    double temp=2.0-aa[i]*cp[i-1];
    cp[i]=cc[i]/temp;
    dp[i]=(dec[i]-aa[i]*dp[i-1])/temp;
  }
  m[3][j]=dp[2];
  m[2][j]=dp[1]-cp[1]*m[3][j];
  m[1][j]=dp[0]-cp[0]*m[2][j];
}

double point_where(byte j,double t){
  byte i=0;
  while(i<3&&t>times[i+1]){
    i++;
  }
  double hi=h[i],dt=t-times[i],rm=times[i+1]-t;
  if(hi<1)return point[i][j];
  return m[i][j]*rm*rm*rm/(6*hi)+m[i+1][j]*dt*dt*dt/(6*hi)+(point[i][j]-m[i][j]*hi*hi/6)*rm/hi+(point[i+1][j]-m[i+1][j]*hi*hi/6)*dt/hi;
}

void Point_s_Replay(){
  state=1;
  for(byte i=1;i<=4;i++){
    h[i-1]=times[i]-times[i-1];
    if(h[i-1]<1){
      h[i-1]=1;
    }
  }
  for(byte j=0;j<3;j++){
    data_handling(j);
  }
  Claw.write(90);
  Right.write(90);
  Left.write(90);
  Base.write(90);
  delay(3000);
  const double STEP=1.5;
  double px=point_where(0,times[0]),py=point_where(1,times[0]),pz=point_where(2,times[0]);
  XYZ_Move(px,py,pz);
  double acc=0;
  for(double t=times[0]+0.1;t<=times[4];t+=0.1){
    if(!state)return;
    double cx=point_where(0,t), cy=point_where(1,t), cz=point_where(2,t);
    acc+=sqrt((cx-px)*(cx-px)+(cy-py)*(cy-py)+(cz-pz)*(cz-pz));
    if(acc>=STEP){
      XYZ_Move(cx,cy,cz);
      acc=0;
    }
    px=cx; 
    py=cy; 
    pz=cz;
  }
  if(state)XYZ_Move(point_where(0,times[4]),point_where(1,times[4]),point_where(2,times[4]));
}