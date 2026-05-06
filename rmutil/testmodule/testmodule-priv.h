#ifndef TESTMODULE_PRIV_H
#define TESTMODULE_PRIV_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "redismodule.h"

static int test__abort_on_fail = 0;

static inline void dump_int_value(const char *name, uint64_t value, FILE *out) {
  fprintf(out, "\t%s: U=%llu I=%lld H=0x%llx\n", name, (unsigned long long)value, (long long)value,
          (unsigned long long)value);
}

extern int test__status_g;
extern RedisModuleCtx *test__curctx;

#define TEST_DUMP_PREAMBLE(file, line) fprintf(stderr, "Assertion failed at %s:%d\n", file, line)
#define TEST_MARK_FAIL()       \
  do {                         \
    test__status_g++;          \
    if (test__abort_on_fail) { \
      abort();                 \
    }                          \
  } while (0);

typedef void (*test__function)(RedisModuleCtx *);

typedef struct {
  const char *testname;
  test__function fn;
} test__entry;

typedef struct {
  const char *classname;
  test__entry *entries;
  size_t nentries;
} test__class;

typedef struct {
  test__class *classes;
  size_t nclasses;
} test__master_list;

extern test__master_list tests__alltests_g;

static inline void test__addclass(const char *classname) {
  test__master_list *master = &tests__alltests_g;

  if (!master->nclasses) {
    // Technically waste an entry here. meh.
    master->classes = calloc(1, sizeof(*master->classes));
  }

  master->nclasses++;
  master->classes = realloc(master->classes, sizeof(*master->classes) * master->nclasses);
  test__class *cls = master->classes + (master->nclasses - 1);

  cls->classname = classname;
  cls->entries = calloc(1, sizeof(*cls->entries));
  cls->nentries = 0;
}

static inline void test__addfn(const char *classname, const char *testname, test__function fn) {
  // Find the class
  test__master_list *master = &tests__alltests_g;
  test__class *cls = NULL;
  for (size_t ii = 0; ii < master->nclasses; ++ii) {
    if (!strcmp(master->classes[ii].classname, classname)) {
      cls = master->classes + ii;
      break;
    }
  }
  if (!cls) {
    fprintf(stderr, "Adding test to class '%s' which was not yet created\n", classname);
    abort();
  }

  cls->nentries++;
  cls->entries = realloc(cls->entries, (sizeof *cls->entries) * cls->nentries);

  test__entry *newent = cls->entries + (cls->nentries - 1);
  newent->fn = fn;
  newent->testname = testname;
}

static inline void test__runtests(RedisModuleCtx *ctx) {
  test__master_list *master = &tests__alltests_g;
  for (size_t ii = 0; ii < master->nclasses; ++ii) {
    test__class *cls = master->classes + ii;
    fprintf(stderr, "=== CLASS '%s' (%lu tests) ===\n", cls->classname, cls->nentries);

    for (size_t jj = 0; jj < cls->nentries; ++jj) {
      test__entry *ent = cls->entries + jj;
      fprintf(stderr, "--- TEST %s.%s (%lu/%lu) ---\n", cls->classname, ent->testname, jj + 1,
              cls->nentries);
      cls->entries[jj].fn(ctx);
    }

    // Once we're done with the test class, free it to appease valgrind:
    free(cls->entries);
  }
  free(master->classes);
}

#define TEST_DEFINE_GLOBALS()                                             \
  test__master_list tests__alltests_g = {.classes = NULL, .nclasses = 0}; \
  int test__status_g = 0;                                                 \
  RedisModuleCtx *test__curctx;

#define TEST_CLASS(n)                                                       \
  static void __attribute__((constructor(100))) test__initclass_##n(void) { \
    test__addclass(#n);                                                     \
  }                                                                         \
  static const char __attribute__((unused)) *test__cur_class = #n;

#define TEST_F(name)                                                           \
  static void test__##name(RedisModuleCtx *);                                  \
  static void __attribute__((constructor(200))) test__addentry__##name(void) { \
    test__addfn(test__cur_class, #name, test__##name);                         \
  }                                                                            \
  static void test__##name

#define TEST_RUN_ALL_TESTS(ctx) test__runtests(ctx)

#define RMTEST_STRTYPE_CSTR 1
#define RMTEST_STRTYPE_RSTR 2
#define RMTEST_AUTOLEN ((size_t)-1)

int RMTest__CheckKeyExistsC(RedisModuleCtx *ctx, const char *key);
int RMTest__CheckKeyExistsR(RedisModuleCtx *ctx, const RedisModuleString *key);

static inline void assert_int_helper(const char *var_a, const char *var_b, uint64_t val_a,
                                     uint64_t val_b, const char *oper, const char *file, int line) {
  TEST_DUMP_PREAMBLE(file, line);
  fprintf(stderr, "Expected: %s %s %s\n", var_a, oper, var_b);
  dump_int_value(var_a, val_a, stderr);
  dump_int_value(var_b, val_b, stderr);
  TEST_MARK_FAIL();
}

static inline void assert_bool_helper(const char *var, int exp, const char *file, int line) {
  TEST_DUMP_PREAMBLE(file, line);
  fprintf(stderr, "Assertion failed at %s:%d\n", file, line);
  fprintf(stderr, "Expected '%s' to be %s. Is %s\n", var, exp ? "true" : "false",
          exp ? "false" : "true");
  TEST_MARK_FAIL();
}

static inline void assert_string_helper(const char *var_a, const char *var_b, const char *exp,
                                        size_t explen, const char *got, size_t gotlen,
                                        const char *file, int line) {
  if (explen == gotlen && strncmp(exp, got, explen) == 0) {
    return;
  }
  TEST_DUMP_PREAMBLE(file, line);
  fprintf(stderr, "Expected '%s' = '%s': <<%.*s>>\n", var_a, var_b, (int)explen, exp);
  fprintf(stderr, "\tGot: <<%.*s>>\n", (int)gotlen, got);
  TEST_MARK_FAIL();
}

#define ASSERT_COMMON(a, b, oper)                                                   \
  do {                                                                              \
    uint64_t test__rv_a = (uint64_t)(a);                                            \
    uint64_t test__rv_b = (uint64_t)(b);                                            \
    if (!(test__rv_a oper test__rv_b)) {                                            \
      assert_int_helper(#a, #b, test__rv_a, test__rv_b, #oper, __FILE__, __LINE__); \
    }                                                                               \
  } while (0)

#define ASSERT_BOOL_COMMON(expr, want)                     \
  do {                                                     \
    int test__rv = !!expr;                                 \
    if (test__rv != want) {                                \
      assert_bool_helper(#expr, want, __FILE__, __LINE__); \
    }                                                      \
  } while (0);

#define ASSERT_STR_COMMON(v1, v2, s1, n1, s2, n2) \
  assert_string_helper(v1, v2, s1, n1, s2, n2, __FILE__, __LINE__)

#endif