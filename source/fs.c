#include "fs.h"

void openSdArchive()
{
	FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
}

void closeSdArchive()
{
	FSUSER_CloseArchive(sdmcArchive);
}

int makeDir(const char *path)
{
	if (!path) 
		return -1;
	
	return mkdir(path, 0777);
}

bool fileExists(char *path) 
{
    FILE * temp = fopen(path, "r");
    if(temp == NULL)
        return false;

    fclose(temp);

    return true;
}

bool dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

bool deleteFile(const char *path) 
{
	openSdArchive();
	Result ret = FSUSER_DeleteFile(sdmcArchive, fsMakePath(PATH_ASCII, path));
	closeSdArchive();
	return ret == 0;
}