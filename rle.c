// Run-Length Encoding/Decoding
// assumes input is ASCII
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void rle_encode(const char *s);
void rle_decode(const char *s);

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: %s [-e|-d] <string>\n", argv[0]);
        return 1;
    }

    if (argv[1][0] != '-' || (argv[1][0] == '-' && argv[1][1] != 'e' && argv[1][1] != 'd')) {
        printf("error: expected -e or -d\n");
        return 1;
    }

    char opt = argv[1][1];
    if (opt == 'e') rle_encode(argv[2]);
    else if (opt == 'd') rle_decode(argv[2]);
    printf("\n");
    return 0;
}

void rle_encode(const char *s)
{
    int counts[128] = { 0 };
    int s_sz = strlen(s);
    char curr = s[0];
    for (int i = 0; i < s_sz; ++i) {
        if (s[i] != curr) {
            printf("%c%d", curr, counts[curr]);
            counts[curr] = 0;
            curr = s[i];
        }
        ++counts[curr];
    }
    printf("%c%d", curr, counts[curr]);
}

void rle_decode(const char *s)
{
    int s_sz = strlen(s);
    // Input comes as pairs <char><number>
    int i = 0;
    char stored = 0;
    while (i < s_sz) {
        int times = 0;
        if (!isdigit(s[i])) {
            stored = s[i];
            ++i;
        } else {
            times = 10*times + s[i] - '0';
            ++i;
            while (isdigit(s[i])) {
                times = 10*times + s[i] - '0';
                ++i;
            }
        }

        for (int i = 0; i < times; ++i)
            printf("%c", stored);
    }
}

