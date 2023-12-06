#include "chatserver.h"

ChatServer chat_server;

void closeServer();

int main() {
    chat_server.startAcceptConnection();
    atexit(closeServer);
}

void closeServer() {
    chat_server.closeConnection();
}
