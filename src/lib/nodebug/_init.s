# Empty weak _init/_fini (the constructors and destructors run through __CTOR_LIST__/__DTOR_LIST__ instead).

    .set noreorder
    .text

    .align 3
    .weak _init
    .type _init, @function
    .ent _init
_init:
    jr      $ra
    nop
    .end _init
    .size _init, . - _init

    .align 3
    .weak _fini
    .type _fini, @function
    .ent _fini
_fini:
    jr      $ra
    nop
    .end _fini
    .size _fini, . - _fini
