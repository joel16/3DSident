#include <stdio.h>
#include <string.h>

#include "fs.h"

Result FS_OpenArchive(FS_Archive * archive, FS_ArchiveID archiveID)
{
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_OpenArchive(archive, archiveID, fsMakePath(PATH_EMPTY, ""))))
		return ret;

	return 0;
}

Result FS_CloseArchive(FS_Archive archive)
{
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_CloseArchive(archive)))
		return ret;

	return 0;
}

Result FS_MakeDir(FS_Archive archive, const char * path)
{	
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_CreateDirectory(archive, fsMakePath(PATH_ASCII, path), 0)))
		return ret;
	
	return 0;
}

Result FS_RecursiveMakeDir(FS_Archive archive, const char * dir) 
{
	Result ret = 0;
	char buf[256];
	char *p = NULL;
	size_t len;

	snprintf(buf, sizeof(buf), "%s",dir);
	len = strlen(buf);

	if (buf[len - 1] == '/')
		buf[len - 1] = 0;

	for (p = buf + 1; *p; p++)
	{
		if (*p == '/') 
		{
			*p = 0;

			ret = FS_MakeDir(archive, buf);
			
			*p = '/';
		}
		
		ret = FS_MakeDir(archive, buf);
	}
	
	return ret;
}

bool FS_FileExists(FS_Archive archive, const char * path)
{
	Handle handle;

	if (R_FAILED(FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, 0)))
		return false;

	if (R_FAILED(FSFILE_Close(handle)))
		return false;

	return true;
}

bool FS_DirExists(FS_Archive archive, const char * path)
{
	Handle handle;

	if (R_FAILED(FSUSER_OpenDirectory(&handle, archive, fsMakePath(PATH_ASCII, path))))
		return false;

	if (R_FAILED(FSDIR_Close(handle)))
		return false;

	return true;
}