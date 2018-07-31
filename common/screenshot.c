#include <3ds.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "screenshot.h"

static int num = 0;

static Result generateScreenshot(const char * path)
{
	int x = 0, y = 0;
	Handle handle;
	u32 bytesWritten = 0;
	u64 offset = 0;
	size_t size = 0x36;
	Result ret = 0;

	// Get top/bottom framebuffers
	u8 * gfxBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	u8 * gfxTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

	// Open file for writing screenshot
	if (R_FAILED(ret = FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_ASCII, path), (FS_OPEN_CREATE | FS_OPEN_WRITE), 0)))
		return ret;

	// Some
	u8 * buf = (u8*)malloc(size + 576000);
	memset(buf, 0, size + 576000);
	buf[size + 576000] = 0;

	if (R_FAILED(ret = FSFILE_SetSize(handle, (u16)(size + 576000))))
	{
		free(buf);
		return ret;
	}

	*(u16*)&buf[0x0] = 0x4D42;
	*(u32*)&buf[0x2] = size + 576000;
	*(u32*)&buf[0xA] = size;
	*(u32*)&buf[0xE] = 0x28;
	*(u32*)&buf[0x12] = 400;
	*(u32*)&buf[0x16] = 480;
	*(u32*)&buf[0x1A] = 0x00180001;
	*(u32*)&buf[0x22] = 576000;

	// Generate top left
	u8* framebuf = gfxTopLeft;

	for (y = 0; y < 240; y++)
	{
		for (x = 0; x < 400; x++)
		{
			int si = ((239 - y) + (x * 240)) * 3;
			int di = size + (x + ((479 - y) * 400)) * 3;
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
		}
	}

	// Generate bottom right
	framebuf = gfxBottom;

	for (y = 0; y < 240; y++)
	{
		for (x = 0; x < 320; x++)
		{
			int si = ((239 - y) + (x * 240)) * 3;
			int di = size + ((x+40) + ((239 - y) * 400)) * 3;
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
			buf[di++] = framebuf[si++];
		}

		// Make adjustments for the smaller width
		for (x = 0; x < 40; x++)
		{
			int di = size + (x + ((239 - y) * 400)) * 3;
			buf[di++] = 0;
			buf[di++] = 0;
			buf[di++] = 0;
		}

		for (x = 360; x < 400; x++)
		{
			int di = size + (x + ((239 - y) * 400)) * 3;
			buf[di++] = 0;
			buf[di++] = 0;
			buf[di++] = 0;
		}
	}

	if (R_FAILED(FSFILE_Write(handle, &bytesWritten, offset, (u32 *)buf, size + 576000, 0x10001)))
	{
		free(buf);
		return ret;
	}

	if (R_FAILED(FSFILE_Close(handle)))
	{
		free(buf);
		return ret;
	}

	free(buf);
	return 0;
}

static void generateScreenshotFileName(int number, char *fileName, const char *ext)
{
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);
	int num = number;
	int day = timeStruct->tm_mday;
	int month = timeStruct->tm_mon + 1;
	int year = timeStruct->tm_year + 1900;

	if (!(FS_DirExists(archive, "/screenshots/")))
		FS_MakeDir(archive, "/screenshots");

	sprintf(fileName, "/screenshots/Screenshot_%02d%02d%02d-%i%s", year, month, day, num, ext);
}

void Screenshot_Capture(void)
{
	static char filename[256];

	sprintf(filename, "%s", "screenshot");

	svcSleepThread(10000000);
	generateScreenshotFileName(num, filename, ".bmp");

	while (FS_FileExists(archive, filename))
	{
		num++;
		generateScreenshotFileName(num, filename, ".bmp");
	}

	generateScreenshot(filename);
	num++;
}