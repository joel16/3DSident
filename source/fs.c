#include "fs.h"

void openArchive(FS_ArchiveID id)
{
	FSUSER_OpenArchive(&fsArchive, id, fsMakePath(PATH_EMPTY, ""));
}

void closeArchive(void)
{
	FSUSER_CloseArchive(fsArchive);
}

Result makeDir(FS_Archive archive, const char * path)
{
	if((!archive) || (!path))
		return -1;
	
	return FSUSER_CreateDirectory(archive, fsMakePath(PATH_ASCII, path), 0);
}

bool fileExists(FS_Archive archive, const char * path)
{
	if((!path) || (!archive))
		return false;
	
	Handle handle;

	Result ret = FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, 0);
	
	if(ret != 0)
		return false;

	ret = FSFILE_Close(handle);
	
	if(ret != 0)
		return false;
	
	return true;
}

bool dirExists(FS_Archive archive, const char * path)
{	
	if((!path) || (!archive))
		return false;
	
	Handle handle;

	Result ret = FSUSER_OpenDirectory(&handle, archive, fsMakePath(PATH_ASCII, path));
	
	if(ret != 0)
		return false;

	ret = FSDIR_Close(handle);
	
	if(ret != 0)
		return false;
	
	return true;
}