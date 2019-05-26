/**
 * Base64 encoding/decoding API
 *
 *
 */

#ifndef _BASE64_H_
#define _BASE64_H_

#include <stdio.h>
#include <stdint.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//--------------------------------------------------------------------------------------------------
/**
 * base64 data encoding.
 * @return
 *      - TRUE if succeeds
 *      - FALSE if failed
 */
//--------------------------------------------------------------------------------------------------
int base64_encode
(
    const uint8_t *dataPtr,     // < [IN]
    size_t dataLength,          // < [IN]
    char *resultPtr,            // < [OUT]
    size_t *resultSizePtr       // < [INOUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Decode base64-encoded data.
 *
 * @return
 *      - TRUE if succeeds
 *      - FALSE if failed
 */
//--------------------------------------------------------------------------------------------------
int base64_decode
(
    const char *srcPtr, ///< [IN]
    size_t srcLen,      ///< [IN]
    uint8_t *dstPtr,    ///< [OUT]
    size_t *dstLenPtr   ///< [INOUT]
);

#endif /*_BASE64_H_*/
