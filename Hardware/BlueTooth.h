#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "stm32f10x.h"

void Bluetooth_Init(void);
void Bluetooth_Process(void);
void Bluetooth_SendString(const char *Text);
void Bluetooth_SendTelemetry(float Position, float LeftTarget,
                             float RightTarget, float LeftSpeed,
                             float RightSpeed);

#endif
