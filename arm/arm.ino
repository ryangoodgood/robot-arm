#include<Servo.h>
#include<WString.h>
#include<math.h>

#define Left_Joystick_x A0
#define Left_Joystick_y A1
#define Right_Joystick_x A2
#define Right_Joystick_y A3

#define H 60
#define L1 80
#define L2 80
#define LEFT_MAX 100

Servo Claw;
Servo Left;
Servo Right;
Servo Base;

int Speed;

int Lx,Ly,Rx,Ry;

int Base_read,Left_read,Right_read,Claw_read;

double R,Z,r;
double x,y,z;
double q1,q2,q3;

byte mode=0;

byte total_time;

byte Base_State[255];
byte Left_State[255];
byte Right_State[255];
byte Claw_State[255];

int point[5][3];
int times[5];

double m[5][3];
int h[4];
double dec[3];

bool state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Record_Clear();

  Claw.attach(6);
  Right.attach(7);
  Left.attach(8);
  Base.attach(9);

  Init();

  Speed=5;

  seread();

  text_print();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial_Communication();
  Joystick_Control();
  delay(50);
}

void Init(){
  Claw.write(90);
  Right.write(90);
  Left.write(90);
  Base.write(90);
}

void Serial_Communication(){
  String line;
  char temp;
  if(Serial.available()){
      line=Serial.readStringUntil('\n');
      line.trim();
      if(line.length()==1){
        temp=line[0];
        switch(temp){
          case '1':
          case '2':
          case '3':
          case '4':
            Button_Control((byte)(temp-'0'));
            break;

          case 'O':
          case 'o':
            Claw.write(0);
            Serial.println(F("恭喜你,爪子打开了!"));
            break;

          case 'S':
          case 's':
            Claw.write(115);
            Serial.println(F("恭喜你,爪子关上了!"));
            break;

          case 'H':
          case 'h':
            Speed+=1;
            if(Speed>30){
              Speed=30;
              Serial.println(F("太快了舵机要受不了了!!!"));
            }
            Serial.print(F("目前速度为:"));
            Serial.println(Speed);
            break;

          case 'L':
          case 'l':
            Speed-=1;
            if(Speed<=0){
              Speed=1;
              Serial.println(F("Speed不能小于等于0!!!"));
            }
            Serial.print(F("目前速度为:"));
            Serial.println(Speed);
            break;

          case 'A':
          case 'a':
            Claw_A(); 
            Serial.println(F("执行成功!"));
            break;

          case 'B':
          case 'b':
            Claw_B(); 
            Serial.println(F("执行成功!"));
            break;

          case 'C':
          case 'c':
            Claw_C(); 
            Serial.println(F("执行成功!"));
            break;

          case 'P':
          case 'p':
            Straight_Draw();
            break;

          case 'V':
          case 'v':
            V_Draw();
            break;

          case 'R':
          case 'r':
            Point_Record();
            break;

          case 'M':
          case 'm':
            Point_l_Replay();
            break;

          case 'N':
          case 'n':
            Point_s_Replay();
            break;

          case '?':
            Out_Put();
            break;
            
          case '!':
            seread();
            Serial.print(F("x:"));
            Serial.println(x);
            Serial.print(F("y:"));
            Serial.println(y);
            Serial.print(F("z:"));
            Serial.println(z);
            break;

          default:
            Serial.println(F("ERROR,输入有问题"));
            break;
            
        }
      }else{
        if(!PWM_Out(line)){
          Serial.println(F("Error,请输入一个字符"));
        }
      }
      Serial.println("");
      text_print();
  }
}

bool PWM_Out(String &line){
  int x,y,z;
  if(line.length()>15){
    return 0;
  }else{
    byte i=0;
    if(line[i]=='X'||line[i]=='x'){
      x=Num_Read(line,++i);
      if(line[i++]==',')
      if(line[i]=='Y'||line[i]=='y'){
        y=Num_Read(line,++i);
        if(line[i++]==',')
        if(line[i]=='Z'||line[i]=='z'){
          z=Num_Read(line,++i);
          if((x>=0&&x<=180)&&(y>=0&&y<=100)&&(z>=30&&z<=150)){
            Base.write(x);
            Left.write(y);
            Right.write(z);
            Serial.println(F("执行成功了,你太棒了!"));
          }else{
            Serial.println(F("我chovy,不要乱输入啊!"));
          }
          return 1;
        }
      }
    }
  }
  return 0;
}

int Num_Read(String &line,byte &i){
  int num=0;
  while(line[i]>='0'&&line[i]<='9'){
    num*=10;
    num+=(int)(line[i]-'0');
    i++;
  }
  return num;
}

