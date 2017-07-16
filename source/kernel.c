#include "actu.h"
#include "kernel.h"

char * getVersion(int version)
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
	
	snprintf(str_ver, 255, "%lu.%lu-%lu\n",
			GET_VERSION_MAJOR(firm_ver),
			GET_VERSION_MINOR(firm_ver),
			GET_VERSION_REVISION(firm_ver)
	);
				
	memset(nver, 0, sizeof(OS_VersionBin));
	memset(cver, 0, sizeof(OS_VersionBin));
	ret = osGetSystemVersionData(nver, cver);

	if (ret)
		snprintf(str_sysver, 100, "0x%08liX", ret);
	else
		snprintf(str_sysver, 100, "%d.%d.%d-%d",
			cver->mainver,
			cver->minor,
			cver->build,
			nver->mainver
	);
	
	if (version == 0)
		return str_kernel;
	else if(version == 1)
		return str_ver;
	else 
		return str_sysver;
}

char * getCID(int type)
{
	u8 buf[16];
	static char cid[32];
	
	if (type == 0) //SDMC
		FSUSER_GetSdmcCid(buf, 0x10);
	
	else if (type == 1) //NAND
		FSUSER_GetNandCid(buf, 0x10);
		
	snprintf(cid, 33, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
			buf[12], buf[13], buf[14], buf[15]);
	
	return cid;
}