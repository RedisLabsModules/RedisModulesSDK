
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

using namespace RedisModule;

#define ASSERT_NOERROR(ctx, r)                      \
  if (r == nullptr) {                               \
    return Reply::Error(ctx, "ERR reply is NULL");  \
  } else if (r.Type() == REDISMODULE_REPLY_ERROR) { \
    Reply::CallReply(ctx, r);                       \
    return REDISMODULE_ERR;                         \
  }

#define AssertReplyEquals(rep, cstr)                \
  RMUtil_Assert(                                    \
	RMUtil_StringEquals(                            \
	  rep.CreateString(),                           \
	  String(cstr, strlen(cstr))                    \
	)                                               \
  )

#define TEST(f)                                        \
  if (argc < 2 ||                                      \
  	  RMUtil_ArgExists(__STRING(f), argv, argc, 1)) {  \
    int rc = f(ctx);                                   \
	if (rc != REDISMODULE_OK) {                        \
      Reply::Error(ctx, "Test " __STRING(f) " FAILED");\
      return REDISMODULE_ERR;                          \
    }                                                  \
  }
       
struct Parse : Cmd<Parse> {
	Parse(Context ctx, Args& args) : Cmd(ctx, args) {
		// we must have at least 4 args
		if (_args.Size() < 4) {
			throw Reply::WrongArity(_ctx);
		}
	}
	int operator()() {
		// init auto memory for created strings
		// RedisModule_AutoMemory(ctx);

		long long x, y;

		// If we got SUM - return the sum of 2 consecutive arguments
		if (RMUtil_ParseArgsAfter("SUM", _args, _args.Size(), "ll", &x, &y) == REDISMODULE_OK) {
			Reply::LongLong(_ctx, x + y);
			return REDISMODULE_OK;
		}

		// If we got PROD - return the product of 2 consecutive arguments
		if (RMUtil_ParseArgsAfter("PROD", _args, _args.Size(), "ll", &x, &y) == REDISMODULE_OK) {
			Reply::LongLong(_ctx, x * y);
			return REDISMODULE_OK;
		}

		// something is fishy...
		Reply::Error(_ctx, "Invalid arguments");

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
	HGetSet(Context ctx, Args& args) : Cmd(ctx, args) {
		// we need EXACTLY 4 arguments
		if (_args.Size() != 4) {
			throw Reply::WrongArity(_ctx);
		}
	}

	int operator()() {
		// RedisModule_AutoMemory(ctx);

		// open the key and make sure it's indeed a HASH and not empty
		Key key(_ctx, _args[1], REDISMODULE_READ | REDISMODULE_WRITE);
		if (key.Type() != REDISMODULE_KEYTYPE_HASH && 
			key.Type() != REDISMODULE_KEYTYPE_EMPTY) {
			return Reply::Error(_ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
		}

		// get the current value of the hash element
		CallReply rep = _ctx.Call("HGET", "ss", _args[1], _args[2]);
		ASSERT_NOERROR(_ctx, rep);

		// set the new value of the element
		CallReply srep = _ctx.Call("HSET", "sss", _args[1], _args[2], _args[3]);
		ASSERT_NOERROR(_ctx, srep);

		// if the value was null before - we just return null
		if (rep.Type() == REDISMODULE_REPLY_NULL) {
			return Reply::Null(_ctx);
		}

		// forward the HGET reply to the client
		Reply::CallReply(_ctx, rep);
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
int TestModule(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
	// RedisModule_AutoMemory(ctx);

	TEST(testParse);
	TEST(testHgetSet);

	Reply::SimpleString(ctx, "PASS");
	return REDISMODULE_OK;
}

extern "C" {
int RedisModule_OnLoad(Context ctx, RedisModuleString **argv, int argc) {
	// Register the module itself
	if (RedisModule_Init(ctx, "example", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
		return REDISMODULE_ERR;
	}

	// register example.parse - the default registration syntax
	if (Command::Create(ctx, "example.parse", Cmd<Parse>::cmdfunc, "readonly", 1, 1, 1) == REDISMODULE_ERR) {
		return REDISMODULE_ERR;
	}

	// register example.hgetset - using the shortened utility registration macro
	RMUtil_RegisterWriteCmd(ctx, "example.hgetset", Cmd<HGetSet>::cmdfunc);

	// register the unit test
	RMUtil_RegisterWriteCmd(ctx, "example.test", TestModule);

	return REDISMODULE_OK;
}
}
// REDIS_MODULE(MyModule);
