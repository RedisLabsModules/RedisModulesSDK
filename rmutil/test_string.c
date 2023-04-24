#include "testmodule/testmodule.h"
#include "strings.h"

TEST_CLASS(string);

TEST_F(testEquality)(RedisModuleCtx *ctx) {
  const char *a_str = "Hello";
  const char *b_str = "World";
  RedisModuleString *a_rs = RedisModule_CreateString(ctx, a_str, strlen(a_str));
  RedisModuleString *b_rs = RedisModule_CreateString(ctx, b_str, strlen(b_str));

  ASSERT_TRUE(RMUtil_StringEquals(a_rs, a_rs));
  ASSERT_FALSE(RMUtil_StringEquals(a_rs, b_rs));
  ASSERT_TRUE(RMUtil_StringEqualsC(a_rs, "Hello"));
  ASSERT_FALSE(RMUtil_StringEqualsC(a_rs, "World"));

  ASSERT_TRUE(RMUtil_StringEqualsCaseC(a_rs, "HelLO"));
  ASSERT_FALSE(RMUtil_StringEqualsCaseC(a_rs, "HELL"));
}

TEST_F(testCaseConversion)(RedisModuleCtx *ctx) {
  RedisModuleString *a_rs = RedisModule_CreateString(ctx, "Hello", strlen("Hello"));
  RMUtil_StringToLower(a_rs);
  ASSERT_STREQ_CR("hello", a_rs);
  RMUtil_StringToUpper(a_rs);
  ASSERT_STREQ_CR("HELLO", a_rs);
}

TEST_F(testArgvConversion)(RedisModuleCtx *ctx) {
  RedisModuleString **argv = RMTest_BuildArgs(ctx, "foo", "bar", "baz", NULL);
  const char **cargs = RedisModule_PoolAlloc(ctx, sizeof(*cargs) * 3);

  const char *exps[] = {"foo", "bar", "baz"};

  // No copying
  RMUtil_StringConvert(argv, cargs, 3, 0);
  for (size_t ii = 0; ii < 3; ++ii) {
    ASSERT_STREQ_C(exps[ii], cargs[ii]);
    ASSERT_EQ(cargs[ii], RedisModule_StringPtrLen(argv[ii], NULL));
  }
  // Do the same thing, but copying

  RMUtil_StringConvert(argv, cargs, 3, RMUTIL_STRINGCONVERT_COPY);
  for (size_t ii = 0; ii < 3; ++ii) {
    ASSERT_STREQ_C(exps[ii], cargs[ii]);
    ASSERT_NE(cargs[ii], RedisModule_StringPtrLen(argv[ii], NULL));
  }
}