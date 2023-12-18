#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <string>

#define ISVALIDSOCKET(s) ((s) >= 0)

const int RECV_SERVER_BUFFER_SIZE = 4096;
const int SERVER_RESPOND_BUFFER_SZ = 4096;
const int CLIENT_RECEIVE_BUFFER_SZ = 300;
const int MAX_DATABASE_SMALLINT_V  = 65535;
const int MAX_MSG_SIZE = MAX_DATABASE_SMALLINT_V / 10;
const int ID_BUFFER_SIZE    = 9;
const int USRNM_BUFFER_SIZE = 21;
const int DATE_BUFFER_SIZE  = 20;
const int INVALID_SOCKET    = -1;

extern const char* TEXT_END_IDENTIF;

extern const char* SERVER_ADDRESS;
extern const char* SERVER_SERVICE;

extern const char* DATABASE_DATE_FORMAT;

extern const char* REMOTE_ADDRESS;
extern const char* REMOTE_SERVICE;

extern const char* LOGIN_CONNECTION;
extern const char* RGSTR_CONNECTION;
extern const char* FIND_CONNECTION;
extern const char* SEND_CONNECTION;
extern const char* GTNMS_CONNECTION;
extern const char* GTAMS_CONNECTION;
extern const char* GTAMS_FRID_CONNECTION;

enum AuthentificationType {
    LOGIN,
    RGSTR,
};

enum RespondCode {
    OK_RSPND,
    ENDSND_RSPND,
    AUTH_ERROR_RSPND,
    RQST_ERROR_RSPND,
    NTFD_ERROR_RSPND,
    SRVR_ERROR_RSPND,
};

struct UserInfo {
    char id[ID_BUFFER_SIZE]; // MEDIUM INT has 8 numbers
    char username[USRNM_BUFFER_SIZE];
};

struct FetchedMessage {
    std::string from_id;
    std::string date;
    std::string text;
};

#endif // CONSTANTS_H
