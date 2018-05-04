
#include <cstdio>
#include <cstdlib>


int
exit_with_errstr(const char* errstr, int errnumber)
{
    printf("%s\n", errstr);
    exit(errnumber);

    return errnumber;
}
