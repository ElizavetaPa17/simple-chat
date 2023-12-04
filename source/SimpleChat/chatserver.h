#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "stdlib.h"
#include <cstring>
#include "stdio.h"
#include <errno.h>

#include <map>

class ChatServer final {
public:
    ChatServer();
    ~ChatServer();

    void startAcceptConnection();

private:
    using socket_t = int;

    struct ClientInfo {
        sockaddr_storage address;
        socklen_t        address_length = sizeof(address);
    };

    socket_t server_socket_;
    std::map<socket_t, ClientInfo> clients_;

    void setupServer();
    void setupAddrInfoHints(addrinfo& hints);

    void handleReadSocket(const socket_t sngle_socket, socket_t& max_socket, fd_set& master);
    void handleWriteSocket(const socket_t sngle_socket);

    socket_t handleNewConnection();

    const char* getClientAddress(const ClientInfo& client_info);
};

#endif // CHATSERVER_H
