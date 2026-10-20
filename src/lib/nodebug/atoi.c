/* newlib libc/stdlib/atoi.c */
extern long strtol(const char *, char **, int);

int atoi(const char *s)
{
    return (int)strtol(s, (char **)0, 10);
}
