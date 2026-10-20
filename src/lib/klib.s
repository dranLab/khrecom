# EE kernel system call stubs (Sony's klib.s): each loads its call number into $v1 and traps.

    .set noreorder
    .set noat

    .macro syscall_stub name, number
    .text
    .align 4
    .globl \name
    .type \name, @function
    .ent \name
\name:
    addiu   $v1, $zero, \number
    syscall
    jr      $ra
    nop
    .end \name
    .size \name, . - \name
    .endm

    syscall_stub RFU000_FullReset, 0x0
    syscall_stub ResetEE, 0x1
    syscall_stub SetGsCrt, 0x2
    syscall_stub RFU003, 0x3
    syscall_stub _Exit, 0x4
    syscall_stub RFU005, 0x5
    syscall_stub _LoadExecPS2, 0x6
    syscall_stub _ExecPS2, 0x7
    syscall_stub RFU008, 0x8
    syscall_stub RFU009, 0x9
    syscall_stub AddSbusIntcHandler, 0xa
    syscall_stub RemoveSbusIntcHandler, 0xb
    syscall_stub Interrupt2Iop, 0xc
    syscall_stub SetVTLBRefillHandler, 0xd
    syscall_stub SetVCommonHandler, 0xe
    syscall_stub SetVInterruptHandler, 0xf
    syscall_stub AddIntcHandler, 0x10
    syscall_stub AddIntcHandler2, 0x10
    syscall_stub RemoveIntcHandler, 0x11
    syscall_stub AddDmacHandler, 0x12
    syscall_stub AddDmacHandler2, 0x12
    syscall_stub RemoveDmacHandler, 0x13
    syscall_stub _EnableIntc, 0x14
    syscall_stub _DisableIntc, 0x15
    syscall_stub _EnableDmac, 0x16
    syscall_stub _DisableDmac, 0x17
    syscall_stub SetAlarm, 0xfc
    syscall_stub ReleaseAlarm, 0xfd
    syscall_stub _iEnableIntc, -0x1a
    syscall_stub _iDisableIntc, -0x1b
    syscall_stub _iEnableDmac, -0x1c
    syscall_stub _iDisableDmac, -0x1d
    syscall_stub iSetAlarm, -0xfe
    syscall_stub iReleaseAlarm, -0xff
    syscall_stub CreateThread, 0x20
    syscall_stub DeleteThread, 0x21
    syscall_stub _StartThread, 0x22
    syscall_stub ExitThread, 0x23
    syscall_stub ExitDeleteThread, 0x24
    syscall_stub TerminateThread, 0x25
    syscall_stub iTerminateThread, -0x26
    syscall_stub DisableDispatchThread, 0x27
    syscall_stub EnableDispatchThread, 0x28
    syscall_stub ChangeThreadPriority, 0x29
    syscall_stub iChangeThreadPriority, -0x2a
    syscall_stub RotateThreadReadyQueue, 0x2b
    syscall_stub _iRotateThreadReadyQueue, -0x2c
    syscall_stub ReleaseWaitThread, 0x2d
    syscall_stub iReleaseWaitThread, -0x2e
    syscall_stub GetThreadId, 0x2f
    syscall_stub ReferThreadStatus, 0x30
    syscall_stub iReferThreadStatus, -0x31
    syscall_stub SleepThread, 0x32
    syscall_stub WakeupThread, 0x33
    syscall_stub _iWakeupThread, -0x34
    syscall_stub CancelWakeupThread, 0x35
    syscall_stub iCancelWakeupThread, -0x36
    syscall_stub SuspendThread, 0x37
    syscall_stub _iSuspendThread, -0x38
    syscall_stub ResumeThread, 0x39
    syscall_stub iResumeThread, -0x3a
    syscall_stub JoinThread, 0x3b
    syscall_stub RFU060, 0x3c
    syscall_stub RFU061, 0x3d
    syscall_stub EndOfHeap, 0x3e
    syscall_stub RFU063, 0x3f
    syscall_stub CreateSema, 0x40
    syscall_stub DeleteSema, 0x41
    syscall_stub SignalSema, 0x42
    syscall_stub iSignalSema, -0x43
    syscall_stub WaitSema, 0x44
    syscall_stub PollSema, 0x45
    syscall_stub iPollSema, -0x46
    syscall_stub ReferSemaStatus, 0x47
    syscall_stub iReferSemaStatus, -0x48
    syscall_stub RFU073, 0x49
    syscall_stub SetOsdConfigParam, 0x4a
    syscall_stub GetOsdConfigParam, 0x4b
    syscall_stub GetGsHParam, 0x4c
    syscall_stub GetGsVParam, 0x4d
    syscall_stub SetGsHParam, 0x4e
    syscall_stub SetGsVParam, 0x4f
    syscall_stub RFU080_CreateEventFlag, 0x50
    syscall_stub RFU081_DeleteEventFlag, 0x51
    syscall_stub RFU082_SetEventFlag, 0x52
    syscall_stub RFU083_iSetEventFlag, -0x53
    syscall_stub RFU084_ClearEventFlag, 0x54
    syscall_stub RFU085_iClearEventFlag, -0x55
    syscall_stub RFU086_WaitEvnetFlag, 0x56
    syscall_stub RFU087_PollEvnetFlag, 0x57
    syscall_stub RFU088_iPollEvnetFlag, -0x58
    syscall_stub RFU089_ReferEventFlagStatus, 0x59
    syscall_stub RFU090_iReferEventFlagStatus, -0x5a
    syscall_stub RFU091, 0x5b
    syscall_stub EnableIntcHandler, 0x5c
    syscall_stub iEnableIntcHandler, -0x5c
    syscall_stub DisableIntcHandler, 0x5d
    syscall_stub iDisableIntcHandler, -0x5d
    syscall_stub EnableDmacHandler, 0x5e
    syscall_stub iEnableDmacHandler, -0x5e
    syscall_stub DisableDmacHandler, 0x5f
    syscall_stub iDisableDmacHandler, -0x5f
    syscall_stub KSeg0, 0x60
    syscall_stub EnableCache, 0x61
    syscall_stub DisableCache, 0x62
    syscall_stub GetCop0, 0x63
    syscall_stub FlushCache, 0x64
    syscall_stub CpuConfig, 0x66
    syscall_stub iGetCop0, -0x67
    syscall_stub iFlushCache, -0x68
    syscall_stub iCpuConfig, -0x6a
    syscall_stub sceSifStopDma, 0x6b
    syscall_stub SetCPUTimerHandler, 0x6c
    syscall_stub SetCPUTimer, 0x6d
    syscall_stub SetOsdConfigParam2, 0x6e
    syscall_stub GetOsdConfigParam2, 0x6f
    syscall_stub GsGetIMR, 0x70
    syscall_stub iGsGetIMR, -0x70
    syscall_stub GsPutIMR, 0x71
    syscall_stub iGsPutIMR, -0x71
    syscall_stub SetPgifHandler, 0x72
    syscall_stub SetVSyncFlag, 0x73
    syscall_stub RFU116, 0x74
    syscall_stub _print, 0x75
    syscall_stub sceSifDmaStat, 0x76
    syscall_stub isceSifDmaStat, -0x76
    syscall_stub _sceSifSetDma, 0x77
    syscall_stub _isceSifSetDma, -0x77
    syscall_stub sceSifSetDChain, 0x78
    syscall_stub isceSifSetDChain, -0x78
    syscall_stub sceSifSetReg, 0x79
    syscall_stub sceSifGetReg, 0x7a
    syscall_stub _ExecOSD, 0x7b
    syscall_stub Deci2Call, 0x7c
    syscall_stub PSMode, 0x7d
    syscall_stub MachineType, 0x7e
    syscall_stub GetMemorySize, 0x7f
    syscall_stub _InitTLB, 0x82

    .data
    .globl __ps2_klibinfo__
__ps2_klibinfo__:
    .ascii "PsIIlibkernl3100"

# SOURCE DATA: .data
