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

#include "constants.h"

class ChatClient final {
public:
    ChatClient();
    ~ChatClient();

    bool sendAuthInfo(const char* username, const char* password, int auth_type);

private:
    using socket_t = int;

    socket_t client_socket_;
    void setupAddrInfoHints(addrinfo& hints);
};

#endif // CHATCLIENT_H
