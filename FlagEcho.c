#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

/* constant needed to map 'a' to '🇦' */
#define MAGIC_OFFSET (0x1F185)

void
print_usage(const char *prog_name)
{
    fwprintf(stderr, L"Usage: %s {ISO 3166-2 Country Code}\n", prog_name);
}

int
flag_convert(wchar_t *a, size_t const len)
{
    size_t i;
    /* two is the max length of Unicode flag digraph characters */
    for (i = 0; (i < len) && (i < 2); ++i)
    {
        if (iswupper(a[i])) {
            a[i] += 0x20 + MAGIC_OFFSET;
        } else if (iswlower(a[i])) {
            a[i] += MAGIC_OFFSET;
        } else {
            return 1;
        }
    }

    return 0;
}

int
main(int argc, char* argv[])
{
    size_t const buffer_size = 4;
    wchar_t ws[buffer_size];

    /* ensure we are in a UTF-8 locale */
    setlocale(LC_ALL, "en_US.UTF-8");

    switch(argc) {
        case 1: 
            print_usage(argv[0]);
            return 2;
        case 2: 
            /* check input string length */
            if (strlen(argv[1]) != 2) {
                fwprintf(stderr, L"error: invalid string length\n");
                return 1;
            }
            
            break;
        default:
            print_usage(argv[0]);
            return 2;
    }
    
    /* convert to a wide character */
    swprintf(ws, buffer_size, L"%hs", argv[1]);

    /* convert the flag and print */
    if (flag_convert(ws, buffer_size)) {
        fwprintf(stderr, L"error: out of range character entered\n");
        return 1;
    }
    
    wprintf(ws);
    return 0;
}
