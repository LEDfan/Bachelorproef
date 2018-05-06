#pragma once
#include <string>
#include <util/Exception.h>

#ifdef NDEBUG
inline void ExcAssert(bool, const std::string&) {}
#else
inline void ExcAssert(bool condition, const std::string& error)
{
        if (!condition) {
                throw stride::util::Exception(error);
        }
}
#endif
