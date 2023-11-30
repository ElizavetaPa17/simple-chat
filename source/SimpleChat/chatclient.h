#ifndef CHATCLIENT_H
#define CHATCLIENT_H

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

class ChatClient final {
public:
    ChatClient();
    ~ChatClient();

    bool sendLoginInfo(const char* username, const char* password);

private:
    using socket_t = int;

    socket_t client_socket_;

    void setupAddrInfoHints(addrinfo& hints);
};

#endif // CHATCLIENT_H
