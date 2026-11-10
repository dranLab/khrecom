/* Version switches (docs/retail-plan.md). The sources build the beta (SLUS_217.99, the reference) by default and
 * the US retail ELF with -DKH_RETAIL (config/SLUS_217.99_retail/build.yaml). Retail is the release configuration of
 * the same source (docs/retail.md): asserts, debug allocations, debug prints and debug-only members compiled out.
 * Included by khtypes.h. Every use in a header or source must stay on its line (__LINE__ is in the beta's assert
 * and allocation strings): KH_DEBUG(...) around a member or statement, `(Name)` in the declarations of the macros
 * below (a parenthesized name is no macro call). */
#ifndef KHVERSION_H
#define KHVERSION_H

#ifdef KH_RETAIL
#define KH_DEBUG(x...)
#define KH_RELEASE(x...) x
/* The debug calls vanish with their arguments (no strings); an assert's condition stays an expression statement
 * (dropped unless it has side effects). */
#define AssertApp(file, line, msg, exp) ((void)0)
#define com_malloc_d(bytes, msg, ra) com_malloc(bytes)
#define com_memalign_d(alignment, bytes, msg, ra) com_memalign(alignment, bytes)
#define com_calloc_d(n, size, msg, ra) com_calloc(n, size)
#define com_realloc_d(mem, size, msg, ra) com_realloc(mem, size)
#define TRACE(args...) ((void)0)
#define TRACE_COL(args...) ((void)0)
#define TRACE_USER(args...) ((void)0)
#define TRACE_USER_COL(args...) ((void)0)
#else
#define KH_DEBUG(x...) x
#define KH_RELEASE(x...)
#endif

#endif /* KHVERSION_H */
