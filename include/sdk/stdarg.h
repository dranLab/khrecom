/* stdarg for the EE (EABI, 64-bit argument slots): the compiler packages ship none. */
#ifndef SDK_STDARG_H
#define SDK_STDARG_H

typedef char *va_list;

/* the first unnamed register's slot: __builtin_next_arg points past the register save area */
#define va_start(ap, last) \
    ((ap) = (char *)__builtin_next_arg(last) - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
#define va_arg(ap, type) (*(type *)(((ap) += 8) - 8))
#define va_end(ap) ((void)0)

#endif
