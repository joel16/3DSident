#include <3ds.h>
#include <stdlib.h>
#include <string.h>

Handle amHandle;

char * base64encode(const char * input);
Result amNetGetDeviceCert(u8 const * buffer);
