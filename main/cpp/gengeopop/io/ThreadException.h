#pragma once
#include <boost/optional.hpp>
#include <mutex>

namespace gengeopop {

/**
 * Create a wrapper for capturing the exceptions thrown in a multi-threaded environment
 * This caught exception can then later be re-thrown in the main thread.
 * Based on https://stackoverflow.com/a/13978507/1393103
 */
class ThreadException
{
public:
        /// Create the ThreadException
        ThreadException() : ptr(nullptr), Lock() {}

        /// Destructor
        ~ThreadException() {}

        /// Re-throw the exception if one was caught
        void Rethrow()
        {
                if (this->ptr != nullptr) {
                        std::rethrow_exception(this->ptr);
                }
        }

        /// Capture the current exception and save it
        void CaptureException()
        {
                std::unique_lock<std::mutex> guard(this->Lock);
                this->ptr = std::current_exception();
        }

        /// The used for executing a function with a return type
        template <typename T>
        struct RunType
        {
                using type = boost::optional<T>;
        };

        template <typename T>
        using RunType_t = typename RunType<T>::type;

        /// Run the given function and capture any exception that may be thrown
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

        /// Returns if an exception was caught
        bool HasError() const { return ptr != nullptr; }

private:
        std::exception_ptr ptr;  ///< The caught exception (if none was caught, contains a nullptr)
        std::mutex         Lock; ///< A lock used to prevent two exceptions being caught at the same time
};

template <>
struct ThreadException::RunType<void>
{
        using type = void;
};

} // namespace gengeopop
