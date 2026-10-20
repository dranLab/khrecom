/* newlib libm/common/s_matherr.c */
struct exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    int err;
};

int matherr(struct exception *x)
{
    int n = 0;
    if (x->arg1 != x->arg1)
        return 0;
    return n;
}
