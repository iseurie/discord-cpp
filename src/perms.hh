#ifndef H_DSCPP_ROLE
#define H_DSCPP_ROLE

#include "api.hh"

class Role : Fetchable {
    private:
    char* name;
    pmask_t perms;
    bool hoist, mentionable, managed;
    int pos, color_hex;

    public:
    ~Role();
    Role();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    const char* getName();
    pmask_t getPerms();
    bool getHoist();
    bool getMentionable();
    bool getManaged();
    int getPos();
    int getColor();
};

enum OverwriteType { ROLE, MEMBER };

class Overwrite : Fetchable {
    private:
    OverwiteType t;
    pmask_t allow;
    pmask_t deny;

    public:
    ~Overwrite();
    Overwrite();
    Error fetch(snowflake id);
    Error parse(rapidjson::Document v);
    pmask_t getAllowed();
    pmask_t getDenied();
}
#endif