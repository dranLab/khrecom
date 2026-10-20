# ATRAC3 decoder kernels in EE MMI assembly (at3core.s, part of libat3dec): the inverse QMF filter bank
# (h_iqmf_fb1/2), IMDCT butterflies and the final output copy.

    .set noreorder
    .set noat

    .text

    .globl h_iqmf_fb2
    .type h_iqmf_fb2, @function
    .ent h_iqmf_fb2
h_iqmf_fb2:
    lui     $t9, %hi(i_qmfCh)
    addiu   $t9, $t9, %lo(i_qmfCh)
    lq      $t0, 0x0($t9)
    lq      $t1, 0x10($t9)
    lq      $t2, 0x20($t9)
    lq      $t3, 0x30($t9)
    lq      $t4, 0x40($t9)
    lq      $t5, 0x50($t9)
    lq      $t6, 0x0($a0)
    lq      $t7, 0x10($a0)
    lq      $t8, 0x20($a0)
    lq      $t9, 0x30($a0)
    lq      $a1, 0x40($a0)
    lq      $a2, 0x50($a0)
    pmthi   $zero
    pmtlo   $zero
    pmaddh  $a3, $t6, $t0
    pmaddh  $a3, $t7, $t1
    pmaddh  $a3, $t8, $t2
    pmaddh  $a3, $t9, $t3
    pmaddh  $a3, $a1, $t4
    pmaddh  $a3, $a2, $t5
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a3, $a2, $a3
    pcpyud  $a2, $a3, $a3
    paddsw  $a3, $a2, $a3
    psraw   $a3, $a3, 13
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x0($a0)
    lq      $a2, 0x50($a0)
    pmthi   $zero
    pmtlo   $zero
    mtsah   $zero, 0x2
    qfsrv   $a3, $t7, $t6
    pmaddh  $a3, $a3, $t0
    qfsrv   $a3, $t8, $t7
    pmaddh  $a3, $a3, $t1
    qfsrv   $a3, $t9, $t8
    pmaddh  $a3, $a3, $t2
    qfsrv   $a3, $a1, $t9
    pmaddh  $a3, $a3, $t3
    qfsrv   $a3, $a2, $a1
    pmaddh  $a3, $a3, $t4
    lq      $a3, 0x60($a0)
    qfsrv   $a3, $a3, $a2
    pmaddh  $a3, $a3, $t5
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a3, $a2, $a3
    pcpyud  $a2, $a3, $a3
    paddsw  $a3, $a2, $a3
    psraw   $a3, $a3, 13
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x4($a0)
    lq      $a2, 0x50($a0)
    pmthi   $zero
    pmtlo   $zero
    mtsah   $zero, 0x4
    qfsrv   $a3, $t7, $t6
    pmaddh  $a3, $a3, $t0
    qfsrv   $a3, $t8, $t7
    pmaddh  $a3, $a3, $t1
    qfsrv   $a3, $t9, $t8
    pmaddh  $a3, $a3, $t2
    qfsrv   $a3, $a1, $t9
    pmaddh  $a3, $a3, $t3
    qfsrv   $a3, $a2, $a1
    pmaddh  $a3, $a3, $t4
    lq      $a3, 0x60($a0)
    qfsrv   $a3, $a3, $a2
    pmaddh  $a3, $a3, $t5
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a3, $a2, $a3
    pcpyud  $a2, $a3, $a3
    paddsw  $a3, $a2, $a3
    psraw   $a3, $a3, 13
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x8($a0)
    lq      $a2, 0x50($a0)
    pmthi   $zero
    pmtlo   $zero
    mtsah   $zero, 0x6
    qfsrv   $a3, $t7, $t6
    pmaddh  $a3, $a3, $t0
    qfsrv   $a3, $t8, $t7
    pmaddh  $a3, $a3, $t1
    qfsrv   $a3, $t9, $t8
    pmaddh  $a3, $a3, $t2
    qfsrv   $a3, $a1, $t9
    pmaddh  $a3, $a3, $t3
    qfsrv   $a3, $a2, $a1
    pmaddh  $a3, $a3, $t4
    lq      $a3, 0x60($a0)
    qfsrv   $a3, $a3, $a2
    pmaddh  $a3, $a3, $t5
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a3, $a2, $a3
    pcpyud  $a2, $a3, $a3
    paddsw  $a3, $a2, $a3
    psraw   $a3, $a3, 13
    pmtlo   $a3
    pmfhl.sh $a3
    jr      $ra
     sw      $a3, 0xC($a0)
    nop
    .end h_iqmf_fb2
    .size h_iqmf_fb2, . - h_iqmf_fb2

    .globl h_iqmf_fb1
    .type h_iqmf_fb1, @function
    .ent h_iqmf_fb1
