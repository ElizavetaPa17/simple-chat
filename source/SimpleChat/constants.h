#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>

#define ISVALIDSOCKET(s) ((s) >= 0)

const int RECV_SERVER_BUFFER_SIZE = 4096;
const int SERVER_RESPOND_BUFFER_SZ = 300;
const int CLIENT_RECEIVE_BUFFER_SZ = 300;
const int INVALID_SOCKET = -1;

extern const char* SERVER_ADDRESS;
extern const char* SERVER_SERVICE;

extern const char* REMOTE_ADDRESS;
extern const char* REMOTE_SERVICE;

extern const char* LOGIN_CONNECTION;
extern const char* RGSTR_CONNECTION;
extern const char* FIND_CONNECTION;
extern const char* GET_CONNECTION;
extern const char* POST_CONNECTION;

enum AuthentificationType {
    LOGIN,
    RGSTR
};

enum RespondCode {
    OK_RSPND,
    AUTH_ERROR_RSPND,
    RQST_ERROR_RSPND,
    NTFD_ERROR_RSPND,
    SRVR_ERROR_RSPND,
};

struct UserInfo {
    char id [9]; // MEDIUM INT has 8 numbers
    char username[21];
};

#endif // CONSTANTS_H
