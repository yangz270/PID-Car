#ifndef CAR_H
#define CAR_H

void Car_Init(void);

void Car_Forward(int16_t Speed);
void Car_Backward(int16_t Speed);
void Car_TurnLeft(int16_t Speed);
void Car_TurnRight(int16_t Speed);
void Car_Stop(void);

#endif
