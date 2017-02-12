#ifndef H_DSCPP_ROLE
#define H_DSCPP_ROLE

#include "api.hh"

// Editable Role object
/* The Guild Role stores a permissions bitmask,
 * in addition to a name and various data
 * for its display in the client roster. */

struct Role : Pushable {
    std::string name;
    pmask_t perms;
    bool hoist, mentionable, managed;
    int pos, color_hex;

    WAPIError fetch(snowflake id);
    WAPIError parse(const rapidjson::Document* v);
    ~Role();
    Role();
};

#endif