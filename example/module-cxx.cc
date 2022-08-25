
#define REDISMODULE_MAIN
#define REDISMODULE_EXPERIMENTAL_API
#include "cxx/moduleapi.hxx"

#include "rmutil/util.h"
#include "rmutil/strings.h"
#include "rmutil/test_util.h"

/* EXAMPLE.PARSE [SUM <x> <y>] | [PROD <x> <y>]
*  Demonstrates the automatic arg parsing utility.
*  If the command receives "SUM <x> <y>" it returns their sum
*  If it receives "PROD <x> <y>" it returns their product
*/

using namespace Redis;

int ArgExists(const char *arg, const Args& args, int offset) {

  size_t larg = strlen(arg);
  for (; offset < args.Size(); offset++) {
    size_t l;
    const char *carg = RedisModule_StringPtrLen(args[offset], &l);
    if (l != larg) continue;
    if (carg != NULL && strncasecmp(carg, arg, larg) == 0) {
      return offset;
    }
  }
  return 0;
}

#define ASSERT_NOERROR(ctx, r)                      \
  if (r == nullptr) {                               \
    return ctx.ReplyWithError("ERR reply is NULL"); \
  } else if (r.Type() == REDISMODULE_REPLY_ERROR) { \
    ctx.ReplyWithCallReply(r);                      \
    return REDISMODULE_ERR;                         \
  }

#define AssertReplyEquals(rep, cstr)                \
  RMUtil_Assert(                                    \
	RMUtil_StringEquals(                            \
	  rep.CreateString(),                           \
	  String(cstr, strlen(cstr))                    \
	)                                               \
  )

#define TEST(f)                                         \
  if (args.Size() < 2 ||                                \
  	  ArgExists(__STRING(f), args, 1)) {                \
    int rc = f(ctx);                                    \
	if (rc != REDISMODULE_OK) {                         \
      ctx.ReplyWithError("Test " __STRING(f) " FAILED");\
      return REDISMODULE_ERR;                           \
    }                                                   \
  }

#define RegisterWriteCmd(ctx, cmd, f) \
  if (Command::Create<f>(ctx, cmd, "write", 1, 1, 1) == REDISMODULE_ERR) \
    return REDISMODULE_ERR;

       
struct Parse : Cmd<Parse> {
	Parse(Context ctx, const Args& args) : Cmd(ctx, args) {
		// we must have at least 4 args
		if (_args.Size() < 4) {
			throw _ctx.WrongArity();
		}
	}
	int operator()() {
		// init auto memory for created strings
		// RedisModule_AutoMemory(ctx);

		long long x, y;

		// If we got SUM - return the sum of 2 consecutive arguments
		if (RMUtil_ParseArgsAfter("SUM", _args, _args.Size(), "ll", &x, &y) == REDISMODULE_OK) {
			_ctx.ReplyWithLongLong(x + y);
			return REDISMODULE_OK;
		}

		// If we got PROD - return the product of 2 consecutive arguments
		if (RMUtil_ParseArgsAfter("PROD", _args, _args.Size(), "ll", &x, &y) == REDISMODULE_OK) {
			_ctx.ReplyWithLongLong(x * y);
			return REDISMODULE_OK;
		}

		// something is fishy...
		_ctx.ReplyWithError("Invalid arguments");

		return REDISMODULE_ERR;
	}
};

/*
* example.HGETSET <key> <element> <value>
* Atomically set a value in a HASH key to <value> and return its value before
* the HSET.
*
* Basically atomic HGET + HSET
*/
struct HGetSet : Cmd<HGetSet> {
	HGetSet(Context ctx, const Args& args) : Cmd(ctx, args) {
		// we need EXACTLY 4 arguments
		if (_args.Size() != 4) {
			throw _ctx.WrongArity();
		}
	}

	int operator()() {
		// RedisModule_AutoMemory(ctx);

		// open the key and make sure it's indeed a HASH and not empty
		Key key(_ctx, _args[1], REDISMODULE_READ | REDISMODULE_WRITE);
		if (key.Type() != REDISMODULE_KEYTYPE_HASH && 
			key.Type() != REDISMODULE_KEYTYPE_EMPTY) {
			return _ctx.ReplyWithError(REDISMODULE_ERRORMSG_WRONGTYPE);
		}

		// get the current value of the hash element
		auto rep = _ctx.Call("HGET", "ss", _args[1], _args[2]);
		ASSERT_NOERROR(_ctx, rep);
		// if (!rep) { _ctx.Reply(rep); }

		// set the new value of the element
		auto srep = _ctx.Call("HSET", "sss", _args[1], _args[2], _args[3]);
		ASSERT_NOERROR(_ctx, srep);

		// if the value was null before - we just return null
		if (rep.Type() == REDISMODULE_REPLY_NULL) {
			return _ctx.ReplyWithNull();
		}

		// forward the HGET reply to the client
		_ctx.ReplyWithCallReply(rep);
		return REDISMODULE_OK;
	}
};

// Test the the PARSE command
int testParse(Context ctx) {
	CallReply r = ctx.Call("example.parse", "ccc", "SUM", "5", "2");
	RMUtil_Assert(r.Type() == REDISMODULE_REPLY_INTEGER);
	AssertReplyEquals(r, "7");

	r = ctx.Call("example.parse", "ccc", "PROD", "5", "2");
	RMUtil_Assert(r.Type() == REDISMODULE_REPLY_INTEGER);
	AssertReplyEquals(r, "10");

	return REDISMODULE_OK;
}

// test the HGETSET command
int testHgetSet(Context ctx) {
	CallReply r = ctx.Call("example.hgetset", "ccc", "foo", "bar", "baz");
	RMUtil_Assert(r.Type() != REDISMODULE_REPLY_ERROR);

	r = ctx.Call("example.hgetset", "ccc", "foo", "bar", "bag");
	RMUtil_Assert(r.Type() == REDISMODULE_REPLY_STRING);
	AssertReplyEquals(r, "baz");

	r = ctx.Call("example.hgetset", "ccc", "foo", "bar", "bang");
	AssertReplyEquals(r, "bag");

	return REDISMODULE_OK;
}

// Unit test entry point for the module
int TestModule(Context ctx, const Args& args) {
	// RedisModule_AutoMemory(ctx);

	TEST(testParse);
	TEST(testHgetSet);

	ctx.ReplyWithSimpleString("PASS");
	return REDISMODULE_OK;
}

extern "C" {
int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **, int) {
	// Register the module itself
	if (RedisModule_Init(ctx, "example", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
		return REDISMODULE_ERR;
	}

	// register example.parse - the default registration syntax
	if (Command::Create<Cmd<Parse>::cmdfunc>(ctx, "example.parse", "readonly", 1, 1, 1) == REDISMODULE_ERR) {
		return REDISMODULE_ERR;
	}

	// register example.hgetset - using the shortened utility registration macro
	RegisterWriteCmd(ctx, "example.hgetset", Cmd<HGetSet>::cmdfunc);

	// register the unit test
	RegisterWriteCmd(ctx, "example.test", TestModule);

	return REDISMODULE_OK;
}
}
// REDIS_MODULE(MyModule);
