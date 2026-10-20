/* DIntr / EIntr: disable / enable interrupts (Status.EIE), returning whether they were enabled. */

int DIntr(void)
{
    int eie, res, t;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    res = eie != 0;
    if (eie) {
        /* di until Status.EIE reads back clear (an interrupt can be taken right after di) */
        asm volatile (
            ".set noreorder\n"
            "\t.p2align 3\n"
            "1:\tdi\n"
            "\tsync.p\n"
            "\tmfc0\t%0, $12\n"
            "\tlui\t%1, 0x1\n"
            "\tand\t%0, %0, %1\n"
            "\tnop\n"
            "\tnop\n"
            "\tbnez\t%0, 1b\n"
            "\tnop\n"
            "\t.set reorder"
            : "=r" (t), "=r" (eie));
    } else {
        res = 0;
    }
    return res;
}

int EIntr(void)
{
    int eie;

    asm volatile ("mfc0\t%0, $12" : "=r" (eie));
    eie &= 0x10000;
    asm volatile ("ei");
    return eie != 0;
}
