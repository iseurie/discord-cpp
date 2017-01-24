#ifndef H_DSCPP_EMOJI
#define H_DSCPP_EMOJI

#include "api.hh"

namespace dsc {

class Emoji : Fetchable {
    private:
    std::vector<snowflake> activeRoles;
    bool requireColons, managed;
    char* name;
    
    public:
    Emoji();
    ~Emoji();
    Error parse(rapidjson::Document v);
    Error fetch(snowflake id);

    std::vector<snowflake>* getActiveRoles();
    bool requiresColons();
    bool isManaged();
    
};

}
#endif