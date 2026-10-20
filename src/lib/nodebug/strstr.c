/* newlib libc/string/strstr.c */
#define NULL 0

char *strstr(const char *searchee, const char *lookfor)
{
    if (*searchee == 0) {
        if (*lookfor)
            return (char *)NULL;
        return (char *)searchee;
    }

    while (*searchee) {
        int i;
        i = 0;

        while (1) {
            if (lookfor[i] == 0) {
                return (char *)searchee;
            }

            if (lookfor[i] != searchee[i]) {
                break;
            }
            i++;
        }
        searchee++;
    }

    return (char *)NULL;
}
