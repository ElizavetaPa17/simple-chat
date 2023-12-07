#include "chatserver.h"
#include "constants.h"
#include <cstdio>

ChatServer::ChatServer()
    : server_socket_(-1)
{
    if (!database_.init()) {
        exit(EXIT_FAILURE);
    }

    setupServer();
}

ChatServer::~ChatServer() {
    fprintf(stdout, "S: Closing the server socket...\n");
    closeConnection();
}

void ChatServer::setupAddrInfoHints(addrinfo& hints) {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}

void ChatServer::closeConnection() {
    close(server_socket_);
}

void ChatServer::setupServer() {
    fprintf(stdout, "S: Configuring the server...\n");
    addrinfo hints;
    setupAddrInfoHints(hints);

    addrinfo* server_address;
    getaddrinfo(SERVER_ADDRESS, SERVER_SERVICE, &hints, &server_address);

    fprintf(stdout, "S: Creating a server socket...\n");
    server_socket_ = socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol);
    if (!ISVALIDSOCKET(server_socket_)) {
        fprintf(stderr, "%s%d%c", "S: Failed to create the server socket: ", errno, '\n');
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "S: Binding the server socket to local address...\n");
    if (bind(server_socket_, server_address->ai_addr, server_address->ai_addrlen)) {
        fprintf(stderr, "%s%d%c","S: Failed to bind the server socket: ", errno, '\n');
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(server_address);

    fprintf(stdout, "S: Start listening...\n");
    if (listen(server_socket_, 10)) {
        fprintf(stderr, "%s%d%c", "S: Failed to listen: ", errno, '\n');
        exit(EXIT_FAILURE);
    }
}

void ChatServer::startAcceptConnection() {
    socket_t max_socket = server_socket_;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(max_socket, &master);

    fprintf(stdout, "S: Start accepting new connections.\n");

    for (fd_set reads = master, writes = master; true; reads = master, writes = master) {
        if (select(max_socket+1, &reads, NULL, NULL, NULL) < 0) {
            fprintf(stderr, "%s%d\n", "S: Failed to select: ", errno);
            exit(EXIT_FAILURE);
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
        if ((recv(client->first, buffer, sizeof(buffer), 0)) > 0) {
            parseReadData(buffer, client->second);
        } else {
            printf("S: Closing connection with the client: %s\n", getClientAddress(client->second));
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
        fprintf(stderr, "%s%d\n", "S: Accept failed: ", errno);
        return INVALID_SOCKET;
    } else {
        fprintf(stdout, "%s%s\n", "S: Accept new connection. Client address: ", getClientAddress(client_info));
        clients_.insert({ new_socket, client_info });
        return new_socket;
    }
}

void ChatServer::parseReadData(char* data, ClientInfo& client) {
    char* p = data;
    if ((p = strstr(data, LOGIN_CONNECTION))) {
        handleLoginConnection(data, client);
    } else if ((p = strstr(data, RGSTR_CONNECTION))) {
        handleRegistrConnection(data, client);
    } else {
        fprintf(stdout, "S: Receive unknown type of connection\n");
    }
}

void ChatServer::handleLoginConnection(char* data, ClientInfo& client) {
    if (!getAuthentInfo(data, client)) {
        fprintf(stderr, "S: Failed to get authentification info about the client: %s\n", 
                         getClientAddress(client));
    } else {
        fprintf(stdout, "S: Trying to login the client...\n");
        if (database_.isUserExist(client.username, client.password)) {
            fprintf(stdout, "Successfull client login.\n");
        } else {
            fprintf(stdout, "S: Failed to login the client.\n");
        }
    }
}

void ChatServer::handleRegistrConnection(char* data, ClientInfo& client) {
    if (!getAuthentInfo(data, client)) {
        fprintf(stderr, "S: Failed to get authentification info about the client: %s\n", 
                         getClientAddress(client));
    } else {
        fprintf(stdout, "S: Trying to register the client...\n");
        if (!database_.addUser(client.username, client.password)) {
            // reset client username and password
            memset(client.username, '\0', sizeof(client.username));
            memset(client.password, '\0', sizeof(client.password));
        }
    }
}

bool ChatServer::sendSuccessRespond(const char* respond) {
    size_t rspnd_sz = strlen(respond);
    if (send(server_socket_, respond, rspnd_sz, 0) < 0) {
        fprintf(stderr, "S: Failed to send success respond: %d\n", errno);
        return false;
    }

    return true;
}

bool ChatServer::getAuthentInfo(char* data, ClientInfo& client) {
    char* p = data;
    const char* usr = NULL, *pswrd = NULL;
    size_t usr_sz = 0, pswrd_sz = 0;
    if ((p = strstr(p, "Username: "))) {
        usr = p + sizeof("Username:");
        p = strstr(p, "\n");
       *p = '\0';
        usr_sz = p - usr;
    } else {
        fprintf(stdout, "S: Receive wrong authentification request format.\n");
        return false;
    }
        
    ++p;
    if ((p = strstr(p, "Password: "))) {
        pswrd = p + sizeof("Password:");
        p = strstr(p, "\n");
       *p = '\0';
        pswrd_sz = p - pswrd;
    } else {
        fprintf(stdout, "S: Receive wrong authentification request format.\n");
        return false;
    }

    memcpy(client.username, usr, usr_sz);
    memcpy(client.password, pswrd, pswrd_sz);
    return true;
}

const char* ChatServer::getClientAddress(const ClientInfo& client_info) {
    static char address_buffer[100];

    if (getnameinfo((sockaddr*) &client_info.address, client_info.address_length,
                 address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST)) {
    }
    return address_buffer;
}
