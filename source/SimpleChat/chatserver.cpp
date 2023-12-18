#include "chatserver.h"
#include <cstring>

#include <vector>
#include <sstream>

#include "constants.h"

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
        fprintf(stderr, "%s%d%c", "S: Failed to create the server socket: .", errno, '\n');
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
            parseReadData(buffer, client->first, client->second);
        } else {
            fprintf(stdout, "S: Closing connection with the client: %s\n", getClientAddress(client->second));
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

void ChatServer::parseReadData(char* data, socket_t sngl_socket, ClientInfo& client) {
    if (strstr(data, LOGIN_CONNECTION)) {
        handleLoginConnection(data, sngl_socket, client);
    } else if (strstr(data, RGSTR_CONNECTION)) {
        handleRegistrConnection(data, sngl_socket, client);
    } else if (strstr(data, FIND_CONNECTION)) {
        handleFindConnection(data, sngl_socket);
    } else if (strstr(data, SEND_CONNECTION)) {
        handleSendConnection(data, sngl_socket);
    } else if (strstr(data, GTAMS_FRID_CONNECTION)) {
        handleGetSendersInfoConnection(data, sngl_socket, false);
    } else if (strstr (data, GTNMS_CONNECTION)) {
        handleGetMessagesConnection(data, sngl_socket, true);
    } else if (strstr (data, GTAMS_CONNECTION)) {
        handleGetMessagesConnection(data, sngl_socket, false);
    } else {
        fprintf(stdout, "S: Receive unknown type of connection\n");
    }
}

void ChatServer::handleLoginConnection(char* data, socket_t sngl_socket, ClientInfo& client) {
    if (!getAuthentInfo(data, client)) {
        fprintf(stderr, "S: Failed to get authentification info about the client: %s\n", 
                         getClientAddress(client));
    } else {
        fprintf(stdout, "S: Trying to login the client...\n");
        if (database_.isUserExist(client.username, client.password)) {
            fprintf(stdout, "S: Successfull client login.\n");
            const UserInfo* user_info;
            user_info = database_.getUserInfoByUsername(client.username);

            char message[30]{};
            sprintf(message, "DATA: \nid: %s\n", user_info->id);
            sendRespond(message, OK_RSPND, sngl_socket);
        } else {
            fprintf(stdout, "S: Failed to login the client.\n");

            memset(client.username, '\0', sizeof(client.username));
            memset(client.password, '\0', sizeof(client.password));
            sendRespond(NULL, AUTH_ERROR_RSPND, sngl_socket);
        }
    }
}

void ChatServer::handleRegistrConnection(char* data, socket_t sngl_socket, ClientInfo& client) {
    if (!getAuthentInfo(data, client)) {
        fprintf(stderr, "S: Failed to get authentification info about the client: %s\n", 
                         getClientAddress(client));
    } else {
        fprintf(stdout, "S: Trying to register the client...\n");
        if (!database_.addUser(client.username, client.password)) {
            // reset client username and password
            memset(client.username, '\0', sizeof(client.username));
            memset(client.password, '\0', sizeof(client.password));

            sendRespond(NULL, AUTH_ERROR_RSPND, sngl_socket);
        } else {
            const UserInfo* user_info;
            user_info = database_.getUserInfoByUsername(client.username);

            char message[30]{};
            sprintf(message, "DATA: \nid: %s\n", user_info->id);
            sendRespond(message, OK_RSPND, sngl_socket);
        }
    }
}

void ChatServer::handleFindConnection(char* data, socket_t sngl_socket) {
    static char message[150]{};

    char *p = NULL, *info = NULL;
    const UserInfo* user_info = NULL;
    size_t info_sz = 0;

    if ((p = strstr(data, "Username: "))) {
        info = p + sizeof("Username: ")-1;
        p = strstr(p, "\n");
        *p = '\0';
        info_sz = p - info;

        if (info_sz > USRNM_BUFFER_SIZE) {
            fprintf(stderr, "S: Username is too long for searching.\n");
            sendRespond(NULL, RQST_ERROR_RSPND, sngl_socket);
        }
        
        user_info = database_.getUserInfoByUsername(info);
    } else if ((p = strstr(data, "Id: "))) {
        info = p + sizeof("Id: ")-1;
        p = strstr(p, "\n");
        *p = '\0';
        info_sz = p - info;

        if (info_sz > USRNM_BUFFER_SIZE) {
            fprintf(stderr, "S: Username is too long for searching.\n");
            sendRespond(NULL, RQST_ERROR_RSPND, sngl_socket);
        }

        user_info = database_.getUserInfoById(info);
    } else {
        fprintf(stderr, "S: Incorrect find request received.\n\n");
        sendRespond(NULL, RQST_ERROR_RSPND, sngl_socket);
    }
    
    if (user_info != NULL) {
        size_t offset = sizeof("DATA: \nid: ")-1;
        memcpy(message, "DATA: \n"
                        "id: ", offset);
        memcpy(message + offset, user_info->id, strlen(user_info->id));
        offset += strlen(user_info->id);
                
        memcpy(message + offset, "\nUsername: ", sizeof("\nUsername: ")-1);
        offset += sizeof("\nUsername: ")-1;

        memcpy(message + offset, user_info->username, strlen(user_info->username));
        offset += strlen(user_info->username);
        message[offset] = '\n';
        message[++offset] = '\0';
                
        sendRespond(message, OK_RSPND, sngl_socket);
    } else {
        sendRespond(NULL, NTFD_ERROR_RSPND, sngl_socket);
    }
}

// TODO ADD HANDLE WRONG REQUEST FORMAT
void ChatServer::handleSendConnection(char* data, socket_t sngl_socket) {
    static char buffer[SERVER_RESPOND_BUFFER_SZ];

    char *from_id = nullptr;
    char *to_id   = nullptr;
    char *date    = nullptr;
    char *text    = nullptr;

    text = strstr(data, "Text: ") + sizeof("Text: ")-1;

    to_id = strstr(data, "RECEIVER_ID: ") + sizeof("RECEIVER_ID: ")-1;
    to_id[strstr(to_id, "\n")-to_id] = '\0';

    from_id = strstr(data, "SENDER_ID: ") + sizeof("SENDER_ID: ")-1;
    from_id[strstr(from_id, "\n")-from_id] = '\0';

    date = strstr(data, "DATE: ") + sizeof("DATE: ")-1;
    date[strstr(date, "\n")-date] = '\0';
    
    database_.addMessage(from_id, to_id, date, text);
}

void ChatServer::handleGetSendersInfoConnection(char* data, socket_t sngl_socket, bool new_flag) {
    char* p_id = data;
    p_id = strstr(p_id, "Receiver_id: ") + sizeof("Receiver_id ")-1;
    p_id[strstr(p_id, "\n")-p_id] = '\0';

    std::vector<std::string> senders_id = database_.getAllSendersId(p_id, new_flag);
    std::stringstream ss;
    for (int i = 0; i < senders_id.size(); ++i) {
        ss << "DATA: \nSender_id: " << senders_id[i] << '\n';

        if (ss.str().size() > SERVER_RESPOND_BUFFER_SZ-50) {
            sendRespond(ss.str().c_str(), OK_RSPND, sngl_socket);
            ss = std::stringstream();
        }
    }

    if (ss.str().size() != 0) {
        sendRespond(ss.str().c_str(), ENDSND_RSPND, sngl_socket);
    } else {
        sendRespond(NULL, ENDSND_RSPND, sngl_socket);
    }
}

void ChatServer::handleGetMessagesConnection(char *data, socket_t sngl_socket, bool new_flag) {
    char* p_id = data;
    p_id = strstr(p_id, "id: ") + sizeof("id: ")-1;
    p_id[strstr(p_id, "\n")-p_id] = '\0';

    std::vector<FetchedMessage> messages = database_.getAllMessages(p_id, new_flag);
    std::stringstream ss;
    for (int i = 0; i < messages.size(); ++i) {
        ss << "DATA: \nSender_id: " << messages[i].from_id;
        ss << "\nDate: " << messages[i].date;
        ss << "\nText: " << messages[i].text << TEXT_END_IDENTIF;
        
        if (ss.str().size() > SERVER_RESPOND_BUFFER_SZ-50) {
            sendRespond(ss.str().c_str(), OK_RSPND, sngl_socket);
            ss = std::stringstream();
        }
    }
    
    if (ss.str().size() != 0) {
        sendRespond(ss.str().c_str(), ENDSND_RSPND, sngl_socket);
    } else {
        sendRespond(NULL, ENDSND_RSPND, sngl_socket);
    }
}

bool ChatServer::sendRespond(const char* msg, RespondCode repsond, socket_t sngl_socket) {
    static char buffer[SERVER_RESPOND_BUFFER_SZ];

    size_t offset = sizeof("CODE: ")-1;
    memcpy(buffer, "CODE: ", offset);

    switch (repsond) {
        case OK_RSPND: 
            memcpy(buffer + offset, "OK\n", sizeof("OK\n")-1);
            offset += sizeof("OK\n")-1;
            break;
        case ENDSND_RSPND:
            memcpy(buffer + offset, "ENDSND\n", sizeof("ENDSND\n")-1);
            offset += sizeof("ENDSND\n")-1;
            break;
        case AUTH_ERROR_RSPND:
            memcpy(buffer + offset, "AUTH_ERROR\n", sizeof("AUTH_ERROR\n")-1);
            offset += sizeof("AUTH_ERROR\n")-1;
            break;
        case RQST_ERROR_RSPND:
            memcpy(buffer + offset, "RQST_ERROR\n", sizeof("RQST_ERROR\n")-1);
            offset += sizeof("AUTH_ERROR\n")-1;
            break;
        case SRVR_ERROR_RSPND:
            memcpy(buffer + offset, "SRVR_ERROR\n", sizeof("SRVR_ERROR\n")-1);
            offset += sizeof("SRVR_ERROR\n")-1;
            break;
        case NTFD_ERROR_RSPND:
            memcpy(buffer + offset, "NTFD_ERROR\n", sizeof("NTFD_ERROR\n")-1);
            offset += sizeof("NTFD_ERROR\n")-1;
            break;
        default:
            fprintf(stderr, "S: Failed to send respond: unknown respond type.\n");
            return false;
    }

    if (msg) {
        size_t msg_sz = strlen(msg);
        if (SERVER_RESPOND_BUFFER_SZ - offset - msg_sz < 2) {
            fprintf(stderr, "S: Failed to send respond: message is too long.");
            return false;
        }

        memcpy(buffer + offset, msg, msg_sz);
        offset += msg_sz+1;
    } 

    buffer[offset] = '\0';
    if (send(sngl_socket, buffer, offset+1, 0) < 0) {
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

    // set checking for usr_sz && pswrd_sz <= 21
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
