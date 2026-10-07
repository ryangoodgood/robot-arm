#include<Servo.h>
#include<Arduino.h>

#define Left_Joystick_x A0
#define Left_Joystick_y A1
#define Right_Joystick_x A2
#define Right_Joystick_y A3

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed;

int Lx,Ly,Rx,Ry;

int Base_read,Left_read,Right_read,Claw_read;

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
      delay(50);
    }else{
      Base.write(Base_read+Speed);
      delay(50);
    }
  }else if(Lx>=800){
    if(Base_read<=0){
      Serial.println(F("Base电机到限制了!!!"));
      delay(50);
    }else{
      Base.write(Base_read-Speed);
      delay(50);
    }
  }

  if(Ly<=200){
    if(Right_read>=150){
      Serial.println(F("Right电机到限制了!!!"));
      delay(50);
    }else{
      Right.write(Right_read+Speed);
      delay(50);
    }
  }else if(Ly>=800){
    if(Right_read<=30){
      Serial.println(F("Right电机到限制了!!!"));
      delay(50);
    }else{
      Right.write(Right_read-Speed);
      delay(50);
    }
  }

  if(Rx<=200){
    if(Claw_read<=0){
      Serial.println(F("Claw电机到限制了!!!"));
      delay(50);
    }else{
      Claw.write(Claw_read-Speed);
      delay(50);
    }
  }else if(Rx>=800){
    if(Claw_read>=120){
      Serial.println(F("Claw电机到限制了!!!"));
      delay(50);
    }else{
      Claw.write(Claw_read+Speed);
      delay(50);
    }
  }

  if(Ry<=200){
    if(Left_read<=0){
      Serial.println(F("Left电机到限制了!!!"));
      delay(50);
    }else{
      Left.write(Left_read-Speed);
      delay(50);
    }
  }else if(Ry>=800){
    if(Left_read>=120){
      Serial.println(F("Left电机到限制了!!!"));
      delay(50);
    }else{
      Left.write(Left_read+Speed);
      delay(50);
    }
  }
}
