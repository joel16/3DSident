#include <3ds.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>

FS_Archive sdmcArchive;

void openSdArchive();
void closeSdArchive();
int makeDir(const char * path);
bool fileExists(char * path);
bool dirExists(const char * path);
bool deleteFile(const char *path);