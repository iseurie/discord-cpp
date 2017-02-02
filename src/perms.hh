#ifndef H_DSCPP_ROLE
#define H_DSCPP_ROLE

#include "api.hh"

struct Role : Fetchable {
    char* name;
    pmask_t perms;
    bool hoist, mentionable, managed;
    int pos, color_hex;

    ~Role();
    Role();
    ErrorCode fetch(snowflake id, long* err);
    ErrorCode parse(rapidjson::Document v, long* err);
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
    ErrorCode fetch(snowflake id, long* err);
    ErrorCode parse(rapidjson::Document v, long* err);
    pmask_t getAllowed();
    pmask_t getDenied();
}
#endif