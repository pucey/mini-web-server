#pragma once

#include <stdexcept>
#include <string>

namespace network
{
    class http_exception : public std::exception
    {        
    public:
        explicit http_exception(const char* message);        
        explicit http_exception(std::string message);

        virtual const char* what() const noexcept;

    private:
        std::string m_message;
    };
}
