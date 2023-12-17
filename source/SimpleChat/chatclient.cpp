#include "chatclient.h"

#include <QDateTime>
#include "constants.h"

ChatClient::ChatClient()
    : client_socket_(-1)
{
    fprintf(stdout, "C: Configuring remote connection...\n");
    addrinfo hints;
    setupAddrInfoHints(hints);

    addrinfo* remote_address;
    if (getaddrinfo(REMOTE_ADDRESS, REMOTE_SERVICE, &hints, &remote_address)) {
        fprintf(stderr, "%s%d%c", "C: Failed to get the remote address: ", errno, '\n');
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "C: Creating the client socket...\n");
    client_socket_ = socket(remote_address->ai_family, remote_address->ai_socktype, remote_address->ai_protocol);
    if (!ISVALIDSOCKET(client_socket_)) {
        fprintf(stderr, "%s%d%c", "C: Failed to create the client socket: ", errno, '\n');
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "C: Connecting to the remote address...\n");
    if (connect(client_socket_, remote_address->ai_addr, remote_address->ai_addrlen)) {
        fprintf(stderr, "%s%d%c", "C: Failed to connect to the remote address: ", errno, '\n');
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(remote_address);
}

ChatClient::~ChatClient() {
    fprintf(stdout, "C: Closing the client socket...\n");
    close(client_socket_);
}

bool ChatClient::authorizeUser(const char* username, const char* password, int auth_type) {
    QString message;
    if (auth_type == LOGIN) {
        message += QString(LOGIN_CONNECTION) + "\n";
        fprintf(stdout, "C: Sending LOGIN request...\n");
    } else if (auth_type == RGSTR) {
        message += QString(RGSTR_CONNECTION) + "\n";
    } else {
        fprintf(stderr, "C: Failed to send authentification information: unknown type\n");
        fprintf(stdout, "C: Sending RGSTR request...\n");
    }

    message += "DATA: \n";
    message += "Username: "   + QString(username)                       + "\n";
    message += "Password: "   + QString(password) + "\n";

    if (send(client_socket_, message.toStdString().c_str(), message.size(), 0) < 0) {
        fprintf(stderr, "%s%d\n", "C: Sending login information failed: ", errno);
        return false;
    } else {
        memcpy(client_info_.username, username, sizeof(client_info_.username));
        bool is_success = getAuthRespond();

        if (is_success) {
            getNewMessages();
            // start accepting new messages;
        }

        return is_success;
    }
}

const char* ChatClient::getClientUsername() {
    if (strlen(client_info_.username) == 0) {
        return NULL;
    } else {
        return client_info_.username;
    }
}

void ChatClient::prepareReceiverID(const char* id) {
    memcpy(receiver_id_, id, ID_BUFFER_SIZE);
}

void ChatClient::sendMessage(const char* text) {
    QString message;
    message += QString(SEND_CONNECTION) + "\n";
    message += "DATE: " + QDateTime::currentDateTime().toString(DATABASE_DATE_FORMAT) + "\n";
    message += "SENDER_ID: " + QString(client_info_.id) + "\n";
    message += "RECEIVER_ID: " + QString(receiver_id_) + "\n";
    message += "DATA: \n";
    message += "Text: " + QString(text);

    fprintf(stdout, "C: Sending SEND request...");
    if (send(client_socket_, message.toStdString().c_str(), message.size(), 0) < 0) {
        fprintf(stderr, "C: Sending SEND request failed: %d\n", errno);
        return; //false;
    } else {
        return; //true
    }
}

bool ChatClient::findUser(const char* username) {
    memset(find_user_info_.id, 0, ID_BUFFER_SIZE);
    memset(find_user_info_.username, 0, USRNM_BUFFER_SIZE);

    QString message;
    message += QString(FIND_CONNECTION) + "\n";
    message += "DATA: \n";
    message += "Username: " + QString(username) + "\n";

    fprintf(stdout, "C: Sending FIND request...\n");
    if (send(client_socket_, message.toStdString().c_str(), message.size(), 0) < 0) {
        fprintf(stderr, "%s%d\n", "C: Sending FIND request failed: ", errno);
        return false;
    } else {
        return getFindRespond();
    }
}

void ChatClient::setupAddrInfoHints(addrinfo& hints) {
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
}

bool ChatClient::getAuthRespond() {
    if (recv(client_socket_, input_buffer_, sizeof(input_buffer_), 0) < 0) {
        fprintf(stderr, "C: Failed to recieve respond information\n");
        return false;
    } else {
        fprintf(stdout, "C: Received authentification respond from the server: ");
        if (strstr(input_buffer_, "CODE: OK\n")) {
            fprintf(stdout, "OK.\n");
            char *p_id = strstr(input_buffer_, "id: ") + sizeof("id: ")-1;
            size_t id_sz = strstr(p_id, "\n") - p_id;
            memcpy(client_info_.id, p_id, id_sz);

            return true;
        } else {
            fprintf(stdout, "FAILED.\n");
            memset(client_info_.username, 0, sizeof(client_info_.username));
            return false;
        }
    }
}

bool ChatClient::getFindRespond() {
    if (recv(client_socket_, input_buffer_, sizeof(input_buffer_), 0) < 0) {
        fprintf(stderr, "C: Failed to recieve respond information\n");
        return false;
    } else {
        fprintf(stdout, "C: Received find respond from the server: ");
        if (strstr(input_buffer_, "CODE: OK\n")) {
            fprintf(stdout, "OK.\n");
            parseFindRespond();
            return true;
        } else {
            fprintf(stdout, "NOT FOUND.\n");
            return false;
        }
    }
}

const typename ChatClient::ClientInfo* ChatClient::getFoundUser() {
    return &find_user_info_;
}

void ChatClient::parseFindRespond() {
    char *p = NULL;
    size_t offset = 0;

    p = strstr(input_buffer_, "id: ") + sizeof("id: ")-1;
    offset += strstr(p, "\n") - p;
    memcpy(find_user_info_.id, p, offset);
    find_user_info_.id[offset] = '\0';

    p = strstr(input_buffer_, "Username: ") + sizeof("Username: ")-1;
    offset += strstr(p, "\n") - p;
    memcpy(find_user_info_.username, p, offset);
    find_user_info_.username[offset] = '\0';
}

bool ChatClient::getAllMsgRespond() {
    while (recv(client_socket_, input_buffer_, sizeof(input_buffer_), 0) > 0) {
            char *p = input_buffer_;
            char sender_id[ID_BUFFER_SIZE];
            char date[DATE_BUFFER_SIZE];
            char text[MAX_MSG_SIZE]{};
            size_t sz = 0;

            while ((p = strstr(p, "Sender_id:"))) {
                p += sizeof("Sender_id:");
                sz = strstr(p, "\n")-p;
                memcpy(sender_id, p, sz);
                sender_id[sz] = 0;

                p = strstr(p, "Date:");
                p += sizeof("Date:");
                sz = strstr(p, "\n")-p;
                memcpy(date, p, sz);
                date[sz] = 0;

                p = strstr(p, "Text:");
                p += sizeof("Text:");
                sz = strstr(p, TEXT_END_IDENTIF)-p;
                memcpy(text, p, sz);
                text[sz] = 0;
            }

        if (strstr(input_buffer_, "CODE: ENDSND")) {
            break;
        }
    }

    return true;
}

void ChatClient::getNewMessages() {
    QString message;
    message += QString(GTNMS_CONNECTION) + "\n";
    message += "DATA: \n";
    message += QString("id: ") + client_info_.id + '\n';

    if (send(client_socket_, message.toStdString().c_str(), message.size(), 0) < 0) {
        fprintf(stderr, "C: Failed to send GTNMS request: %d\n", errno);
        return;
    } else {
        getAllMsgRespond();
        return;
    }
}

void ChatClient::getAllMessages() {
    QString message;
    message += QString(GTAMS_CONNECTION) + "\n";
    message += "DATA: \n";
    message += QString("id: ") + client_info_.id + '\n';

    if (send(client_socket_, message.toStdString().c_str(), message.size(), 0) < 0) {
        fprintf(stderr, "C: Failed to send GTNMS request: %d\n", errno);
        return;
    } else {
        getAllMsgRespond();
        return;
    }
}
