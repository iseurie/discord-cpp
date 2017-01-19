#ifndef H_DSCPP_FETCHABLE
#define H_DSCPP_FETCHABLE

#include "api.hh"

namespace dsc {

class TextMessage : Fetchable {
    private:
        char* content;
        User* author;
        
    public:
        TextMessage();
        ~TextChannel();
        Error fetch(snowflake id);
        Error fetch(rapidjson::Document v);
        const char* getContent();
        
};

}
#endif