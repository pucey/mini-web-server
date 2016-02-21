// https://github.com/jasonish/libevent-examples/blob/master/echo-server/libevent_echosrv1.c
// 1. Do cmake
// 2. Run daemon and try log
// 3. Run http
#include <iostream>
#include <string>
#include <cstdlib>
#include "network/http_exception.hpp"
#include "network/http_server.hpp"

int
main(int argc, char **argv)
{
    if (argc != 7) {
        std::cerr << "Incorrect number of parameters\n";
        std::cerr << "Usage example:\n";
        std::cerr << "  " << argv[0] << " -d <dir> -h <ip> -p <port>\n";
        return EXIT_FAILURE;
    }
    
    std::string root;
    std::string ip;
    int port = -1;    
    for (int i = 1; i < argc; i += 2) {
        std::string option(argv[i]);
        if (option == "-d") {
            root = argv[i + 1];
        } else if (option == "-h") {
            ip = argv[i + 1];
        } else if (option == "-p") {
            //port = std::stoi(std::string(argv[i + 1]));
            port = atoi(argv[i + 1]);
        }
    }

    //if (fork()) {
    //    return EXIT_SUCCESS;
    //}
    
    // child process
    //close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);
    try {
        network::http_server server(root, ip, port);
        server.run();
    } catch (const network::http_exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}