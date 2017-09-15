#include "fs.h"

Result openArchive(FS_Archive * archive, FS_ArchiveID id)
{
	return FSUSER_OpenArchive(archive, id, fsMakePath(PATH_EMPTY, ""));
}

Result closeArchive(FS_Archive archive)
{
	return FSUSER_CloseArchive(archive);
}

Result makeDir(FS_Archive archive, const char * path)
{
	if((!archive) || (!path))
		return -1;
	
	return FSUSER_CreateDirectory(archive, fsMakePath(PATH_ASCII, path), 0);
}

bool fileExists(FS_Archive archive, const char * path)
{
	if ((!path) || (!archive))
		return false;
	
	Handle handle;

	if (R_FAILED(FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, 0)))
		return false;

	if (R_FAILED(FSFILE_Close(handle)))
		return false;
	
	return true;
}

bool dirExists(FS_Archive archive, const char * path)
{	
	if ((!path) || (!archive))
		return false;
	
	Handle handle;

	if (R_FAILED(FSUSER_OpenDirectory(&handle, archive, fsMakePath(PATH_ASCII, path))))
		return false;

	if (R_FAILED(FSDIR_Close(handle)))
		return false;
	
	return true;
}