#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "testmodule-priv.h"

// ==
#define ASSERT_EQ(a, b) ASSERT_COMMON(a, b, ==)
// !=
#define ASSERT_NE(a, b) ASSERT_COMMON(a, b, !=)
// >
#define ASSERT_GT(a, b) ASSERT_COMMON(a, b, >)
// >=
#define ASSERT_GE(a, b) ASSERT_COMMON(a, b, >=)
// <
#define ASSERT_LT(a, b) ASSERT_COMMON(a, b, <)
// <=
#define ASSERT_LE(a, b) ASSERT_COMMON(a, b, <=)

// Use these macros for better print-outs
#define ASSERT_TRUE(x) ASSERT_BOOL_COMMON(x, 1)
#define ASSERT_FALSE(x) ASSERT_BOOL_COMMON(x, 0)

// Compare RedisModuleString s1 with char s2
#define ASSERT_STREQ_C(s1, s2) ASSERT_STR_COMMON(#s1, #s2, s1, strlen(s1), s2, strlen(s2))

// Compare two RedisModuleString objects
#define ASSERT_STREQ_R(r1, r2)                                           \
  do {                                                                   \
    size_t test__n1;                                                     \
    size_t test__n2;                                                     \
    const char *test__s1 = RedisModule_StringPtrLen(r1, &test__n1);      \
    const char *test__s2 = RedisModule_StringPtrLen(r2, &test__n2);      \
    ASSERT_STR_COMMON(#r1, #r2, test__s1, test__n1, test__s2, test__n2); \
  } while (0);

#define ASSERT_STREQ_CR(s, r)                                      \
  do {                                                             \
    size_t test__n1;                                               \
    const char *test__s1 = RedisModule_StringPtrLen(r, &test__n1); \
    ASSERT_STR_COMMON(#s, #r, s, strlen(s), test__s1, test__n1);   \
  } while (0);

#define ASSERT_KEY_EXISTS_C(key) ASSERT_TRUE(RMTest__CheckKeyExistsC(test__curctx, key))
#define ASSERT_KEY_EXISTS_R(key) ASSERT_TRUE(RMTest__CheckKeyExistsR(test__curctx, key))

#define ASSERT_KEY_MISSING_C(key) ASSERT_FALSE(RMTest__CheckKeyExistsC(test__curctx, key))
#define ASSERT_KEY_MISSING_R(key) ASSERT_FALSE(RMTest__CheckKeyExistsR(test__curctx, key));

/** Returns an array of RedisModuleString. No need to free */
RedisModuleString **RMTest_BuildArgs(RedisModuleCtx *ctx, ...);
#endif  // TESTMODULE_H