#ifndef H_DSCPP_ROLE
#define H_DSCPP_ROLE

#include "api.hh"

struct Role : Fetchable {
    std::string name;
    pmask_t perms;
    bool hoist, mentionable, managed;
    int pos, color_hex;

    ~Role();
    Role();
    ErrorCode fetch(snowflake id, long* err = NULL);
    ErrorCode parse(rapidjson::Document v, long* err = NULL);
    const char* getName();
    pmask_t getPerms();
    bool getHoist();
    bool getMentionable();
    bool getManaged();
    int getPos();
    int getColor();
};


class Overwrite : Fetchable {
    private:
    OverwiteType t;
    pmask_t allow;
    pmask_t deny;

    public:
    enum OverwriteType { ROLE, MEMBER };
    
    ~Overwrite();
    Overwrite();
    ErrorCode fetch(snowflake id, long* err = NULL);
    ErrorCode parse(rapidjson::Document v, long* err = NULL);
    pmask_t getAllowed();
    pmask_t getDenied();
}
#endif