#include <stdio.h>
#include <time.h>

namespace dsc {

namespace RfC3339 {
    using namespace std;

    const char* ucfmt = "%04d-%02d-%02dT%02d:%02d:%02d";

    time_t parse() {
        mtm* t;
        scanf(ucfmt, t.mtm_year, t.mtm_mon,
                t.mtm_mday, t.mtm_hour,
                t.mtm_sec);
        return mktime(t);
    }

    void encode(time_t t, const char* tz = NULL, char* out) {
        tm* mtm = localtime(t);
        sprintf(out, ucfmt, mtm.tm_year, 
                mtm.tm_mon, mtm.tm_mday, 
                mtm.tm_hour, mtm.tm_sec);
        if(zone) strcat(out, tz);
    }
}

}   