#include<Servo.h>

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
  Claw.write(90);
  Right.write(60);
  Left.write(90);
  Base.write(90);

  pinMode(LEFT_BUTTON,INPUT_PULLUP);
  pinMode(RIGHT_BUTTON,INPUT_PULLUP);
  
  Speed=5;
}

void loop() {
  // put your main code here, to run repeatedly:

  Servo_Bool();

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