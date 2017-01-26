#ifndef H_DSCPP_EMOJI
#define H_DSCPP_EMOJI

#include "api.hh"

namespace dsc {

class Emoji : Fetchable {
    private:
    int activeRoleC;
    snowflake* activeRoleV;
    bool requireColons, managed;
    char* name;
    
    public:
    Emoji();
    ~Emoji();
    Error parse(rapidjson::Document v);
    Error fetch(snowflake id);

    int getActiveRoleC();
    const Role* getActiveRoleV();
    bool requiresColons();
    bool isManaged();
};

}
#endif