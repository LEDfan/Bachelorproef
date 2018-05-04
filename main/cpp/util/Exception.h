#ifndef STRIDE_EXCEPTION_H
#define STRIDE_EXCEPTION_H

#include <stdexcept>

namespace stride {
namespace util {

/// Basic exception class. Needed to prevent clang-tidy warning: "thrown exception type is not nothrow copy
/// constructible"
class Exception : public std::runtime_error
{
public:
        Exception(const std::string& msg);
};
} // namespace util

} // namespace stride

#endif // STRIDE_EXCEPTION_H
