#include "redismodule.h"
#include "testmodule.h"
#include <stdarg.h>

TEST_DEFINE_GLOBALS();

static int isFilterMatch(const char *filter, size_t nfilter, const char *testname, int isPrefix) {
  if (filter == NULL) {
    return 1;
  } else if (!isPrefix) {
    if (nfilter != strlen(testname)) {
      return 0;
    } else {
      return !strncmp(filter, testname, nfilter);
    }
  } else {
    // Is a prefix:
    return !strncmp(filter, testname, nfilter);
  }
}

static int TM_RunTests(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);
  const char *filter = NULL;
  size_t nfilter = 0;
  int isPrefix = 0;

  if (argc >= 2) {
    filter = RedisModule_StringPtrLen(argv[1], &nfilter);
    if (filter[nfilter - 1] == '*') {
      isPrefix = 1;
      nfilter--;
    }
  }

  for (size_t ii = 0; ii < tests__alltests_g.nclasses; ++ii) {
    const test__class *cls = tests__alltests_g.classes + ii;
    for (size_t jj = 0; jj < cls->nentries; ++jj) {
      char fqName[128] = {0};
      const test__entry *ent = cls->entries + jj;
      sprintf(fqName, "%s.%s", cls->classname, ent->testname);
      if (!isFilterMatch(filter, nfilter, fqName, isPrefix)) {
        fprintf(stderr, "Skipping: %s (filtered out)\n", ent->testname);
        continue;
      }
      fprintf(stderr, "Running: %s\n", fqName);
      test__curctx = ctx;
      ent->fn(ctx);
    }
  }

  RedisModule_ReplyWithLongLong(ctx, test__status_g);
  return REDISMODULE_OK;
}

static int TM_ListTests(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  RedisModule_AutoMemory(ctx);
  RedisModule_ReplyWithArray(ctx, REDISMODULE_POSTPONED_ARRAY_LEN);
  size_t arrlen = 0;
  for (size_t ii = 0; ii < tests__alltests_g.nclasses; ++ii) {
    const test__class *cls = tests__alltests_g.classes + ii;
    for (size_t jj = 0; jj < cls->nentries; ++jj) {
      const test__entry *ent = cls->entries + jj;
      RedisModule_ReplyWithString(
          ctx, RedisModule_CreateStringPrintf(ctx, "%s.%s", cls->classname, ent->testname));
      ++arrlen;
    }
  }
  RedisModule_ReplySetArrayLength(ctx, arrlen);
  return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (RedisModule_Init(ctx, "tm", 1, REDISMODULE_APIVER_1) != REDISMODULE_OK) {
    return REDISMODULE_ERR;
  }

  if (RedisModule_CreateCommand(ctx, "TM.RUNTESTS", TM_RunTests, "readonly", 0, 0, 0) !=
      REDISMODULE_OK)
    return REDISMODULE_ERR;
  if (RedisModule_CreateCommand(ctx, "TM.LIST", TM_ListTests, "readonly", 0, 0, 0) !=
      REDISMODULE_OK)
    return REDISMODULE_ERR;

  return REDISMODULE_OK;
}

static int RMTest__CheckKeyExistsCommon(RedisModuleCtx *ctx, const void *k, size_t n, int isCstr) {
  RedisModuleString *s;
  if (isCstr) {
    s = RedisModule_CreateString(ctx, k, n);
  } else {
    s = (void *)k;
  }

  RedisModuleKey *rkey = RedisModule_OpenKey(ctx, s, REDISMODULE_READ);
  int rv = rkey != NULL;
  if (rkey) {
    RedisModule_CloseKey(rkey);
  }
  if (isCstr) {
    RedisModule_FreeString(ctx, s);
  }
  return rv;
}

int RMTest__CheckKeyExistsC(RedisModuleCtx *ctx, const char *key) {
  return RMTest__CheckKeyExistsCommon(ctx, key, strlen(key), 1);
}
int RMTest__CheckKeyExistsR(RedisModuleCtx *ctx, const RedisModuleString *key) {
  return RMTest__CheckKeyExistsCommon(ctx, key, 0, 0);
}

RedisModuleString **RMTest_BuildArgs(RedisModuleCtx *ctx, ...) {
  va_list ap;
  va_start(ap, ctx);

  va_list ap2;
  va_copy(ap2, ap);

  size_t nitems = 0;
  const char *p;
  while ((p = va_arg(ap2, const char *)) != NULL) {
    ++nitems;
  }
  va_end(ap2);

  RedisModuleString **retrv = RedisModule_PoolAlloc(ctx, (sizeof *retrv) * nitems);
  for (size_t ii = 0; ii < nitems; ++ii) {
    p = va_arg(ap, const char *);
    retrv[ii] = RedisModule_CreateString(ctx, p, strlen(p));
  }
  va_end(ap);
  return retrv;
}