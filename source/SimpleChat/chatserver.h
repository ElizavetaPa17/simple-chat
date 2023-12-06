#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "stdlib.h"
#include "stdio.h"

#include <map>

#include "dbutility.h"

class ChatServer final {
public:
    ChatServer();
    ~ChatServer();

    void startAcceptConnection();
    void closeConnection();

private:
    using socket_t = int;

    struct ClientInfo {
        sockaddr_storage address;
        socklen_t        address_length = sizeof(address);
    };

    socket_t server_socket_;
    std::map<socket_t, ClientInfo> clients_;
    DbUtility database_;

    void setupServer();
    void setupAddrInfoHints(addrinfo& hints);

    void handleReadSocket(const socket_t sngle_socket, socket_t& max_socket, fd_set& master);
    void handleWriteSocket(const socket_t sngle_socket);

    socket_t handleNewConnection();
    void     parseReadData(char* data, size_t data_sz);

    const char* getClientAddress(const ClientInfo& client_info);
};

#endif // CHATSERVER_H
