#pragma once

#include <string>
#include <map>

namespace network
{
    class http_server
    {
    public:
        http_server(std::string root_dir, const std::string& ip_address, int port);
        void run();

    private:
        enum http_response_t {
            HTTP_OK = 200,
            HTTP_NOT_FOUND = 404
        };
    
        std::string m_root;
        int m_listen_fd;
        
    private:
        static const std::map<http_response_t, std::string> responseToString;
        
        static void on_read(int fd, short ev, void *arg);
        static int  set_non_block(int fd);
        static void proceed_get_request(int fd, const std::string& root, const std::string& req);
        static std::string build_response(http_response_t response, const std::string& data);
    };
}
