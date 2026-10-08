#include<Servo.h>
#include<string.h>

#include "Servo_Move.h"
#include "Claw_Grab.h"
#include "Joystick_Control.h"
#include "Record_Replay.h"
#include "Button_Control.h"
#include "Print.h"

#define LEFT_BUTTON 5
#define RIGHT_BUTTON 4

Servo Claw;
Servo Left;
Servo Right;
Servo Base;

int Speed;

extern double x,y,z;

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

  seread();

  text_print();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial_Communication();
  Joystick_Control();
  delay(50);
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

          case 'p':
            Straight_Draw();
            Claw.write(90);
            Right.write(90);
            Left.write(90);
             Base.write(90);
            break;

          case 'v':
            V_Draw();
            Claw.write(90);
            Right.write(90);
            Left.write(90);
            Base.write(90);
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
