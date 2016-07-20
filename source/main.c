#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main()
{
	gfxInitDefault();

    char *str_ver = malloc(100), *str_sysver = malloc(100);
    u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion();

    consoleInit(GFX_TOP, NULL);

    snprintf(str_ver, 100, "Kernel version is %lu.%lu-%lu\nFIRM version is %lu.%lu-%lu\n",
             GET_VERSION_MAJOR(os_ver), GET_VERSION_MINOR(os_ver), GET_VERSION_REVISION(os_ver),
             GET_VERSION_MAJOR(firm_ver), GET_VERSION_MINOR(firm_ver), GET_VERSION_REVISION(firm_ver));

    printf(str_ver);

    OS_VersionBin *nver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));
    OS_VersionBin *cver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));

    memset(nver, 0, sizeof(OS_VersionBin));
    memset(cver, 0, sizeof(OS_VersionBin));

    s32 ret = osGetSystemVersionData(nver, cver);
    if (ret)
        printf("osGetSystemVersionData returned 0x%08X\n", (unsigned int) ret);

    snprintf(str_sysver, 100, "Current system version is %d.%d.%d-%d\n",
             cver->mainver, cver->minor, cver->build, nver->mainver);

    printf(str_sysver);
    printf("Press any key to exit");

    free(nver);
    free(cver);
    free(str_ver);
    free(str_sysver);

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		if (hidKeysDown()) break;

		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	gfxExit();
	return 0;
}
