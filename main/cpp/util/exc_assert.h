#pragma once
#include <Exception.h>
#include <string>

#ifdef NDEBUG
inline void exc_assert(bool, const std::string&) {}
#else
inline void exc_assert(bool condition, const std::string& error)
{
        if (!condition) {
                throw Exception(error);
        }
}
#endif
