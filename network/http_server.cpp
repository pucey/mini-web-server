#include "http_server.hpp"
#include "http_exception.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>

#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace
{
    std::ofstream LOG("/tmp/webserver.log", std::ios_base::app);

    void thread_proc(int fd, std::string req)
    {
        LOG << "client (" << fd << "), req = " << req << std::endl;
    }
}

namespace network
{
    const std::map<http_server::http_response_t, std::string> http_server::responseToString = {
        {HTTP_OK, "OK"},
        {HTTP_NOT_FOUND, "NOT FOUND"}
    };
    
    http_server::http_server(std::string root_dir, const std::string& ip_address, int port)
        : m_root(std::move(root_dir))
    {
        if (m_root.back() == '/') {
            // root should be without /
            m_root.pop_back();
        }
        LOG << "HTTP Server params: root = " << m_root << "; ip = " << ip_address << "; port = " << port << std::endl;
        event_init();

        m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_listen_fd < 0) {
            throw http_exception("[-] socket failed");
        }
        int reuseaddr_on = 1;
        if (setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on, sizeof(reuseaddr_on)) == -1) {
            throw http_exception("[-] setsockopt failed");
        }

        sockaddr_in listen_addr = {0};            
        listen_addr.sin_family = AF_INET;
        //listen_addr.sin_addr.s_addr = INADDR_ANY;
        listen_addr.sin_addr.s_addr = inet_addr(ip_address.c_str());
        listen_addr.sin_port = htons(port);
        if (bind(m_listen_fd, (sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
            throw http_exception("[-] bind failed");
        }        
    }
    
    void
    http_server::run()
    {
        if (listen(m_listen_fd, SOMAXCONN) < 0) {
            throw http_exception("[-] listen failed");
        }

        if (set_non_block(m_listen_fd) < 0) {
            throw http_exception("[-] set_non_block failed");
        }
        
        LOG << "HTTP server successfully started" << std::endl;

        event ev_accept;
        event_set(&ev_accept, m_listen_fd, EV_READ|EV_PERSIST, on_accept, this);
        //event_set(&ev_accept, m_listen_fd, EV_READ|EV_PERSIST, on_accept, NULL);
        event_add(&ev_accept, NULL);

        event_dispatch();
    }
    
    void
    http_server::on_accept(int fd, short ev, void *arg)
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            throw http_exception("accept failed");
        }

        if (set_non_block(client_fd) < 0) {
            LOG << "failed to set client socket non-blocking" << std::endl;
        }

        LOG << "accept = " << std::this_thread::get_id() << std::endl;
        //std::thread t([arg, client_fd](){
            LOG << "thread inside accept = " << std::this_thread::get_id() << std::endl;
            client_t* client = new client_t();
            client->pHttp_server = reinterpret_cast<http_server*>(arg);
            event_set(&client->ev_read, client_fd, EV_READ|EV_PERSIST, on_read, client);
            event_add(&client->ev_read, NULL);
        //});
        //t.join();

        LOG << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;
    }
    
    void
    http_server::on_read(int fd, short ev, void *arg)
    {
        client_t *client = (client_t*)arg;
        u_char buf[8196] = {0};

        int len = read(fd, buf, sizeof(buf));
        if (len <= 0) {
            if (len == 0) {
                LOG << "Client disconnected." << std::endl;
            } else {
                LOG << "Socket failure, disconnecting client" << std::endl;
            }
            close(fd);
            event_del(&client->ev_read);
            delete client;
            return;
        }

        std::string request(buf, buf + len);
        if (request.find("GET") == 0) {
            proceed_get_request(fd, client->pHttp_server->m_root, request);
			shutdown(fd, SHUT_RDWR);
			event_del(&client->ev_read);
			delete client;
        }
    }

    void
    http_server::proceed_get_request(int fd, const std::string& root, const std::string& req)
    {
        LOG << "REQUEST" << std::endl;
        LOG << req << std::endl;
        std::istringstream iss(req);
        std::string temp;
        std::string file;
        iss >> temp >> file;
        
        const auto pos = file.find('?');
        if (pos != std::string::npos) {
            file = file.substr(0, pos);
        }
        const std::string path = root + file;
        LOG << "path = " << path << std::endl;
        std::ifstream in(path);
        if (in) {
            const std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            const auto response = build_response(HTTP_OK, content);
            LOG << response << std::endl;
            write(fd, response.c_str(), response.size());
        } else {
            const auto response = build_response(HTTP_NOT_FOUND, "<H1>NOT FOUND</H1>");
            LOG << response << std::endl;
            write(fd, response.c_str(), response.size());
        }
    }
    
    std::string
    http_server::build_response(http_server::http_response_t response, const std::string& data)
    {
        std::ostringstream oss;
        oss << "HTTP/1.0 " << response << " " << responseToString.find(response)->second << "\r\n";
        if (data.size()) {
			oss << "Connection: close\r\n";
            oss << "Content-Length: " << data.size() << "\r\n";
		}
		oss << "Content-Type: text/html\r\n\r\n" << data;
        return oss.str();
    }
    
    int
    http_server::set_non_block(int fd)
    {
        int flags = fcntl(fd, F_GETFL);
        if (flags < 0) {
            return flags;
        }
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, flags) < 0) {
            return -1;
        }            
        return 0;
    }
}
