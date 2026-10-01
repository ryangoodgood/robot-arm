#include<Servo.h>
#include<string.h>

#include "Servo_Move.h"
#include "Claw_Grab.h"
#include "Joystick_Control.h"
#include "Record_Replay.h"
#include "Button_Control.h"

#define LEFT_BUTTON 5
#define RIGHT_BUTTON 4

Servo Claw;
Servo Left;
Servo Right;
Servo Base;

int Speed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Record_Clear();

  Claw.attach(6);
  Right.attach(7);
  Left.attach(8);
  Base.attach(9);

  Claw.write(90);
  Right.write(90);
  Left.write(90);
  Base.write(90);

  pinMode(LEFT_BUTTON,INPUT_PULLUP);
  pinMode(RIGHT_BUTTON,INPUT_PULLUP);

  Speed=5;

  text_print();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial_Communication();
  Joystick_Control();
}

void Serial_Communication(){
  String line;
  char temp;
  if(Serial.available()){
      line=Serial.readStringUntil('\n');
      line.trim();
      if(line.length()==1){
        temp=line[0];
        if(temp=='1'||temp=='2'||temp=='3'||temp=='4'){
          Button_Control((byte)temp-'0');
        }else if(temp=='O'||temp=='o'){
          Claw.write(0);
          Serial.println(F("恭喜你,爪子打开了!"));
        }else if(temp=='S'||temp=='s'){
          Claw.write(115);
          Serial.println(F("恭喜你,爪子关上了!"));
        }else if(temp=='H'||temp=='h'){
          Speed+=1;
          Serial.print(F("目前速度为:"));
          Serial.println(Speed);
        }else if(temp=='L'||temp=='l'){
          Speed-=1;
          Serial.print(F("目前速度为:"));
          Serial.println(Speed);
        }else if(temp=='A'||temp=='a'){
          Claw_A(); 
          Serial.println(F("执行成功!"));
        }else if(temp=='B'||temp=='b'){
          Claw_B();
          Serial.println(F("执行成功!"));
        }else if(temp=='C'||temp=='c'){
          Claw_C();
          Serial.println(F("执行成功!"));
        }else if(temp=='?'){
          Out_Put();
        }else{
          Serial.println(F("ERROR,输入有问题"));
        }
      }else{
        if(!PWM_Out(line)){
          Serial.println(F("Error,请输入一个字符"));
        }
      }
      Serial.println("");
      text_print();

  }
  if(Speed<=0){
    Speed=1;
    Serial.println(F("Speed不能小于等于0!!!"));
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
          if((x>=0&&x<=180)&&(y>=0&&y<=120)&&(z>=30&&z<=150)){
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
    num+=(int)line[i]-'0';
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
}