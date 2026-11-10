# crt0.s (c:/KHCOM/MainApp/crt0.s): the program's entry point, Sony's EE crt0 as the game shipped it.
# Hand-written from splat's disassembly (same instructions, under noreorder) with the names Sony's linker script and
# crt0 use instead of address-named labels: the .bss clear runs from _fbss to _end, the main thread gets _gp,
# _stack, _stack_size and _args, the heap _end and _heap_size (docs/port-step1-shiftable.md: splat had expressed
# _gp as a .bss variable + 0x2130 and _fbss as the first .sbss variable, which only hold at the original addresses).
# _fbss, _stack and _heap_size come from config/<VERSION>/linker_script_extra.ld.

    .set noreorder
    .set noat

    .text
    .align 3
    # a 4-byte function before _start; the original has no symbol for it, splat calls it func_00100000
    .globl func_00100000
    .type func_00100000, @function
func_00100000:
    nop
    .size func_00100000, 4
    nop

    .align 3
    .globl _start
    .type _start, @function
    .ent _start
_start:
    padduw  $at, $zero, $zero
    padduw  $v0, $zero, $zero
    padduw  $v1, $zero, $zero
    padduw  $a0, $zero, $zero
    padduw  $a1, $zero, $zero
    padduw  $a2, $zero, $zero
    padduw  $a3, $zero, $zero
    padduw  $t0, $zero, $zero
    padduw  $t1, $zero, $zero
    padduw  $t2, $zero, $zero
    padduw  $t3, $zero, $zero
    padduw  $t4, $zero, $zero
    padduw  $t5, $zero, $zero
    padduw  $t6, $zero, $zero
    padduw  $t7, $zero, $zero
    padduw  $s0, $zero, $zero
    padduw  $s1, $zero, $zero
    padduw  $s2, $zero, $zero
    padduw  $s3, $zero, $zero
    padduw  $s4, $zero, $zero
    padduw  $s5, $zero, $zero
    padduw  $s6, $zero, $zero
    padduw  $s7, $zero, $zero
    padduw  $t8, $zero, $zero
    padduw  $t9, $zero, $zero
    padduw  $gp, $zero, $zero
    padduw  $sp, $zero, $zero
    padduw  $fp, $zero, $zero
    padduw  $ra, $zero, $zero
    mthi    $zero
    mthi1   $zero
    mtlo    $zero
    mtlo1   $zero
    mtsah   $zero, 0x0
    mtc1    $zero, $f0
    mtc1    $zero, $f1
    mtc1    $zero, $f2
    mtc1    $zero, $f3
    mtc1    $zero, $f4
    mtc1    $zero, $f5
    mtc1    $zero, $f6
    mtc1    $zero, $f7
    mtc1    $zero, $f8
    mtc1    $zero, $f9
    mtc1    $zero, $f10
    mtc1    $zero, $f11
    mtc1    $zero, $f12
    mtc1    $zero, $f13
    mtc1    $zero, $f14
    mtc1    $zero, $f15
    mtc1    $zero, $f16
    mtc1    $zero, $f17
    mtc1    $zero, $f18
    mtc1    $zero, $f19
    mtc1    $zero, $f20
    mtc1    $zero, $f21
    mtc1    $zero, $f22
    mtc1    $zero, $f23
    mtc1    $zero, $f24
    mtc1    $zero, $f25
    mtc1    $zero, $f26
    mtc1    $zero, $f27
    mtc1    $zero, $f28
    mtc1    $zero, $f29
    mtc1    $zero, $f30
    mtc1    $zero, $f31
    adda.s  $f0, $f1
    sync.p
    ctc1    $zero, $31

    # clear .sbss and .bss: bytes up to a 16-byte boundary, then quadwords, then the rest
    lui     $v0, %hi(_fbss)
    lui     $v1, %hi(_end)
    addiu   $v0, $v0, %lo(_fbss)
    addiu   $v1, $v1, %lo(_end)
    .align 2
1:  andi    $a0, $v0, 0xF
    beqz    $a0, 2f
    nop
    sb      $zero, 0x0($v0)
    addiu   $v0, $v0, 0x1
    nop
    j       1b
    nop
    .align 2
2:  lui     $a0, 0xFFFF
    ori     $a0, $a0, 0xFFF0
    and     $a0, $v1, $a0
    .align 2
3:  beq     $v0, $a0, 4f
    nop
    sq      $zero, 0x0($v0)
    addiu   $v0, $v0, 0x10
    nop
    nop
    j       3b
    nop
    .align 2
4:  beq     $v0, $v1, 5f
    nop
    sb      $zero, 0x0($v0)
    addiu   $v0, $v0, 0x1
    nop
    nop
    j       4b
    nop

    # main thread: SetupThread(_gp, _stack, _stack_size, _args, _root)
    .align 2
5:  lui     $a0, %hi(_gp)
    lui     $a1, %hi(_stack)
    lui     $a2, %hi(_stack_size)
    lui     $a3, %hi(_args)
    lui     $t0, %hi(_root)
    addiu   $a0, $a0, %lo(_gp)
    addiu   $a1, $a1, %lo(_stack)
    addiu   $a2, $a2, %lo(_stack_size)
    addiu   $a3, $a3, %lo(_args)
    addiu   $t0, $t0, %lo(_root)
    daddu   $gp, $a0, $zero
    addiu   $v1, $zero, 0x3C
    syscall 0
    daddu   $sp, $v0, $zero

    # heap: SetupHeap(_end, _heap_size)
    lui     $a0, %hi(_end)
    lui     $a1, %hi(_heap_size)
    addiu   $a0, $a0, %lo(_end)
    addiu   $a1, $a1, %lo(_heap_size)
    addiu   $v1, $zero, 0x3D
    syscall 0

    jal     _InitSys
    nop
    jal     FlushCache
    daddu   $a0, $zero, $zero
    lui     $s0, %hi(_init)
    addiu   $s0, $s0, %lo(_init)
    beqz    $s0, 6f
    nop
    jalr    $s0
    nop
    .align 2
6:  lui     $a0, %hi(_fini)
    addiu   $a0, $a0, %lo(_fini)
    beqz    $a0, 7f
    nop
    jal     atexit
    nop
    .align 2
7:  ei
    lui     $v0, %hi(_args)
    addiu   $v0, $v0, %lo(_args)
    lw      $a0, 0x0($v0)
    jal     main
    addiu   $a1, $v0, 0x4
    j       exit
    daddu   $a0, $v0, $zero
    .end _start
    .size _start, . - _start

    .align 3
    .globl _exit
    .type _exit, @function
    .ent _exit
_exit:
    j       Exit
    nop
    .end _exit
    .size _exit, . - _exit

    .align 3
    .globl _root
    .type _root, @function
    .ent _root
_root:
    addiu   $v1, $zero, 0x23        # ExitThread
    syscall 0
    .end _root
    .size _root, . - _root
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    # the main thread's argument block (argc, argv[16], strings), filled by the kernel
    .bss
    .align 7
    .globl _args
_args:
    .space 0x180

# SOURCE DATA: .bss
