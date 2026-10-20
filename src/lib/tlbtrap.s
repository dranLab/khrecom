# EE TLB-exception and debug-exception trampolines (Sony's tlbtrap.s): save the CPU state to EEREG, return from
# the exception into a handler call (_kTLBRefillHandler, _kDebugHandler[cause]) on the hstack, and restore it.

    .set noreorder
    .set noat

    # sq/lq every GPR but zero, k0 and k1 at EEREG + 16 * its number
    .macro save_gprs
    .irp r, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,28,29,30,31
    sq      $\r, \r * 16($k0)
    .endr
    .endm
    .macro restore_gprs
    .irp r, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,28,29,30,31
    lq      $\r, \r * 16($k0)
    .endr
    .endm

    # the multiply/shift state: hi, hi1, lo, lo1, sa
    .macro save_hilo
    mfhi    $v0
    lui     $at, %hi(EERH0)
    sd      $v0, %lo(EERH0)($at)
    mfhi1   $v0
    lui     $at, %hi(EERH1)
    sd      $v0, %lo(EERH1)($at)
    mflo    $v0
    lui     $at, %hi(EERL0)
    sd      $v0, %lo(EERL0)($at)
    mflo1   $v0
    lui     $at, %hi(EERL1)
    sd      $v0, %lo(EERL1)($at)
    mfsa    $v0
    lui     $at, %hi(EERSA)
    sd      $v0, %lo(EERSA)($at)
    .endm

    .text

    .align 7
    .globl _kTLBException
    .type _kTLBException, @function
    .ent _kTLBException
_kTLBException:
    lui     $k0, %hi(EEREG)
    addiu   $k0, $k0, %lo(EEREG)
    save_gprs
    save_hilo
    mfc0    $a0, $12            # Status
    mfc0    $a1, $13            # Cause
    mfc0    $a2, $14            # EPC
    mfc0    $a3, $8             # BadVAddr
    lui     $t0, %hi(EEREG)
    addiu   $t0, $t0, %lo(EEREG)
    lui     $at, %hi(EEEPC)
    sw      $a2, %lo(EEEPC)($at)
    lui     $at, %hi(_xlaunch)
    addiu   $at, $at, %lo(_xlaunch)
    mtc0    $at, $14            # return into _xlaunch
    sync.p
    mfc0    $at, $12
    addiu   $v0, $zero, -2      # clear Status.IE
    and     $at, $at, $v0
    mtc0    $at, $12
    sync.p
    eret
    .end _kTLBException
    .size _kTLBException, . - _kTLBException

    .align 3
    .globl _xlaunch
    .type _xlaunch, @function
    .ent _xlaunch
_xlaunch:
    lui     $at, %hi(_kTLBRefillHandler)
    lw      $at, %lo(_kTLBRefillHandler)($at)
    lui     $sp, %hi(hstack)
    jalr    $at
    addiu   $sp, $sp, %lo(hstack)
    addiu   $v1, $zero, -0x54   # syscall: _ExitTLBHandler
    syscall
    .end _xlaunch
    .size _xlaunch, . - _xlaunch

    .align 6
    .globl _kExitTLBHandler
    .type _kExitTLBHandler, @function
    .ent _kExitTLBHandler
_kExitTLBHandler:
    mfc0    $at, $12
    addiu   $k0, $zero, -0x1C   # clear Status.EXL, ERL, KSU
    and     $at, $at, $k0
    mtc0    $at, $12
    sync.p
    lui     $v0, %hi(EEEPC)
    lw      $v0, %lo(EEEPC)($v0)
    mtc0    $v0, $14
    sync.p
    lui     $v0, %hi(EERH0)
    ld      $v0, %lo(EERH0)($v0)
    mthi    $v0
    lui     $v0, %hi(EERH1)
    ld      $v0, %lo(EERH1)($v0)
    mthi1   $v0
    lui     $v0, %hi(EERL0)
    ld      $v0, %lo(EERL0)($v0)
    mtlo    $v0
    lui     $v0, %hi(EERL1)
    ld      $v0, %lo(EERL1)($v0)
    mtlo1   $v0
    lui     $v0, %hi(EERSA)
    ld      $v0, %lo(EERSA)($v0)
    mtsa    $v0
    sync.p
    lui     $k0, %hi(EEREG)
    addiu   $k0, $k0, %lo(EEREG)
    restore_gprs
    mfc0    $k0, $12
    ori     $k0, $k0, 0x13      # set Status.KSU = user, EXL, IE: eret drops back to user mode
    mtc0    $k0, $12
    sync.p
    eret
    .end _kExitTLBHandler
    .size _kExitTLBHandler, . - _kExitTLBHandler

    .align 6
    .globl _kDebugException
    .type _kDebugException, @function
    .ent _kDebugException
_kDebugException:
    lui     $k0, %hi(EEREG)
    addiu   $k0, $k0, %lo(EEREG)
    save_gprs
    save_hilo
    mfc0    $a0, $12            # Status
    mfc0    $a1, $13            # Cause
    mfc0    $a2, $14            # EPC
    mfc0    $a3, $8             # BadVAddr
    mfc0    $t0, $23            # BPC
    lui     $t1, %hi(EEREG)
    addiu   $t1, $t1, %lo(EEREG)
    lui     $at, %hi(.Ldebug_launch)
    addiu   $at, $at, %lo(.Ldebug_launch)
    mtc0    $at, $14            # return into .Ldebug_launch
    sync.p
    mfc0    $at, $12
    addiu   $v0, $zero, -2      # clear Status.IE
    and     $at, $at, $v0
    mtc0    $at, $12
    sync.p
    eret
.Ldebug_launch:
    andi    $v0, $a1, 0x7C      # Cause.ExcCode * 4
    lui     $at, %hi(_kDebugHandler)
    addu    $at, $at, $v0
    lw      $at, %lo(_kDebugHandler)($at)
    lui     $sp, %hi(hstack)
    jalr    $at
    addiu   $sp, $sp, %lo(hstack)
    break   1023, 1023
    .end _kDebugException
    .size _kDebugException, . - _kDebugException

    .bss
    .align 4
hstack:                         # the handlers' stack grows down from here
EEREG:
    .space  0x200
EERH0:
    .space  8
EERH1:
    .space  8
EERL0:
    .space  8
EERL1:
    .space  8
EERSA:
    .space  8
EEEPC:
    .space  8
# SOURCE DATA: .bss
