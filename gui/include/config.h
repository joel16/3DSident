#ifndef _3DSIDENT_CONFIG_H_
#define _3DSIDENT_CONFIG_H_

#include <3ds.h>

const wchar_t *Config_GetUsername(void);
char *Config_GetBirthday(void);
char *Config_GetEulaVersion(void);
char *Config_GetSoundOutputMode(void);
char *Config_GetParentalPin(void);
char *Config_GetParentalEmail(void);
char *Config_GetParentalSecretAnswer(void);
bool Config_IsDebugModeEnabled(void);
bool Config_IsUpdatesEnabled(void);
bool Config_IsPowerSaveEnabled(void);
bool Config_IsAutoBrightnessEnabled(void);

#endif