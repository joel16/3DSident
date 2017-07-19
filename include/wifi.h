#ifndef WIFI_H
#define WIFI_H

#include <3ds.h>

/*	
	Wifi Slot Structure constructed using the info provided here https://www.3dbrew.org/wiki/Config_Savegame#WiFi_Slot_Structure
*/

#define CFG_WIFI_BLKID (u32) 0x00080000 // Configuration blocks BlkID for WiFi configuration slots
#define CFG_WIFI_SLOT_SIZE (u32) 0xC00 // Blk size 0xC

typedef struct 
{
	bool set; // Was the network was set or not?
	bool use; // Use this network strucutre to connect?
	bool isFirst; // If this structure is the first (0) or the second (1) in the larger WiFi slot structure?
	u8 padding1;
	
	char SSID[0x20]; // SSID of the network, without a trailing nullbyte.
	u8 SSID_length; // Length of the SSID.
	u8 AP_crypto_key_type;
	u16 padding2;
	
	char passphrase[0x40]; // Plaintext of the passphrase of the network, without a trailing nullbyte.
	u8 PBKDF2_passphrase[0x20]; // PBKDF2 of the passphrase and SSID (http://jorisvr.nl/wpapsk.html).
} networkStructure; // This is apparently used twice in the actual wifi slot structure (?) // 0xAC

typedef struct 
{
	bool set; // Was the network was set or not?
	u16 checksum; // CRC-16 checksum of the next 0x410 bytes.
	
	networkStructure network; // First network structure. Only set if the network was set "normally", or was the last to be set using WPS during the session. Size 0x88
	u8 padding1[0x20];
	
	networkStructure network_WPS; // Second network structure. Only set if the network was set using WPS, otherwise 0-filled.
	u8 padding2[0x20C];
	
	bool auto_obtain_ip_addr; // Automatically get the IP address (use DHCP) or not, defaults to 1.
	bool auto_obtain_DNS; // Automatically get the DNS or not, defaults to 1
	u16 padding3;
	
	u8 ip_addr[0x4]; // IP address, to use if (auto_obtain_ip_addr) is false.
	u8 gateway_addr[0x4]; // IP address of the gateway, to use if (auto_obtain_ip_addr) is false.
	u8 subnet_mask[0x4]; // Subnetwork mask, to use if (auto_obtain_ip_addr) is false.
	u8 primary_DNS[0x4]; // IP address of the primary DNS , to use if (auto_obtain_ip_addr) is false.
	u8 secondary_DNS[0x4]; // IP address of the secondary DNS, to use if (auto_obtain_ip_addr) is false.
	
	u8 lastSet[0x4]; // Always 0x01050000 ? Only set if the network was the last to be set during the session.
	u8 ip_to_use[0x4]; // IP address to use. Only set if the network was the last to be set during the session.
	u8 mac_addr[0x6]; // MAC address of the AP. Only set if the network was the last to be set during the session.
	u8 channel; // Channel. Only set if the network was the last to be set during the session.
	u8 padding4; // Only set if the network was the last to be set during the session.
	
	bool use_proxy; // Use a proxy or not, defaults to 0.
	bool basic_authentication; // Use a basic authentication for the proxy, defaults to 0.
	u16 port_number; // Port to use for the proxy, defaults to 1.
	char proxy_addr[0x30]; // URL/address of the proxy, including a trailing nullbyte.
	u8 padding5[0x34];
	char proxy_username[0x20]; // Username to use for basic authentication, including a trailing nullbyte.
	char proxy_password[0x20]; // Password to use for basic authentication, including a trailing nullbyte.
	u16 padding6;
	u16 MTU_value; // MTU value, defaults to 1400 and ranges between 576 and 1500, inclusive.
	u8 padding8[0x7EC];
} wifiSlotStructure;

#endif