#include <3ds.h>

#include "utils.h"

namespace Utils {
    bool IsNew3DS(void) {
        bool check = false;

        if (R_FAILED(APT_CheckNew3DS(std::addressof(check)))) {
            return false;
        }
        
        return check;
    }

    // Thanks TheOfficialFloW
    void GetSizeString(char *string, u64 size) {
        double double_size = static_cast<double>(size);
        int i = 0;
        const char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
        
        while (double_size >= 1024.0f) {
            double_size /= 1024.0f;
            i++;
        }
        
        std::sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
    }
    
    std::string GetSubstring(const std::string& str, const std::string& str1, const std::string& str2) {
        size_t pos1 = str.find(str1);
        if (pos1 != std::string::npos) {
            size_t pos2 = str.find(str2, pos1 + str1.length());
            
            if (pos2 != std::string::npos) {
                size_t length = pos2 - (pos1 + str1.length());
                return str.substr(pos1 + str1.length(), length);
            }
        }
        
        return "";
    }
    
    void UTF16ToUTF8(u8 *buf, const u16 *data, size_t length) {
        ssize_t units = utf16_to_utf8(buf, data, length);

        if (units < 0) {
            units = 0;
        }
        
        buf[units] = 0;
    }
}
