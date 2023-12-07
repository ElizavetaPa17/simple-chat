#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>

#include "stdlib.h"
#include "stdio.h"

#include "constants.h"

class ChatClient final {
public:
    ChatClient();
    ~ChatClient();

    bool authorizeUser(const char* username, const char* password, int auth_type);

    const char* getClientUsername();

private:
    using socket_t = int;

    struct ClientInfo {
        char username[21]{};
    };


    socket_t client_socket_;
    ClientInfo client_info_;
    char input_buffer_[CLIENT_RECEIVE_BUFFER_SZ];

    bool getAuthRespond();

    void setupAddrInfoHints(addrinfo& hints);
};

#endif // CHATCLIENT_H
