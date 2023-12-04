#include "chatclient.h"

#include <QDateTime>
#include "constants.h"

ChatClient::ChatClient()
    : client_socket_(-1)
{
    fprintf(stdout, "Configuring remote connection...\n");
    addrinfo hints;
    setupAddrInfoHints(hints);

    addrinfo* remote_address;
    if (getaddrinfo(REMOTE_ADDRESS, REMOTE_SERVICE, &hints, &remote_address)) {
        fprintf(stderr, "%s%d%c", "Failed to get the remote address: ", errno, '\n');
        // return error;
    }

    fprintf(stdout, "Creating the client socket...\n");
    client_socket_ = socket(remote_address->ai_family, remote_address->ai_socktype, remote_address->ai_protocol);
    if (!ISVALIDSOCKET(client_socket_)) {
        fprintf(stderr, "%s%d%c", "Failed to create the client socket: ", errno, '\n');
        // return error;
    }

    fprintf(stdout, "Connecting to the remote address...\n");
    if (connect(client_socket_, remote_address->ai_addr, remote_address->ai_addrlen)) {
        fprintf(stderr, "%s%d%c", "Failed to connect to the remote address: ", errno, '\n');
        // return error;
    }

    freeaddrinfo(remote_address);
}

ChatClient::~ChatClient() {
    fprintf(stdout, "Closing the client socket...\n");
    close(client_socket_);
}

bool ChatClient::sendAuthInfo(const char* username, const char* password, int auth_type) {
    QString message;
    if (auth_type == LOGIN) {
        message += "TYPE: " + QString(LOGIN_CONNECTION) + "\n";
    } else if (auth_type == RGSTR) {
        message += "TYPE: " + QString(RGSTR_CONNECTION) + "\n";
    } else {
        fprintf(stderr, "Failed to send authentification information: unknown type\n");
    }

    message += "DATE: "       + QDateTime::currentDateTime().toString() + "\n";
    message += "DATA: \n";
    message += "Username: "   + QString(username)                       + "\n";
    message += "Password: "   + QString(password) + "\n";

    fprintf(stdout, "Sending login request...\n");
    if (send(client_socket_, message.toStdString().c_str(), message.size(), NULL) < 0) {
        fprintf(stderr, "%s%d\n", "Sending login information failed: ", errno);
        return false;
    }

    return true;
}

void ChatClient::setupAddrInfoHints(addrinfo& hints) {
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
}
