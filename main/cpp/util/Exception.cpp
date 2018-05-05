#include "Exception.h"

namespace stride {
namespace util {

Exception::Exception(const std::string& msg) : runtime_error(msg) {}

} // namespace util
} // namespace stride
