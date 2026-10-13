#ifndef SERVO_MOVE_H
#define SERVO_MOVE_H

void Base_Move(byte target,byte MoveSpeed);
void Left_Move(byte target,byte MoveSpeed);
void Right_Move(byte target,byte MoveSpeed);
void Claw_Move(byte target,byte MoveSpeed);
void XYZ_Move(const double t_x,const double t_y,const double t_z);
void sewrite();
void seread();

#endif
