#include <3ds.h>
#include <cstring>

#include "fs.h"
#include "kernel.h"
#include "system.h"
#include "utils.h"

namespace Kernel {
    const char *GetInitalVersion(void) {
        Result ret = 0;
        
        FS_Archive archive;
        if (R_FAILED(ret = FS::OpenArchive(std::addressof(archive), ARCHIVE_NAND_TWL_FS))) {
            return "unknown";
        }
        
        Handle handle;
        if (R_FAILED(ret = FSUSER_OpenFileDirectly(&handle, ARCHIVE_NAND_TWL_FS, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, "/sys/log/product.log"), FS_OPEN_READ, 0))) {
            return "unknown";
        }

        u64 size = 0;
        if (R_FAILED(ret = FSFILE_GetSize(handle, std::addressof(size)))) {
            return "unknown";
        }

        char *buf = new char[size + 1];
        u32 bytesRead = 0;
        
        if (R_FAILED(ret = FSFILE_Read(handle, std::addressof(bytesRead), 0, reinterpret_cast<u32 *>(buf), static_cast<u32>(size)))) {
            return "unknown";
        }
        
        buf[size] = '\0';
        
        // New 3DS/2DS only
        static char version[11];
        std::strcpy(version, Utils::GetSubstring(buf, "cup:", " preInstall:").c_str());
        
        if (std::strlen(version) == 0) {
            std::strcpy(version, Utils::GetSubstring(buf, "cup:", ",").c_str());
        }
        
        std::strcat(version, "-");
        std::strcat(version, Utils::GetSubstring(buf, "nup:", " cup:").c_str());

        if (R_FAILED(ret = FSFILE_Close(handle))) {
            return "unknown";
        }

        delete[] buf;
        FS::CloseArchive(archive);
        return version;
    }

    const char *GetVersion(VersionInfo info) {
        Result ret = 0;
        u32 osVersion = osGetKernelVersion();
        
        OS_VersionBin *nver = new OS_VersionBin[sizeof(OS_VersionBin)];
        OS_VersionBin *cver = new OS_VersionBin[sizeof(OS_VersionBin)];

        static char kernelString[128];
        static char firmString[128];
        static char systemVersionString[128];
        
        std::snprintf(kernelString, 128, "%lu.%lu-%lu",
            GET_VERSION_MAJOR(osVersion),
            GET_VERSION_MINOR(osVersion),
            GET_VERSION_REVISION(osVersion)
        );
        
        std::snprintf(firmString, 128, "%lu.%lu-%lu",
            GET_VERSION_MAJOR(osVersion),
            GET_VERSION_MINOR(osVersion),
            GET_VERSION_REVISION(osVersion)
        );

        if (R_FAILED(ret = osGetSystemVersionDataString(nver, cver, systemVersionString, sizeof(systemVersionString)))) {
            std::snprintf(systemVersionString, 128, "unknown");
        }

        if (info == VERSION_INFO_KERNEL) {
            return kernelString;
        }
        else if (info == VERSION_INFO_FIRM) {
            return firmString;
        }

        return systemVersionString;
    }

    const char *GetSdmcCid(void) {
        Result ret = 0;
        u8 buf[16];
        
        if (R_FAILED(ret = FSUSER_GetSdmcCid(buf, 0x10))) {
            return "unknown";
        }
        
        static char cid[33];
        for (int i = 0; i < 16; ++i) {
            std::snprintf(cid + i * 2, 3, "%02X", buf[i]);
        }
        
        cid[32] = '\0';
        return cid;
    }

    const char *GetNandCid(void) {
        Result ret = 0;
        u8 buf[16];
        
        if (R_FAILED(ret = FSUSER_GetNandCid(buf, 0x10))) {
            return "unknown";
        }
        
        static char cid[33];
        for (int i = 0; i < 16; ++i) {
            std::snprintf(cid + i * 2, 3, "%02X", buf[i]);
        }
        
        cid[32] = '\0';
        return cid;
    }

    u32 GetDeviceId(void) {
        Result ret = 0;
        u32 id = 0;

        if (R_FAILED(ret = AM_GetDeviceId(std::addressof(id)))) {
            return ret;
        }
        
        return id;
    }
}
