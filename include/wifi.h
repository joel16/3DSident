#ifndef WIFI_H
#define WIFI_H

#include <3ds.h>

/*	
	The things below here were found by LiquidFenrir and his Wifi Manager. (https://github.com/LiquidFenrir/WifiManager)
*/

#define CFG_WIFI_BLKID (u32)0x00080000
#define CFG_WIFI_SLOT_SIZE (u32)0xC00

typedef struct {
	bool exists;
	bool use;
	bool second;
	u8 padding1;
	char SSID[0x20];
	u8 SSID_length;
	u8 AP_encryption_type;
	u16 padding2;
	char password[0x40]; //plaintext, blank for a network set up with WPS
	u8 passwordPSK[0x20];
} networkStruct;

typedef struct {
	bool exists;
	u8 padding1;
	u16 checksum; //crc-16 of the next 0x410 bytes, with initval 0: https://github.com/lammertb/libcrc/blob/v2.0/src/crc16.c#L43-L76
	//if the network is set "normally", 'use' is 1
	//if the network is set by WPS, 'use' is 0
	//'second' is 0
	//if setting multiple networks with WPS in the same session, only the last set will have this. others will have completely 0x00 except for 'exists'
	networkStruct network; 
	u8 padding2[0x20];
	//completely 0x00 if the network was set "normally", otherwise (set by WPS) normal but 'use' and 'second' are 1
	networkStruct network_WPS;
	u8 padding3[0x20C];
	
	bool auto_obtain_IP; //defaults to 1
	bool auto_obtain_DNS; //defaults to 1
	u16 padding4;
	
	u8 IP_address[4];
	u8 gateway_address[4];
	u8 subnet_mask[4];
	
	u8 primary_DNS[4];
	u8 secondary_DNS[4];
	
	// if setting multiple networks in the same session, only the last set will have these. others will have completely 0x00
	u8 unk1[4];
	u8 IP_to_use[4];
	u8 MAC_address[6];
	u8 channel;
	u8 padding5;
	
	bool use_proxy; //defaults to 0
	bool basic_authentication; //defaults to 0
	u16 port_number; //defaults to 1
	char proxy_address[0x30]; //including ending nullbyte
	u8 padding6[0x34];
	char proxy_username[0x20]; //including ending nullbyte
	char proxy_password[0x20]; //including ending nullbyte
	u16 padding7;
	u16 MTU_value; //defaults to 1400, range [576;1500]
	
	//nothing beyond this point (0x414), but each slot is 0xC00 big...
	u8 padding8[0x7EC];
} wifiBlock;

#endif