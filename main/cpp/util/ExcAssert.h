#pragma once
#include <Exception.h>
#include <string>

#ifdef NDEBUG
inline void ExcAssert(bool, const std::string&) {}
#else
inline void ExcAssert(bool condition, const std::string& error)
{
        if (!condition) {
                throw Exception(error);
        }
}
#endif
