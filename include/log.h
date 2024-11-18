#pragma once

#include <3ds.h>

namespace Log {
    Result Open(void);
    Result Close(void);
    void Error(const char *data, ...);
}
