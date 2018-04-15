#ifndef STRIDE_EXCEPTION_H
#define STRIDE_EXCEPTION_H

#include <stdexcept>

class Exception : public std::runtime_error
{
public:
        Exception(const std::string& msg);
};

#endif // STRIDE_EXCEPTION_H
