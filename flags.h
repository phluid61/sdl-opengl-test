#ifndef __FLAGS_H__
#define __FLAGS_H__

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t flag_t;
#define FLAG_C(v) UINT8_C(v)

#define FLAG_TEST_MASK(flags, mask, bits) ((bool)(((flag_t)(flags) & (flag_t)(mask)) == (flag_t)(bits)) )
#define FLAG_TEST_ALL(flags, bits)        ((bool)(((flag_t)(flags) & (flag_t)(bits)) == (flag_t)(bits)) )
#define FLAG_TEST_ANY(flags, bits)        ((bool)(((flag_t)(flags) & (flag_t)(bits)) != FLAG_C(0)) )
#define FLAG_TEST_NONE(flags, bits)       ((bool)(((flag_t)(flags) & (flag_t)(bits)) == FLAG_C(0)) )

#define FLAG_TEST(flags, bits) FLAG_TEST_ALL(flags, bits)

#define FLAG_SET(flags, bits)      { flags = (flag_t)(flags) |  (flag_t)(bits); }
#define FLAG_CLEAR(flags, bits)    { flags = (flag_t)(flags) & ~(flag_t)(bits); }
#define FLAG_TOGGLE(flags, bits)   { flags = (flag_t)(flags) ^  (flag_t)(bits); }

#endif

/* vim: ts=4 sts=4 sw=4
 */