void Out_Put(){
  Serial.print(F("Claw:"));
  Serial.println(Claw.read());
  Serial.print(F("Left:"));
  Serial.println(Left.read());
  Serial.print(F("Right:"));
  Serial.println(Right.read());
  Serial.print(F("Base:"));
  Serial.println(Base.read());
}

void text_print(){
  Serial.println(F("请输入指令"));
  Serial.println(F("'O',打开爪子"));
  Serial.println(F("'S',关闭爪子"));
  Serial.println(F("'H',嗨嗨嗨,要开始加速喽"));
  Serial.println(F("'L',呜呜呜,要开始减速了"));
  Serial.println(F("'A','B','C',三个夹取物体"));
  Serial.println(F("按钮1:A,B,C循环"));
  Serial.println(F("按钮2:录制动作"));
  Serial.println(F("按钮3:回放动作"));
  Serial.println(F("按钮4:重置位置"));
  Serial.println(F("'P',直线绘制"));
  Serial.println(F("'V',V绘制"));
  Serial.println(F("'R',五点录制"));
  Serial.println(F("'M',五点折线绘制"));
  Serial.println(F("'N',五点曲线绘制"));
  Serial.println(F("绘制中:按键1暂停,2继续,3取消"));
}

void Joystick_Control(){
  Lx=analogRead(Left_Joystick_x);
  Ly=analogRead(Left_Joystick_y);
  Rx=analogRead(Right_Joystick_x);
  Ry=analogRead(Right_Joystick_y);

  Base_read=Base.read();
  Left_read=Left.read();
  Right_read=Right.read();
  Claw_read=Claw.read();

  if(Lx<=200){
    if(Base_read>=180){
      Serial.println(F("Base电机到限制了!!!"));
    }else{
      Base.write(Base_read+Speed);
    }
  }else if(Lx>=800){
    if(Base_read<=0){
      Serial.println(F("Base电机到限制了!!!"));
    }else{
      Base.write(Base_read-Speed);
    }
  }

  if(Ly<=200){
    if(Right_read>=150){
      Serial.println(F("Right电机到限制了!!!"));
    }else{
      Right.write(Right_read+Speed);
    }
  }else if(Ly>=800){
    if(Right_read<=30){
      Serial.println(F("Right电机到限制了!!!"));
    }else{
      Right.write(Right_read-Speed);
    }
  }

  if(Rx<=200){
    if(Claw_read<=0){
      Serial.println(F("Claw电机到限制了!!!"));
    }else{
      Claw.write(Claw_read-Speed);
    }
  }else if(Rx>=800){
    if(Claw_read>=120){
      Serial.println(F("Claw电机到限制了!!!"));
    }else{
      Claw.write(Claw_read+Speed);
    }
  }

  if(Ry<=200){
    if(Left_read<=0){
      Serial.println(F("Left电机到限制了!!!"));
    }else{
      Left.write(Left_read-Speed);
    }
  }else if(Ry>=800){
    if(Left_read>=100){
      Serial.println(F("Left电机到限制了!!!"));
    }else{
      Left.write(Left_read+Speed);
    }
  }
}

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
  Right.write(q2);
  Left.write(q3);
  Base.write(q1);
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
  int n=(int)(dist+0.5);               
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
              Init();
              return;
            }
          }
        }
      }else if(temp=='3'){
        state=0;
        Init();
        return;
      }
    }
    x=x0+step_x*i;
    y=y0+step_y*i;
    z=z0+step_z*i;
    sewrite();
    delay(10);
  }

  Serial.print(F("到达 ("));
  Serial.print(t_x);
  Serial.print(F(","));
  Serial.print(t_y);
  Serial.print(F(","));
  Serial.print(t_z);
  Serial.println(F(")"));
  seread();
}

void Claw_A(){
  Claw_Move(60,Speed);
  Right_Move(90,Speed);
  Left_Move(90,Speed);
  Base_Move(90,Speed);

  Base_Move(135,Speed);
  delay(500);
  Left_Move(15,Speed);
  Right_Move(140,Speed);
  delay(500);
  Claw_Move(130,3);
  delay(500);

  Right_Move(100,Speed);
  delay(500);
  Left_Move(30,Speed);
  delay(500);
  Base_Move(45,Speed);
  delay(500);

  Left_Move(15,Speed);
  delay(500);
  Right_Move(140,Speed);
  delay(500);
  Claw_Move(60,3);
  delay(500);

  Right_Move(90,Speed);
  Left_Move(90,Speed);
  Base_Move(90,Speed);
  Claw_Move(90,Speed);
  delay(500);
}

