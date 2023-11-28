#include "chatserver.h"

#define ISVALIDSOCKET(s) ((s) >= 0)

const char* SERVER_ADDRESS = NULL;
const char* SERVER_SERVICE = "8080";

ChatServer::ChatServer()
    : server_socket_(-1)
{

    fprintf(stdout, "Configuring the server...");
    addrinfo hints;
    setupAddrInfoHints(hints);

    addrinfo* server_address;
    getaddrinfo(SERVER_ADDRESS, SERVER_SERVICE, &hints, &server_address);

    fprintf(stdout, "Creating a server socket...");
    server_socket_ = socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol);
    if (!ISVALIDSOCKET(server_socket_)) {
        fprintf(stderr, "%s%d", "Failed to create the server socket: ", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Binding the server socket to local address...");
    if (bind(server_socket_, server_address->ai_addr, server_address->ai_addrlen)) {
        fprintf(stderr, "%s%d","Failed to bind the server socket: ", errno);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(server_address);

    fprintf(stdout, "Start listening...");
    if (listen(server_socket_, 10)) {
        fprintf(stderr, "%s%d", "Failed to listen: ", errno);
        exit(EXIT_FAILURE);
    }
}

ChatServer::~ChatServer() {
    fprintf(stdout, "Closing the server socket...");
    close(server_socket_);
}

void ChatServer::setupAddrInfoHints(addrinfo& hints) {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}
