#pragma once
#include <boost/optional.hpp>
#include <mutex>

namespace gengeopop {

// Based on https://stackoverflow.com/a/13978507/1393103
class ThreadException
{
public:
        ThreadException() : ptr(nullptr), Lock() {}
        ~ThreadException() {}

        void Rethrow()
        {
                if (this->ptr != nullptr) {
                        std::rethrow_exception(this->ptr);
                }
        }
        void CaptureException()
        {
                std::unique_lock<std::mutex> guard(this->Lock);
                this->ptr = std::current_exception();
        }

        template <typename Function, typename... Parameters,
                  typename ReturnType = decltype(std::declval<Function>()(std::declval<Parameters>()...))>
        std::enable_if_t<!std::is_same<ReturnType, void>::value, boost::optional<ReturnType>> Run(Function f,
                                                                                                  Parameters... params)
        {
                try {
                        return f(params...);
                } catch (...) {
                        CaptureException();
                }
                return {};
        }

        template <typename Function, typename... Parameters,
                  typename ReturnType = decltype(std::declval<Function>()(std::declval<Parameters>()...))>
        std::enable_if_t<std::is_same<ReturnType, void>::value, void> Run(Function f, Parameters... params)
        {
                try {
                        f(params...);
                } catch (...) {
                        CaptureException();
                }
        }

        bool HasError() const { return ptr != nullptr; }

private:
        std::exception_ptr ptr;
        std::mutex         Lock;
};

} // namespace gengeopop
