#include<Servo.h>
#include<Arduino.h>

#include "Servo_Move.h"

extern Servo Claw;
extern Servo Left;
extern Servo Right;
extern Servo Base;

extern int Speed;

void Claw_A(){
  Claw.write(60);
  Right.write(90);
  Left.write(90);
  Base.write(90);

  Base_Move(135,Speed);
  delay(500);
  Left_Move(15,Speed);
  Right_Move(140,Speed);
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
  Right_Move(140,Speed);
  delay(500);
  Claw_Move(60,3);
  delay(500);

  Right_Move(90,Speed);
  Left_Move(90,Speed);
  delay(500);
  Base_Move(90,Speed);
}

void Claw_B(){
  Claw.write(60);
  Right.write(90);
  Left.write(90);
  Base.write(90);

  Base_Move(90,Speed);
  delay(500);
  Left_Move(30,Speed);
  Right_Move(120,Speed);
  delay(500);
  Claw_Move(140,3);
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
  Right_Move(140,Speed);
  delay(500);
  Claw_Move(30,3);
  delay(500);

  Right_Move(90,Speed);
  Left_Move(90,Speed);
  delay(500);
  Base_Move(90,Speed);
  Claw_Move(60,Speed);
}
