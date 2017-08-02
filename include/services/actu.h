#ifndef ACTU_H
#define ACTU_H

#include <3ds.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

struct Birthday
{
	u16 year; 
	u8 month; 
	u8 day;
}; 

typedef struct
{
	u32 miiID; // Mii ID
	u64 systemID; // System ID (identifies owner, for purpose of enforcing editing restrictions and blue pants).
	u32 dateOfCreation; // Specialness and date of creation (big-endian 32bit unsigned integer)
	u8 mac_addr[0x6]; // Creator's full MAC
	u16 padding1;
	u16 birthday; // Bit-mapped: Birthday (4bit-day,5bit-month), Sex, Shirt color, Favorite
	u16 miiName[0x14]; // UTF-16 Mii Name
	u16 widthHeight; // width & height
	u8 face; // bit 0: disable sharing, bit 1-4: face shape, bit 5-7: skin color
	u8 makeup; // bit 0-3: wrinkles, bit 4-7: makeup
	u8 hairStyle; // hair style
	u8 hairColour; // bit 0-2: hair color, bit 3: flip hair
	u8 unk[0x4]; 
	u8 eyebrowStyle; // bit 0-4: eyebrow style, bit 5-7: eyebrow color
	u8 eyebrowScale; // bit 0-3: eyebrow scale, bit 4-6: eyebrow yscale
	u16 eyebrowPosition; // bit 0-3: eyebrow rotation, bit 5-8: eyebrow x spacing, bit 9-13: eyebrow y position
	u8 unk1[0x4];
	bool allowCopy; // Allow Copying (?) (0D on 0E off 8D on)
	u8 unknown[0x7];
	u16 authorName[0x14]; // UTF-16 Author Name (10 chars max
} MiiData; 

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

Result actInit(u32 sdkVer, u32 sharedMemSize);
void actExit(void);
Result ACTU_Initialize(u32 sdkVer, void *addr, u32 memSize);
Result ACTU_GetAccountDataBlock(void *buffer, u32 size, u32 blkId);

#endif