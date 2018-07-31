#ifndef ACTU_H
#define ACTU_H

#include <3ds.h>

struct Birthday
{
	u16 year; 
	u8 month; 
	u8 day;
};

typedef struct
{
	u32 persistentID; // Persistent ID
	u32 padding1; 
	u64 transferableID; // Transferable ID Base
	u8 miiData[0x60]; // Mii data struct (above)
	u16 miiName[0x16]; // UTF-16 mii name
	char accountID[0x11]; // ASCII NUL-terminated Nintendo Network ID
	u8 padding2; 
	struct Birthday birthday;
	u32 principalID; // Principal ID
} AccountDataBlock;

Result actInit(void);
void actExit(void);
Result ACTU_GetAccountDataBlock(void * buffer, u32 size, u32 blkId);

#endif