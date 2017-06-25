#include "am.h"
	
char * base64encode(const char * input)
{
    int      len      = strlen(input);
    int      leftover = len % 3;
    char    *ret      = malloc(((len/3) * 4) + ((leftover)?4:0) + 1);
    int      n        = 0;
    int      outlen   = 0;
    uint8_t  i        = 0;
    uint8_t *inp      = (uint8_t *) input;
    const char *index = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789+/";

    if (ret == NULL)
        return NULL;

    // Convert each 3 bytes of input to 4 bytes of output.
    len -= leftover;
    for (n = 0; n < len; n+=3) {
        i = inp[n] >> 2;
        ret[outlen++] = index[i];

        i  = (inp[n]   & 0x03) << 4;
        i |= (inp[n+1] & 0xf0) >> 4;
        ret[outlen++] = index[i];

        i  = ((inp[n+1] & 0x0f) << 2);
        i |= ((inp[n+2] & 0xc0) >> 6);
        ret[outlen++] = index[i];

        i  = (inp[n+2] & 0x3f);
        ret[outlen++] = index[i];
    }

    // Handle leftover 1 or 2 bytes.
    if (leftover) {
        i = (inp[n] >> 2);
        ret[outlen++] = index[i];

        i = (inp[n]   & 0x03) << 4;
        if (leftover == 2) {
            i |= (inp[n+1] & 0xf0) >> 4;
            ret[outlen++] = index[i];

            i  = ((inp[n+1] & 0x0f) << 2);
        }
        ret[outlen++] = index[i];
        ret[outlen++] = '=';
        if (leftover == 1)
            ret[outlen++] = '=';
    }
    ret[outlen] = '\0';
    return ret;
}

Result amNetGetDeviceCert(u8 const * buffer)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x818,1,2); // 0x08180042
	cmdbuf[1] = 0x180;
	cmdbuf[2] = (0x180 << 4) | 0xC;
	cmdbuf[3] = (u32)buffer;

	if(R_FAILED(ret = svcSendSyncRequest(amHandle))) return ret;

	return (Result)cmdbuf[1];
}
