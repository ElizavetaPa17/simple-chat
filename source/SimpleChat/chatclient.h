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
private:
    using socket_t = int;

    struct ClientInfo {
        char id[9]{};
        char username[21]{};
    };

    socket_t client_socket_;
    ClientInfo client_info_;
    ClientInfo find_user_info_;
    char input_buffer_[CLIENT_RECEIVE_BUFFER_SZ];

    bool getAuthRespond();
    bool getFindRespond();

    void setupAddrInfoHints(addrinfo& hints);
    void parseFindRespond();

public:
    ChatClient();
    ~ChatClient();

    bool authorizeUser(const char* username, const char* password, int auth_type);

    const char*       getClientUsername();

    bool              findUser(const char* username);
    const ClientInfo* getFoundUser();

};

#endif // CHATCLIENT_H
