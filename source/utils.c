#include "utils.h"

int bilinearFilterEnabled;

void setBilinearFilter(int enabled, sf2d_texture *texture)
{
	bilinearFilterEnabled = enabled;
	if (enabled == 1)
	{
		sf2d_texture_set_params(texture, GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) | GPU_TEXTURE_MIN_FILTER(GPU_NEAREST));
	}
}

void getSizeString(char *string, uint64_t size) //Thanks TheOfficialFloW
{
	double double_size = (double)size;

	int i = 0;
	static char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
	while (double_size >= 1024.0f) {
		double_size /= 1024.0f;
		i++;
	}

	sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}

char * base64Encode(u8 const * input)
{
    int      len      = strlen((const char *)input);
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