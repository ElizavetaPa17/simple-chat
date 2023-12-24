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

#include <QString>

#include "constants.h"

class ChatClient final {
private:
    using socket_t = int;

    socket_t client_socket_;
    UserInfo client_info_;
    UserInfo find_user_info_;
    char input_buffer_[CLIENT_RECEIVE_BUFFER_SZ];
    char new_msg_buffer_[CLIENT_RECEIVE_BUFFER_SZ];

    bool getAuthRespond();
    bool getFindRespond();
    std::vector<FetchedMessage> getAllSendersMessagesResponse();
    std::vector<QString> getAllSendersIdRespond();

    void setupAddrInfoHints(addrinfo& hints);
    void parseFindRespond();

    void acceptingNewMessages();

public:
    ChatClient();
    ~ChatClient();

    bool authorizeUser(const char* username, const char* password, int auth_type);

    const UserInfo&   getClientInfo();
    void              sendMessage(const char* text);
    bool              findUser(const char* user_info, bool is_username);
    const UserInfo*   getFoundUser();

    std::vector<UserInfo> getAllSendersInfo(bool new_flag);
    std::vector<FetchedMessage> getAllSenderMessages();
};

#endif // CHATCLIENT_H
