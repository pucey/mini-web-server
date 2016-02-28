#include <iostream>
#include <string>
#include <unistd.h>
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
	int opt = 0;
	while ((opt = getopt(argc, argv, "h:p:d:t")) != -1) {
		switch (opt) {
		case 'h': ip = optarg; break;
		case 'p': port = std::stoi(optarg); break;
		case 'd': root = optarg; break;
		}
	}


    if (fork()) {
        return EXIT_SUCCESS;
    }
    
    // child process
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    try {
        network::http_server server(root, ip, port);
        server.run();
    } catch (const network::http_exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
