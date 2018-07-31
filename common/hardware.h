#ifndef _3DSIDENT_HARDWARE_H_
#define _3DSIDENT_HARDWARE_H_

#include <3ds.h>

char *Hardware_GetAudioJackStatus(void);
char *Hardware_GetCardSlotStatus(void);
char *Hardware_DetectSD(void);
char *Hardware_GetBrightness(u32 screen);

#endif