h_iqmf_fb1:
    lui     $t9, %hi(i_qmfAh)
    addiu   $t9, $t9, %lo(i_qmfAh)
    beqz    $a2, mproc
     nop
    nop
    lui     $t9, %hi(i_qmfBh)
    addiu   $t9, $t9, %lo(i_qmfBh)
mproc:
    lq      $t0, 0x0($t9)
    lq      $t1, 0x10($t9)
    lq      $t2, 0x20($t9)
    lq      $t3, 0x0($a1)
    lq      $t4, 0x10($a1)
    lq      $t5, 0x20($a1)
    lq      $t6, 0x230($a1)
    lq      $t7, 0x240($a1)
    lq      $t8, 0x250($a1)
    pmthi   $zero
    pmtlo   $zero
    pmaddh  $t9, $t0, $t3
    pmaddh  $t9, $t1, $t4
    pmaddh  $t9, $t2, $t5
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    pmthi   $zero
    pmtlo   $zero
    pmaddh  $a2, $t0, $t6
    pmaddh  $a2, $t1, $t7
    pmaddh  $a2, $t2, $t8
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x0($a0)
    mtsah   $zero, 0x1
    lq      $a2, 0x30($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $t9, $t4, $t3
    pmaddh  $t9, $t0, $t9
    qfsrv   $t9, $t5, $t4
    pmaddh  $t9, $t1, $t9
    qfsrv   $t9, $a2, $t5
    pmaddh  $t9, $t2, $t9
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    lq      $a3, 0x260($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $a2, $t7, $t6
    pmaddh  $a2, $t0, $a2
    qfsrv   $a2, $t8, $t7
    pmaddh  $a2, $t1, $a2
    qfsrv   $a2, $a3, $t8
    pmaddh  $a2, $t2, $a2
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x8($a0)
    mtsah   $zero, 0x2
    lq      $a2, 0x30($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $t9, $t4, $t3
    pmaddh  $t9, $t0, $t9
    qfsrv   $t9, $t5, $t4
    pmaddh  $t9, $t1, $t9
    qfsrv   $t9, $a2, $t5
    pmaddh  $t9, $t2, $t9
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    lq      $a3, 0x260($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $a2, $t7, $t6
    pmaddh  $a2, $t0, $a2
    qfsrv   $a2, $t8, $t7
    pmaddh  $a2, $t1, $a2
    qfsrv   $a2, $a3, $t8
    pmaddh  $a2, $t2, $a2
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x10($a0)
    mtsah   $zero, 0x3
    lq      $a2, 0x30($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $t9, $t4, $t3
    pmaddh  $t9, $t0, $t9
    qfsrv   $t9, $t5, $t4
    pmaddh  $t9, $t1, $t9
    qfsrv   $t9, $a2, $t5
    pmaddh  $t9, $t2, $t9
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    lq      $a3, 0x260($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $a2, $t7, $t6
    pmaddh  $a2, $t0, $a2
    qfsrv   $a2, $t8, $t7
    pmaddh  $a2, $t1, $a2
    qfsrv   $a2, $a3, $t8
    pmaddh  $a2, $t2, $a2
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x18($a0)
    mtsah   $zero, 0x4
    lq      $a2, 0x30($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $t9, $t4, $t3
    pmaddh  $t9, $t0, $t9
    qfsrv   $t9, $t5, $t4
    pmaddh  $t9, $t1, $t9
    qfsrv   $t9, $a2, $t5
    pmaddh  $t9, $t2, $t9
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    lq      $a3, 0x260($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $a2, $t7, $t6
    pmaddh  $a2, $t0, $a2
    qfsrv   $a2, $t8, $t7
    pmaddh  $a2, $t1, $a2
    qfsrv   $a2, $a3, $t8
    pmaddh  $a2, $t2, $a2
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x20($a0)
    mtsah   $zero, 0x5
    lq      $a2, 0x30($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $t9, $t4, $t3
    pmaddh  $t9, $t0, $t9
    qfsrv   $t9, $t5, $t4
    pmaddh  $t9, $t1, $t9
    qfsrv   $t9, $a2, $t5
    pmaddh  $t9, $t2, $t9
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    lq      $a3, 0x260($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $a2, $t7, $t6
    pmaddh  $a2, $t0, $a2
    qfsrv   $a2, $t8, $t7
    pmaddh  $a2, $t1, $a2
    qfsrv   $a2, $a3, $t8
    pmaddh  $a2, $t2, $a2
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x28($a0)
    mtsah   $zero, 0x6
    lq      $a2, 0x30($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $t9, $t4, $t3
    pmaddh  $t9, $t0, $t9
    qfsrv   $t9, $t5, $t4
    pmaddh  $t9, $t1, $t9
    qfsrv   $t9, $a2, $t5
    pmaddh  $t9, $t2, $t9
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    lq      $a3, 0x260($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $a2, $t7, $t6
    pmaddh  $a2, $t0, $a2
    qfsrv   $a2, $t8, $t7
    pmaddh  $a2, $t1, $a2
    qfsrv   $a2, $a3, $t8
    pmaddh  $a2, $t2, $a2
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    sw      $a3, 0x30($a0)
    mtsah   $zero, 0x7
    lq      $a2, 0x30($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $t9, $t4, $t3
    pmaddh  $t9, $t0, $t9
    qfsrv   $t9, $t5, $t4
    pmaddh  $t9, $t1, $t9
    qfsrv   $t9, $a2, $t5
    pmaddh  $t9, $t2, $t9
    pmfhi   $t9
    pmflo   $a3
    paddsw  $t9, $a3, $t9
    pcpyud  $a3, $t9, $t9
    paddsw  $t9, $a3, $t9
    prot3w  $a3, $t9
    paddsw  $t9, $a3, $t9
    lq      $a3, 0x260($a1)
    pmthi   $zero
    pmtlo   $zero
    qfsrv   $a2, $t7, $t6
    pmaddh  $a2, $t0, $a2
    qfsrv   $a2, $t8, $t7
    pmaddh  $a2, $t1, $a2
    qfsrv   $a2, $a3, $t8
    pmaddh  $a2, $t2, $a2
    pmfhi   $a2
    pmflo   $a3
    paddsw  $a2, $a3, $a2
    pcpyud  $a3, $a2, $a2
    paddsw  $a2, $a3, $a2
    prot3w  $a3, $a2
    paddsw  $a2, $a3, $a2
    psubsw  $a3, $t9, $a2
    paddsw  $a2, $t9, $a2
    pextlw  $a3, $a2, $a3
    psraw   $a3, $a3, 15
    pmtlo   $a3
    pmfhl.sh $a3
    jr      $ra
     sw      $a3, 0x38($a0)
    nop
    .end h_iqmf_fb1
    .size h_iqmf_fb1, . - h_iqmf_fb1

    .globl h_d_make
    .type h_d_make, @function
    .ent h_d_make
h_d_make:
    lq      $t0, 0x0($a0)
    lq      $t1, 0x10($a0)
    lq      $t2, 0x0($a1)
    lq      $t3, 0x10($a1)
    psubsw  $t4, $t0, $t2
    paddsw  $t5, $t0, $t2
    psraw   $t4, $t4, 9
    psraw   $t5, $t5, 9
    psubsw  $t6, $t1, $t3
    paddsw  $t7, $t1, $t3
    psraw   $t6, $t6, 9
    psraw   $t7, $t7, 9
    pmtlo   $t4
    pmthi   $t6
    pmfhl.sh $t8
    pexcw   $t8, $t8
    sq      $t8, 0x0($a2)
    pmtlo   $t5
    pmthi   $t7
    pmfhl.sh $t8
    pexcw   $t8, $t8
    jr      $ra
     sq      $t8, 0x0($a3)
    nop
    .end h_d_make
    .size h_d_make, . - h_d_make

    .globl imdct_sub
    .type imdct_sub, @function
    .ent imdct_sub
imdct_sub:
    mtsah   $zero, 0x2
    srl     $a2, $a2, 3
loop:
    addi    $a2, $a2, -0x1
    lq      $t0, 0x0($a0)
    lq      $t1, 0x0($a1)
    qfsrv   $t1, $t1, $t1
    pexew   $t1, $t1
    psubsw  $t0, $t0, $t1
    psraw   $t0, $t0, 1
    sq      $t0, 0x0($a0)
    addi    $a0, $a0, 0x10
    bgtz    $a2, loop
     addi    $a1, $a1, -0x10
    jr      $ra
     nop
    nop
    .end imdct_sub
    .size imdct_sub, . - imdct_sub

    .globl imdct_ch
    .type imdct_ch, @function
    .ent imdct_ch
imdct_ch:
    mtsah   $zero, 0x2
    lq      $t0, 0x0($a0)
    lq      $t1, 0x0($a1)
    lq      $t2, 0x0($a2)
    lq      $t9, 0x0($a3)
    psraw   $t3, $t0, 16
    qfsrv   $t4, $t3, $t3
    psllvw  $t3, $t3, $zero
    psllvw  $t4, $t4, $zero
    pmultw  $t3, $t3, $t2
    pmultw  $t4, $t4, $t2
    ppacw   $t3, $t4, $t3
    pexcw   $t3, $t3
    psraw   $t8, $t0, 1
    pand    $t5, $t8, $t9
    qfsrv   $t6, $t5, $t5
    psllvw  $t5, $t5, $zero
    psllvw  $t6, $t6, $zero
    pmultw  $t5, $t5, $t2
    pmultw  $t6, $t6, $t2
    ppacw   $t5, $t6, $t5
    pexcw   $t5, $t5
    psraw   $t5, $t5, 15
    paddsw  $t3, $t3, $t5
    psubsw  $t0, $t1, $t3
    psraw   $t0, $t0, 1
    paddsw  $t1, $t1, $t3
    psraw   $t1, $t1, 1
    sq      $t0, 0x0($a0)
    jr      $ra
     sq      $t1, 0x0($a1)
    nop
    .end imdct_ch
    .size imdct_ch, . - imdct_ch

    .globl imdct_ch2
    .type imdct_ch2, @function
    .ent imdct_ch2
imdct_ch2:
    mtsah   $zero, 0x2
    lq      $t1, 0x0($a1)
    pcpyud  $t0, $t1, $t1
    lq      $t2, 0x0($a2)
    lq      $t9, 0x0($a3)
    psraw   $t3, $t0, 16
    pexcw   $t3, $t3
    psllvw  $t3, $t3, $zero
    pmultw  $t3, $t3, $t2
    psraw   $t8, $t0, 1
    pand    $t5, $t8, $t9
    pexcw   $t5, $t5
    psllvw  $t5, $t5, $zero
    pmultw  $t5, $t5, $t2
    psraw   $t5, $t5, 15
    paddsw  $t3, $t3, $t5
    pexcw   $t3, $t3
    psubsw  $t0, $t1, $t3
    psraw   $t0, $t0, 1
    paddsw  $t1, $t1, $t3
    psraw   $t1, $t1, 1
    sd      $t0, 0x0($a0)
    jr      $ra
     sd      $t1, 0x0($a1)
    nop
    .end imdct_ch2
    .size imdct_ch2, . - imdct_ch2

    .globl final_dout
    .type final_dout, @function
    .ent final_dout
final_dout:
    addiu   $a2, $zero, 0x100
loop2:
    addi    $a2, $a2, -0x8
    lq      $t0, 0x0($a0)
    addi    $a0, $a0, 0x10
    sq      $t0, 0x0($a1)
    nop
    bgtz    $a2, loop2
     addi    $a1, $a1, 0x10
    jr      $ra
     nop
    .end final_dout
    .size final_dout, . - final_dout
