#ifndef SERVO_MOVE_H
#define SERVO_MOVE_H

void Base_Move(byte target,byte MoveSpeed);
void Left_Move(byte target,byte MoveSpeed);
void Right_Move(byte target,byte MoveSpeed);
void Claw_Move(byte target,byte MoveSpeed);
void XYZ_Move(const int t_x,const int t_y,const int t_z);

#endif
