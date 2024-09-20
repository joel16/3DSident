#pragma once

#include <string>

namespace Utils {
    bool IsNew3DS(void);
    void GetSizeString(char *string, u64 size);
    std::string GetSubstring(const std::string &text, const std::string &start, const std::string &end);
    void UTF16ToUTF8(u8 *buf, const u16 *data, size_t length);
}