void Claw_B(){
  Claw_Move(60,Speed);
  Right_Move(90,Speed);
  Left_Move(90,Speed);
  Base_Move(90,Speed);

  Base_Move(90,Speed);
  delay(500);
  Left_Move(30,Speed);
  Right_Move(120,Speed);
  delay(500);
  Claw_Move(130,3);
  delay(500);

  Right_Move(100,Speed);
  delay(500);
  Left_Move(50,Speed);
  delay(500);
  Base_Move(160,Speed);
  delay(500);

  Left_Move(30,Speed);
  delay(500);
  Right_Move(120,Speed);
  delay(500);
  Claw_Move(60,3);
  delay(500);

  Right_Move(90,Speed);
  Left_Move(90,Speed);
  Base_Move(90,Speed);
  Claw_Move(90,Speed);
  delay(500);
}

void Claw_C(){
  Claw_Move(30,Speed);
  Right_Move(90,Speed);
  Left_Move(90,Speed);
  Base_Move(90,Speed);

  Base_Move(30,Speed);
  delay(500);
  Left_Move(15,Speed);
  Right_Move(150,Speed);
  delay(500);
  Claw_Move(130,3);
  delay(500);

  Right_Move(100,Speed);
  delay(500);
  Left_Move(30,Speed);
  delay(500);
  Base_Move(150,Speed);
  delay(500);

  Left_Move(15,Speed);
  delay(500);
  Right_Move(140,Speed);
  delay(500);
  Claw_Move(30,3);
  delay(500);

  Right_Move(90,Speed);
  Left_Move(90,Speed);
  Base_Move(90,Speed);
  Claw_Move(90,Speed);
  delay(500);
}

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
      Init();
      break;

  }
}

void Record_Clear(){
  for(byte i=0;i<255;i++){
    Base_State[i]=90;
    Left_State[i]=90;
    Right_State[i]=90;
    Claw_State[i]=90;
  }
}

void Record(){
  Record_Clear();
  total_time=0;
  Serial.println(F("录制开始"));
  while(total_time<255){
    Joystick_Control();
    Base_State[total_time]=Base.read();
    Left_State[total_time]=Left.read();
    Right_State[total_time]=Right.read();
    Claw_State[total_time]=Claw.read();
    total_time++;
    delay(50);
    if(Serial.available()){
      if(Serial.read()=='2'){
        break;
      }
    }
  }
  Serial.println(F("录制结束"));
}

void Replay(){
  byte i=0;
  Serial.println(F("回放开始"));
  while(i<total_time){
    Base.write(Base_State[i]);
    Left.write(Left_State[i]);
    Right.write(Right_State[i]);
    Claw.write(Claw_State[i]);
    delay(50);
    i++;
  }
  Serial.println(F("回放结束"));
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

  Claw.write(90);
  XYZ_Move(0,80,140);
}

void V_Draw(){
  state=1;
  XYZ_Move(-50,60,54);
  delay(2000);
  Claw.write(130);
  delay(2000);

  XYZ_Move(-50,60,50);
  
  delay(2000);
  XYZ_Move(0,130,50);
  delay(2000);
  
  XYZ_Move(0,50,50);
  delay(2000);

  Claw.write(90);
  XYZ_Move(0,80,140);
}

void Point_Record(){
  byte i=0;
  int time=0;
  Serial.println(F("五点录制开始"));
  while(i<5){
    Joystick_Control();
    if(Serial.available()){
      if(Serial.read()=='1'){
        seread();
        point[i][0]=x;
        point[i][1]=y;
        point[i][2]=z;
        times[i]=time;
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
  Init();
  Serial.println(F("五点折线绘制开始"));
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
  Serial.println(F("五点曲线绘制开始"));
  for(byte i=1;i<=4;i++){
    h[i-1]=times[i]-times[i-1];
    if(h[i-1]<1){
      h[i-1]=1;
    }
  }
  for(byte j=0;j<3;j++){
    data_handling(j);
  }
  Init();
  delay(3000);
  const double step=1.5;
  double px=point_where(0,times[0]),py=point_where(1,times[0]),pz=point_where(2,times[0]);
  XYZ_Move(px,py,pz);
  double acc=0;
  for(double t=times[0]+0.1;t<=times[4];t+=0.1){
    if(!state)return;
    double cx=point_where(0,t), cy=point_where(1,t), cz=point_where(2,t);
    acc+=sqrt((cx-px)*(cx-px)+(cy-py)*(cy-py)+(cz-pz)*(cz-pz));
    if(acc>=step){
      XYZ_Move(cx,cy,cz);
      acc=0;
    }
    px=cx; 
    py=cy; 
    pz=cz;
  }
  if(state)XYZ_Move(point_where(0,times[4]),point_where(1,times[4]),point_where(2,times[4]));
}