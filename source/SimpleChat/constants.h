#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <string>

#define ISVALIDSOCKET(s) ((s) >= 0)

const int RECV_SERVER_BUFFER_SIZE = 4096;
const int SERVER_RESPOND_BUFFER_SZ = 65835*2;
const int CLIENT_RECEIVE_BUFFER_SZ = 65835*2;
const int MAX_DATABASE_SMALLINT_V  = 65535;
const int MAX_MSG_SIZE = MAX_DATABASE_SMALLINT_V / 10;
const int ID_BUFFER_SIZE    = 9;
const int USRNM_BUFFER_SIZE = 21;
const int DATE_BUFFER_SIZE  = 20;
const int INVALID_SOCKET    = -1;

const int FIXED_MSG_WIDTH = 500;
const int CHARS_IN_LINE = 52;

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

extern const char* GTAMS_FRID_CONNECTION;
extern const char* GTASND_TOID_CONNECTION;

extern const char* OK_SRVR_RESPOND;
extern const char* ERR_SRVR_RESPOND;
extern const char* END_SRVR_RESPOND;
extern const char* NFND_SRVR_RESPOND;
extern const char* FND_SRVR_RESPOND;
extern const char* MSG_SRVR_RESPOND;
extern const char* SND_INFO_SRVR_RESPOND;
extern const char* END_MSG_SRVR_RESPOND;
extern const char* END_SND_INFO_SRVR_RESPOND;
extern const char* AUTH_ERR_SRVR_RESPOND;
extern const char* AUTH_SCC_SRVR_RESPOND;

enum AuthentificationType {
    LOGIN,
    RGSTR,
};

enum RespondCode {
    OK_RSPND,
    ENDSND_RSPND,
    AUTH_ERROR_RSPND,
    AUTH_SUCCS_RSPND,
    RQST_ERROR_RSPND,
    FND_RSPND,
    NTFD_ERROR_RSPND,
    MSG_RSPND,
    END_MSG_RSPND,
    SND_INFO_RSPND,
    END_SND_INFO_RSPND,
    SRVR_ERROR_RSPND,
};

struct UserInfo {
    char id[ID_BUFFER_SIZE]; // MEDIUM INT has 8 numbers
    char username[USRNM_BUFFER_SIZE];
};

struct FetchedMessage {
    std::string from_id;
    std::string to_id;
    std::string date;
    std::string text;
};

#endif // CONSTANTS_H
