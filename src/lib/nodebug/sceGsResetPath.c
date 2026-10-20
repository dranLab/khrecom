/* libgraph: reset the GIF/VIF1/VU1 path (and sceGsSyncPath) */
typedef unsigned int u_int;
typedef int u_long128 __attribute__((mode(TI)));

/* the function-local static packets are in the nodebug data block (not this object's) */
extern u_long128 q1 __asm__("q1.12");
extern u_long128 q2 __asm__("q2.13");

#define VIF1_FBRST ((volatile u_int *)0x10003c10)
#define VIF1_ERR ((volatile u_int *)0x10003c20)
#define VIF1_FIFO ((volatile u_long128 *)0x10005000)
#define GIF_CTRL ((volatile u_int *)0x10003000)
#define GIF_STAT ((volatile u_int *)0x10003020)
#define VIF1_STAT ((volatile u_int *)0x10003c00)
#define D1_CHCR ((volatile u_int *)0x10009000)
#define D2_CHCR ((volatile u_int *)0x1000a000)

void sceGsResetPath(void)
{
    u_int r;

    *VIF1_FBRST = 1;
    *VIF1_ERR = 2;
    __asm__ volatile(
        ".set noreorder\n"
        "\tdaddu %0, $0, $0\n"
        "\tsync\n"
        "\tcfc2.ni %0, $vi28\n"
        "\tnop\n"
        "\tori %0, %0, 0x200\n"
        "\tctc2.ni %0, $vi28\n"
        "\tsync.p\n"
        "1:\n"
        "\tcfc2.ni %0, $vi29\n"
        "\tnop\n"
        "\tnop\n"
        "\tnop\n"
        "\tandi %0, %0, 0x100\n"
        "\tbnez %0, 1b\n"
        "\tnop\n"
        "\taddiu %0, $0, 0x404\n"
        "\tctc2.ni %0, $vi28\n"
        "\tnop\n"
        ".set reorder"
        : "=r"(r));
    *VIF1_FIFO = q1;
    *VIF1_FIFO = q2;
    *GIF_CTRL = 1;
}

int sceGsSyncPath(int mode, unsigned short timeout)
{
    u_int ret, vpu;

    while (1) {
        ret = 0;
        if (*D1_CHCR & 0x0100)
            ret = 1;
        if (*D2_CHCR & 0x0100)
            ret |= 2;
        if (*VIF1_STAT & 3)
            ret |= 4;
        __asm__ volatile("cfc2.ni %0, $vi29" : "=r"(vpu));
        if (vpu & 0x100)
            ret |= 8;
        if (*GIF_STAT & 0xc00)
            ret |= 0x10;
        if (ret == 0)
            break;
    }
    return 0;
}
