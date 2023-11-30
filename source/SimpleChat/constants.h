#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>

#define ISVALIDSOCKET(s) ((s) >= 0)

extern const char* SERVER_ADDRESS;
extern const char* SERVER_SERVICE;

extern const char* REMOTE_ADDRESS;
extern const char* REMOTE_SERVICE;

extern const char* LOGIN_CONNECTION;
extern const char* RGSTR_CONNECTION;
extern const char* GET_CONNECTION;
extern const char* POST_CONNECTION;

extern const char* KEEP_ALIVE_CONNECTION;
extern const char* CLOSE_CONNECTION;

#endif // CONSTANTS_H