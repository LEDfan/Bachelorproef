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

        template <typename T>
        struct RunType
        {
                using type = boost::optional<T>;
        };

        template <typename T>
        using RunType_t = typename RunType<T>::type;

        template <typename Function, typename... Parameters>
        auto Run(Function f, Parameters... params)
        {
                try {
                        return f(params...);
                } catch (...) {
                        CaptureException();
                }
                return RunType_t<std::result_of_t<Function(Parameters...)>>();
        }

        bool HasError() const { return ptr != nullptr; }

private:
        std::exception_ptr ptr;
        std::mutex         Lock;
};

template <>
struct ThreadException::RunType<void>
{
        using type = void;
};

} // namespace gengeopop
