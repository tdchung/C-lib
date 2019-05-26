#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "base64.h"

#define ASSERT(condition, res) \
        if ((condition)==res) { printf("ASSERT OK: CONDITION: %s, RESPONSE: %d\n", #condition, (int)res); } \
        else { printf("ASSERT FAILED: %s, RESPONSE: %d\n", #condition,  (int)res); }

int main (int argc, const char * argv[])
{
    char dataout[100] = {0};
    size_t dataout_len = 100;

    // test encode base 64 failed
    ASSERT(base64_encode("hello World!", sizeof("hello World!"), dataout, 0), FALSE);
    ASSERT(base64_encode("hello World!", sizeof("hello World!"), NULL, &dataout_len), FALSE);
    ASSERT(base64_encode("hello World!", sizeof("hello World!"), dataout, NULL), FALSE);
    ASSERT(base64_encode(NULL, 10, dataout, &dataout_len), FALSE);


    // test encode base
    dataout_len = 100;
    ASSERT(base64_encode("hello World!", sizeof("hello World!"), dataout, &dataout_len), TRUE);
    printf("DATA base64: %s\n", dataout);
    printf("Lenght base64: %d\n", (int)dataout_len);

    exit(0);
}
