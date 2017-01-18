#ifndef H_DSCPP_CLIENT
#define H_DSCPP_CLIENT

#include <string>
#include <vector>
#include <stdint.h>
#include "user.hh"

namespace dsc {

enum ClientType { NORMAL, BOT };

class Client {
    private:
    std::string sessionToken;
    public:
    Client();
    Error Auth(const char* user, const char* pass);
    Error Auth(const char* token);
    User* CurrentUser;
}

}
#endif
