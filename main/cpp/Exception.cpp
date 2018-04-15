#include "Exception.h"
Exception::Exception(const std::string& msg) : runtime_error(msg) {}
