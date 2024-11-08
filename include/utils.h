#pragma once

#include <string>

namespace Utils {
    bool IsNew3DS(void);
    void GetSizeString(char *string, u64 size);
    std::string GetSubstring(const std::string& str, const std::string& str1, const std::string& str2);
    void UTF16ToUTF8(u8 *buf, const u16 *data, size_t length);
}
