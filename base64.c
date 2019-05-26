
#include "base64.h"

static const unsigned char DecodeTable[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

int base64_encode
(
    const uint8_t *srcPtr,
    size_t srcLen,
    char *dstPtr,
    size_t *dstLenPtr
)
{
    const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t *data = (const uint8_t *) srcPtr;
    size_t resultIndex = 0;
    size_t x;
    uint32_t n = 0;
    int padCount = srcLen % 3;
    uint8_t n0, n1, n2, n3;
    size_t resultSize;

    if ((NULL == dstLenPtr) || (NULL == dstPtr) || (NULL == srcPtr))
    {
        return FALSE;
    }
    resultSize = *dstLenPtr;

    for (x = 0; x < srcLen; x += 3)
    {
        /* 3 8-bit -> 24-bit */
        n = ((uint32_t) data[x]) << 16;
        if ((x + 1) < srcLen)
        {
            n += ((uint32_t)data[x + 1]) << 8;
        }
        if ((x + 2) < srcLen)
        {
            n += data[x + 2];
        }

        /* 24-bit -> 4 6-bit */
        n0 = (uint8_t)(n >> 18) & 63;
        n1 = (uint8_t)(n >> 12) & 63;
        n2 = (uint8_t)(n >> 6) & 63;
        n3 = (uint8_t)n & 63;

        // add n0, n1
        if(resultIndex >= resultSize)
        {
            return FALSE;
        }
        dstPtr[resultIndex++] = base64Chars[n0];
        if(resultIndex >= resultSize)
        {
            return FALSE;
        }
        dstPtr[resultIndex++] = base64Chars[n1];

        // add n2
        if ((x + 1) < srcLen)
        {
            if(resultIndex >= resultSize)
            {
                return FALSE;
            }
            dstPtr[resultIndex++] = base64Chars[n2];
        }

        // add n3
        if ((x + 2) < srcLen)
        {
            if(resultIndex >= resultSize)
            {
                return FALSE;
            }
            dstPtr[resultIndex++] = base64Chars[n3];
        }
    }

    // add padding
    if (padCount > 0)
    {
        for (; padCount < 3; padCount++)
        {
            if(resultIndex >= resultSize)
            {
                return FALSE;
            }
            dstPtr[resultIndex++] = '=';
        }
    }
    
    // add 0x00
    if (resultIndex >= resultSize)
    {
        return FALSE;
    }
    dstPtr[resultIndex] = 0;

    // count 0x00
    *dstLenPtr = resultIndex + 1;

    return TRUE;
}

int base64_decode
(
    const char *srcPtr,
    size_t srcLen,
    uint8_t *dstPtr,
    size_t *dstLenPtr
)
{
    const char *in = srcPtr;
    const char *end = in + srcLen;
    char iter = 0;
    uint32_t buf = 0;
    size_t len = 0;
    uint8_t *out = dstPtr;
    size_t outLen;

    if ((NULL == srcPtr) || (NULL == dstPtr) || (NULL == dstLenPtr))
    {
        return FALSE;
    }

    outLen = *dstLenPtr;

    while (in < end)
    {
        unsigned char c = DecodeTable[(int)(*in++)];

        switch (c)
        {
            case WHITESPACE:
                continue;   /* skip whitespace */
            case INVALID:
                return FALSE;  /* invalid input, return error */
            case EQUALS:                 /* pad character, end of data */
                in = end;
                continue;
            default:
                buf = buf << 6 | c;
                iter++; // increment the number of iteration
                /* If the buffer is full, split it into bytes */
                if (iter == 4)
                {
                    if ((len += 3) > outLen)
                    {
                        return FALSE; /* buffer overflow */
                    }
                    *(out++) = (buf >> 16) & 255;
                    *(out++) = (buf >> 8) & 255;
                    *(out++) = buf & 255;
                    buf = 0;
                    iter = 0;
                }
        }
    }

    if (iter == 3)
    {
        if ((len += 2) > outLen)
        {
            return FALSE;
        }
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2)
    {
        if (++len > outLen)
        {
            return FALSE;
        }
        *(out++) = (buf >> 4) & 255;
    }

    *dstLenPtr = len; /* modify to reflect the actual output size */

    return TRUE;
}
