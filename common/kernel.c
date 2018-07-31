#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "kernel.h"
#include "system.h"
#include "utils.h"

char *Kernel_GetInitalVersion(void)
{
	static char initialVer[0xB];
	
	FS_OpenArchive(&archive, ARCHIVE_NAND_TWL_FS);
	
	Handle handle;
	
	if (R_FAILED(FSUSER_OpenFileDirectly(&handle, ARCHIVE_NAND_TWL_FS, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, "/sys/log/product.log"), FS_OPEN_READ, 0)))
		return NULL;
	
	u64 size64 = 0;
	u32 size = 0;
	
	if (R_FAILED(FSFILE_GetSize(handle, &size64)))
		return NULL;
		
	size = (u32)size64;
	
	char *buf = (char *)malloc(size + 1);
	u32 bytesread = 0;
	
	if (R_FAILED(FSFILE_Read(handle, &bytesread, 0, (u32 *)buf, size)))
		return NULL;
	
	buf[size] = '\0';
	
	// New 3DS/2DS only
	strcpy(initialVer, Utils_ExtractBetween(buf, "cup:", " preInstall:"));

	if (strlen(initialVer) == 0)
		strcpy(initialVer, Utils_ExtractBetween(buf, "cup:", ","));

	strcat(initialVer, "-");
	strcat(initialVer, Utils_ExtractBetween(buf, "nup:", " cup:"));
	
	if (R_FAILED(FSFILE_Close(handle)))
		return NULL;
	
	free(buf);
	
	FS_CloseArchive(archive);
	
	return initialVer;
}

char *Kernel_GetVersion(int version)
{
	char *str_kernel = (char *)malloc(sizeof(char) * 255), *str_ver = (char *)malloc(sizeof(char) * 255), *str_sysver = (char *)malloc(sizeof(char) * 255);
	u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion();
	OS_VersionBin *nver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin)), *cver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin));
	s32 ret;
	
	snprintf(str_kernel, 255, "%lu.%lu-%lu",
			GET_VERSION_MAJOR(os_ver),
			GET_VERSION_MINOR(os_ver),
			GET_VERSION_REVISION(os_ver)
	);
	
	snprintf(str_ver, 255, "%lu.%lu-%lu",
			GET_VERSION_MAJOR(firm_ver),
			GET_VERSION_MINOR(firm_ver),
			GET_VERSION_REVISION(firm_ver)
	);
				
	memset(nver, 0, sizeof(OS_VersionBin));
	memset(cver, 0, sizeof(OS_VersionBin));

	if (R_FAILED(ret = osGetSystemVersionData(nver, cver)))
		snprintf(str_sysver, 100, "0x%08liX", ret);
	else
		snprintf(str_sysver, 100, "%d.%d.%d-%d%c",
			cver->mainver,
			cver->minor,
			cver->build,
			nver->mainver,
			System_GetFirmRegion()
		);
	
	if (version == 0)
		return str_kernel;
	else if (version == 1)
		return str_ver;
	else if (version == 2)
		return Kernel_GetInitalVersion();
	else 
		return str_sysver;
}

char *Kernel_GetSDMCCID(void)
{
	u8 buf[16];
	static char cid[33];
		
	if (R_SUCCEEDED(FSUSER_GetSdmcCid(buf, 0x10)))
	{
		snprintf(cid, 33, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
			buf[12], buf[13], buf[14], buf[15]);
		
		return cid;
	}

	return NULL;
}

char *Kernel_GetNANDCID(void)
{
	u8 buf[16];
	static char cid[33];
		
	if (R_SUCCEEDED(FSUSER_GetNandCid(buf, 0x10)))
	{
		snprintf(cid, 33, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
			buf[12], buf[13], buf[14], buf[15]);

		return cid;
	}

	return NULL;
}

u32 Kernel_GetDeviceId(void) // Same as PS_GetDeviceId
{
	u32 id = 0;
	
	if (R_SUCCEEDED(AM_GetDeviceId(&id)))
		return id;
	
	return 0;
}
