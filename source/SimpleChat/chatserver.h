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

class ChatServer {
public:
    ChatServer();
    ~ChatServer();

private:
    using socket_t = int;

    socket_t server_socket_;

    void setupAddrInfoHints(addrinfo& hints);
};

#endif // CHATSERVER_H
