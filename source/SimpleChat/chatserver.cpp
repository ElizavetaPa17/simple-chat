#include "chatserver.h"

#include "constants.h"

ChatServer::ChatServer()
    : server_socket_(-1)
{

    fprintf(stdout, "Configuring the server...\n");
    addrinfo hints;
    setupAddrInfoHints(hints);

    addrinfo* server_address;
    getaddrinfo(SERVER_ADDRESS, SERVER_SERVICE, &hints, &server_address);

    fprintf(stdout, "Creating a server socket...\n");
    server_socket_ = socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol);
    if (!ISVALIDSOCKET(server_socket_)) {
        fprintf(stderr, "%s%d%c", "Failed to create the server socket: ", errno, '\n');
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Binding the server socket to local address...\n");
    if (bind(server_socket_, server_address->ai_addr, server_address->ai_addrlen)) {
        fprintf(stderr, "%s%d%c","Failed to bind the server socket: ", errno, '\n');
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(server_address);

    fprintf(stdout, "Start listening...\n");
    if (listen(server_socket_, 10)) {
        fprintf(stderr, "%s%d%c", "Failed to listen: ", errno, '\n');
        exit(EXIT_FAILURE);
    }
}

ChatServer::~ChatServer() {
    fprintf(stdout, "Closing the server socket...\n");
    close(server_socket_);
}

void ChatServer::setupAddrInfoHints(addrinfo& hints) {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}
