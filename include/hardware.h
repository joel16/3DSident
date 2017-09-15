#ifndef HARDWARE_H
#define HARDWARE_H

#include <3ds.h>

char * getCardSlotStatus(void);
char * getBrightness(u32 screen);
bool detectSD(void);

#endif