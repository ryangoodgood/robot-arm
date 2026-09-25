#include<Servo.h>
#include<string.h>

#define LEFT_BUTTON 5
#define RIGHT_BUTTON 4

#define Left_Joystick_x A0
#define Left_Joystick_y A1
#define Right_Joystick_x A2
#define Right_Joystick_y A3

Servo Claw;
Servo Left;
Servo Right;
Servo Base;

int Speed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Claw.attach(6);
  Right.attach(7);
  Left.attach(8);
  Base.attach(9);
  Claw.write(100);
  Right.write(60);
  Left.write(90);
  Base.write(90);

  pinMode(LEFT_BUTTON,INPUT_PULLUP);
  pinMode(RIGHT_BUTTON,INPUT_PULLUP);
  
  Speed=5;

  Serial.println("请输入指令");
  Serial.println("'O',打开爪子");
  Serial.println("'S',关闭爪子");
  Serial.println("'H',嗨嗨嗨,要开始加速喽");
  Serial.println("'L',呜呜呜,要开始减速了");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial_Communication();
  Servo_Bool();

}

void Serial_Communication(){
  String line;
  char temp;
  if(Serial.available()){
      line=Serial.readStringUntil('\n');
      if(line.length()==1){
        temp=line[0];
        if(temp=='O'||temp=='o'){
          Claw.write(0);
          Serial.println("恭喜你,爪子打开了!");
        }else if(temp=='S'||temp=='s'){
          Claw.write(115);
          Serial.println("恭喜你,爪子关上了!");
        }else if(temp=='H'||temp=='h'){
          Speed+=1;
          Serial.print("目前速度为:");
          Serial.println(Speed);
        }else if(temp=='L'||temp=='l'){
          Speed-=1;
          Serial.print("目前速度为:");
          Serial.println(Speed);
        }else{
          Serial.println("ERROR,输入有问题");
        }
      }else{
        if(!PWM_Out(line)){
          Serial.println("Error,请输入一个字符");
        }
      }
      Serial.println("");
      Serial.println("请输入指令");
      Serial.println("'O',打开爪子");
      Serial.println("'S',关闭爪子");
      Serial.println("'H',嗨嗨嗨,要开始加速喽");
      Serial.println("'L',呜呜呜,要开始减速了");
  }
}

bool PWM_Out(String &line){
  int x,y,z;
  if(line.length()>15){
    return 0;
  }else{
    int i=0;
    if(line[i]=='x'){
      x=Num_Read(line,++i);
      if(line[i]==',')
      if(line[++i]=='y'){
        y=Num_Read(line,++i);
        if(line[i]==',')
        if(line[++i]=='z'){
          z=Num_Read(line,++i);
          if(x<=180&&y<=180&&z<=180){
            Base.write(x);
            Left.write(y);
            Right.write(z);
            Serial.println("执行成功了,你太棒了!");
          }else{
            Serial.println("我chovy,不要乱输入啊!");
          }
          return 1;
        }
      }
    }
  }
  return 0;
}

void Servo_Bool(){
  int Lx=analogRead(Left_Joystick_x);
  int Ly=analogRead(Left_Joystick_y);
  int Rx=analogRead(Right_Joystick_x);
  int Ry=analogRead(Right_Joystick_y);

  if(Lx<=200){
    Base.write(Base.read()+Speed);
    delay(50);
  }else if(Lx>=800){
    Base.write(Base.read()-Speed);
    delay(50);
  }
  if(Ly<=200){
    Right.write(Right.read()+Speed);
    delay(50);
  }else if(Ly>=800){
    Right.write(Right.read()-Speed);
    delay(50);
  }
  if(Rx<=200){
    Claw.write(Claw.read()-Speed);
    delay(50);
  }else if(Rx>=800){
    Claw.write(Claw.read()+Speed);
    delay(50);
  }
  if(Ry<=200){
    Left.write(Left.read()-Speed);
    delay(50);
  }else if(Ry>=800){
    Left.write(Left.read()+Speed);
    delay(50);
  }
}

int Num_Read(String &line,int &i){
  int num=0;
  while(line[i]>='0'&&line[i]<='9'){
    num*=10;
    num+=(int)line[i]-'0';
    i++;
  }
  return num;
}