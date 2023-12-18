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
#include <string>
#include <map>

#include "constants.h"

class ChatClient final {
private:
    using socket_t = int;

    struct ClientInfo {
        char id[ID_BUFFER_SIZE]{};
        char username[USRNM_BUFFER_SIZE]{};
    };

    socket_t client_socket_;
    ClientInfo client_info_;
    ClientInfo find_user_info_;
    char receiver_id_[ID_BUFFER_SIZE]{};
    char input_buffer_[CLIENT_RECEIVE_BUFFER_SZ];

    bool getAuthRespond();
    bool getFindRespond();
    std::multimap<int, std::pair<QString, QString>> getAllMsgRespond();
    std::vector<QString> getAllSendersIdRespond();

    void setupAddrInfoHints(addrinfo& hints);
    void parseFindRespond();

public:
    ChatClient();
    ~ChatClient();

    bool authorizeUser(const char* username, const char* password, int auth_type);

    const char*       getClientUsername();
    void              prepareReceiverID(const char* id);
    void              sendMessage(const char* text);
    bool              findUser(const char* username);
    const ClientInfo* getFoundUser();

    std::vector<QString> getAllSendersId(bool new_flag);
    std::multimap<int, std::pair<QString, QString>> getNewMessages();
    std::multimap<int, std::pair<QString, QString>> getAllMessages();
};

#endif // CHATCLIENT_H
