#include "http_exception.hpp"

namespace network
{
    http_exception::http_exception(const char* message) : m_message(message)
    {}
        
    http_exception::http_exception(std::string message) : m_message(std::move(message))
    {}
        
    const char*
    http_exception::what() const noexcept
    {
        return (std::string("http_exception: ") + m_message).c_str();
    }
}
