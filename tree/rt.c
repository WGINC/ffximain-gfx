/* rt.c -- freestanding helpers so reconstruction DLLs import nothing.
 * GCC/clang may emit calls to these even under -ffreestanding. */
#include <stddef.h>

void *memcpy(void *d, const void *s, size_t n)
{
    unsigned char *dp = d;
    const unsigned char *sp = s;
    while (n--) *dp++ = *sp++;
    return d;
}

void *memmove(void *d, const void *s, size_t n)
{
    unsigned char *dp = d;
    const unsigned char *sp = s;
    if (dp < sp) {
        while (n--) *dp++ = *sp++;
    } else {
        dp += n;
        sp += n;
        while (n--) *--dp = *--sp;
    }
    return d;
}

void *memset(void *d, int c, size_t n)
{
    unsigned char *dp = d;
    while (n--) *dp++ = (unsigned char)c;
    return d;
}

int memcmp(const void *a, const void *b, size_t n)
{
    const unsigned char *x = a, *y = b;
    for (; n; --n, ++x, ++y)
        if (*x != *y) return *x < *y ? -1 : 1;
    return 0;
}

/* Minimal DLL entry point, so every reconstruction DLL links without the CRT. The module is
 * only ever mapped by the test harness or by a proxy. */
int __stdcall DllMainCRTStartup(void *module, unsigned reason, void *reserved)
{
    (void)module;
    (void)reason;
    (void)reserved;
    return 1;
}

/* Hex-Rays prints the original's inline string loops (repne scasb / rep movs) as these calls. */
size_t strlen(const char *s) { const char *p = s; while (*p) ++p; return (size_t)(p - s); }
char *strcpy(char *d, const char *s) { char *r = d; while ((*d++ = *s++)) {} return r; }
char *strcat(char *d, const char *s) { strcpy(d + strlen(d), s); return d; }
int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { ++a; ++b; }
    return (unsigned char)*a < (unsigned char)*b ? -1 : (unsigned char)*a > (unsigned char)*b;
}
