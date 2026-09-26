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
  Right.write(30);
  Claw.write(90);
  Right.write(90);
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
        }else if(temp=='A'||temp=='a'){
          Claw_A(); 
          Serial.println("执行成功!");
        }else if(temp=='C'||temp=='c'){
          Claw_C();
          Serial.println("执行成功!");
        }else if(temp=='?'){
          Out_Put();
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
    if(line[i]=='X'||line[i]=='x'){
      x=Num_Read(line,++i);
      if(line[i]==',')
      if(line[++i]=='Y'||line[++i]=='y'){
        y=Num_Read(line,++i);
        if(line[i]==',')
        if(line[++i]=='Z'||line[++i]=='z'){
          z=Num_Read(line,++i);
          if((x>=0&&x<=180)&&(y>=0&&y<=120)&&(z>=30&&z<=150)){
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

  int Base_read=Base.read();
  int Left_read=Left.read();
  int Right_read=Right.read();
  int Claw_read=Claw.read();

  if(Lx<=200){
    if(Base_read>=180){
      Serial.println("Base电机到限制了!!!");
      delay(50);
    }else{
      Base.write(Base_read+Speed);
      delay(50);
    }
  }else if(Lx>=800){
    if(Base_read<=0){
      Serial.println("Base电机到限制了!!!");
      delay(50);
    }else{
      Base.write(Base_read-Speed);
      delay(50);
    }

  }

  if(Ly<=200){
    if(Right_read>=150){
      Serial.println("Right电机到限制了!!!");
      delay(50);
    }else{
      Right.write(Right_read+Speed);
      delay(50);
    }
  }else if(Ly>=800){
    if(Right_read<=30){
      Serial.println("Right电机到限制了!!!");
      delay(50);
    }else{
      Right.write(Right_read-Speed);
      delay(50);
    }
  }

  if(Rx<=200){
    if(Claw_read<=0){
      Serial.println("Claw电机到限制了!!!");
      delay(50);
    }else{
      Claw.write(Claw_read-Speed);
      delay(50);
    }
  }else if(Rx>=800){
    if(Claw_read>=120){
      Serial.println("Claw电机到限制了!!!");
      delay(50);
    }else{
      Claw.write(Claw_read+Speed);
      delay(50);
    }
  }

  if(Ry<=200){
    if(Left_read<=0){
      Serial.println("Left电机到限制了!!!");
      delay(50);
    }else{
      Left.write(Left_read-Speed);
      delay(50);
    }
  }else if(Ry>=800){
    if(Left_read>=120){
      Serial.println("Left电机到限制了!!!");
      delay(50);
    }else{
      Left.write(Left_read+Speed);
      delay(50);
    }
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

void Out_Put(){
  Serial.print("Claw:");
  Serial.println(Claw.read());
  Serial.print("Left:");
  Serial.println(Left.read());
  Serial.print("Right:");
  Serial.println(Right.read());
  Serial.print("Base:");
  Serial.println(Base.read());
}

void Claw_A(){
  Claw.write(60);
  Right.write(90);
  Left.write(90);
  Base.write(90);

  Base_Move(135,Speed);
  delay(500);
  Left_Move(15,Speed);
  Right_Move(150,Speed);
  delay(500);
  Claw_Move(140,3);
  delay(500);

  Right_Move(100,Speed);
  delay(500);
  Left_Move(30,Speed);
  delay(500);
  Base_Move(45,Speed);
  delay(500);

  Left_Move(15,Speed);
  delay(500);
  Right_Move(150,Speed);
  delay(500);
  Claw_Move(60,3);
  delay(500);

  Right_Move(90,Speed);
  Left_Move(90,Speed);
  delay(500);
  Base_Move(90,Speed);
}

void Claw_C(){
  Claw.write(30);
  Right.write(90);
  Left.write(90);
  Base.write(90);

  Base_Move(30,Speed);
  delay(500);
  Left_Move(15,Speed);
  Right_Move(150,Speed);
  delay(500);
  Claw_Move(140,3);
  delay(500);

  Right_Move(100,Speed);
  delay(500);
  Left_Move(30,Speed);
  delay(500);
  Base_Move(150,Speed);
  delay(500);

  Left_Move(15,Speed);
  delay(500);
  Right_Move(150,Speed);
  delay(500);
  Claw_Move(30,3);
  delay(500);

  Right_Move(90,Speed);
  Left_Move(90,Speed);
  delay(500);
  Base_Move(90,Speed);
  Claw_Move(60,Speed);
}

void Base_Move(int target,int MoveSpeed){
  int now=Base.read();
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
}

void Left_Move(int target,int MoveSpeed){
  int now=Left.read();
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
}

void Right_Move(int target,int MoveSpeed){
  int now=Right.read();
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
}

void Claw_Move(int target,int MoveSpeed){
  int now=Claw.read();
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
}