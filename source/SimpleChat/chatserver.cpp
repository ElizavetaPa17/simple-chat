#include "chatserver.h"
#include "constants.h"
#include <cstdio>

ChatServer::ChatServer()
    : server_socket_(-1)
{
    setupServer();
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

void ChatServer::setupServer() {
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

void ChatServer::startAcceptConnection() {
    socket_t max_socket = server_socket_;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(max_socket, &master);

    fprintf(stdout, "Start accepting new connections...\n");

    for (fd_set reads = master, writes = master; true; reads = master, writes = master) {
        if (select(max_socket+1, &reads, NULL, NULL, NULL) < 0) {
            fprintf(stderr, "%s%d%c", "Failed to select: ", errno, 'c');
        } else {
            for (socket_t sngle_socket = 1; sngle_socket <= max_socket; ++sngle_socket) {
                if (FD_ISSET(sngle_socket, &reads)) {
                    handleReadSocket(sngle_socket, max_socket, master);
                }
            }
        }
    }
}

void ChatServer::handleReadSocket(const socket_t sngle_socket, socket_t& max_socket, fd_set& master) {
    if (sngle_socket == server_socket_) {
        socket_t new_socket = handleNewConnection();
        if (ISVALIDSOCKET(new_socket)) {
            FD_SET(new_socket, &master);
            max_socket = new_socket > max_socket ? new_socket : max_socket;
        }
    } else {
        const auto& client = clients_.find(sngle_socket);
        char buffer[RECV_SERVER_BUFFER_SIZE];
        // ЦИКЛ СЧИТЫВАНИЯ, А ТО ПОЛУЧАЕТСЯ 1 ПОРЦИЯ!!
        if (recv(client->first, buffer, sizeof(buffer), 0) > 0) {
            printf("%s", buffer);
        } else {
            printf("Closing connection with the client: %s\n", getClientAddress(client->second));
            clients_.erase(client->first);
            FD_CLR(client->first, &master);
        }
    }
}
void ChatServer::handleWriteSocket(const socket_t sngl_socket) {

}

typename ChatServer::socket_t ChatServer::handleNewConnection() {
    ClientInfo client_info;
    socket_t new_socket = accept(server_socket_, (sockaddr*) &client_info.address, &client_info.address_length);
    if (!ISVALIDSOCKET(new_socket)) {
        fprintf(stderr, "%s%d\n", "Accept failed: ", errno);
        return INVALID_SOCKET;
    } else {
        fprintf(stdout, "%s%s\n", "Accept new connection. Client address: ", getClientAddress(client_info));
        clients_.insert({ new_socket, client_info });
        return new_socket;
    }
}

const char* ChatServer::getClientAddress(const ClientInfo& client_info) {
    static char address_buffer[100];

    if (getnameinfo((sockaddr*) &client_info.address, client_info.address_length,
                 address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST)) {
    }
    return address_buffer;
}
