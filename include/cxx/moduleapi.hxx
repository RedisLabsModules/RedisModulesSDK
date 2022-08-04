#pragma once

#include <string>
#include "redismodule.h"

namespace RedisModule {

///////////////////////////////////////////////////////////////////////////////////////////////

struct Module {
	void *Alloc(size_t bytes);
	void *Calloc(size_t nmemb, size_t size);
	void *Realloc(void *ptr, size_t bytes);
	void Free(void *ptr);

	char *Strdup(const char *str);
	int GetApi(const char *, void *);
	long long Milliseconds(void);

	const char *StringPtrLen(const RedisModuleString *str, size_t *len);
	int StringToLongLong(const RedisModuleString *str, long long *ll);
	int StringToDouble(const RedisModuleString *str, double *d);
	int StringCompare(RedisModuleString *a, RedisModuleString *b);

	void CloseKey(RedisModuleKey *kp);
	int KeyType(RedisModuleKey *kp);
	size_t ValueLength(RedisModuleKey *kp);
	int ListPush(RedisModuleKey *kp, int where, RedisModuleString *ele);
	RedisModuleString *ListPop(RedisModuleKey *key, int where);
	int DeleteKey(RedisModuleKey *key);
	int StringSet(RedisModuleKey *key, RedisModuleString *str);
	char *StringDMA(RedisModuleKey *key, size_t *len, int mode);
	int StringTruncate(RedisModuleKey *key, size_t newlen);
	mstime_t GetExpire(RedisModuleKey *key);
	int SetExpire(RedisModuleKey *key, mstime_t expire);
	int HashSet(RedisModuleKey *key, int flags, ...);
	int HashGet(RedisModuleKey *key, int flags, ...);
	int ModuleTypeSetValue(RedisModuleKey *key, RedisModuleType *mt, void *value);
	RedisModuleType *ModuleTypeGetType(RedisModuleKey *key);
	void *ModuleTypeGetValue(RedisModuleKey *key);

	int ZsetAdd(RedisModuleKey *key, double score, RedisModuleString *ele, int *flagsptr);
	int ZsetIncrby(RedisModuleKey *key, double score, RedisModuleString *ele, int *flagsptr, double *newscore);
	int ZsetScore(RedisModuleKey *key, RedisModuleString *ele, double *score);
	int ZsetRem(RedisModuleKey *key, RedisModuleString *ele, int *deleted);
	void ZsetRangeStop(RedisModuleKey *key);
	int ZsetFirstInScoreRange(RedisModuleKey *key, double min, double max, int minex, int maxex);
	int ZsetLastInScoreRange(RedisModuleKey *key, double min, double max, int minex, int maxex);
	int ZsetFirstInLexRange(RedisModuleKey *key, RedisModuleString *min, RedisModuleString *max);
	int ZsetLastInLexRange(RedisModuleKey *key, RedisModuleString *min, RedisModuleString *max);
	RedisModuleString *ZsetRangeCurrentElement(RedisModuleKey *key, double *score);
	int ZsetRangeNext(RedisModuleKey *key);
	int ZsetRangePrev(RedisModuleKey *key);
	int ZsetRangeEndReached(RedisModuleKey *key);

	RedisModuleCtx *GetContextFromIO(RedisModuleIO *io);
	void SaveUnsigned(RedisModuleIO *io, uint64_t value);
	uint64_t LoadUnsigned(RedisModuleIO *io);
	void SaveSigned(RedisModuleIO *io, int64_t value);
	int64_t LoadSigned(RedisModuleIO *io);
	void EmitAOF(RedisModuleIO *io, const char *cmdname, const char *fmt, ...);
	void SaveString(RedisModuleIO *io, RedisModuleString *s);
	void SaveStringBuffer(RedisModuleIO *io, const char *str, size_t len);
	RedisModuleString *LoadString(RedisModuleIO *io);
	char *LoadStringBuffer(RedisModuleIO *io, size_t *lenptr);
	void SaveDouble(RedisModuleIO *io, double value);
	double LoadDouble(RedisModuleIO *io);
	void SaveFloat(RedisModuleIO *io, float value);
	float LoadFloat(RedisModuleIO *io);
	void LogIOError(RedisModuleIO *io, const char *levelstr, const char *fmt, ...);

	const char *CallReplyStringPtr(RedisModuleCallReply *reply, size_t *len);
	RedisModuleString *CreateStringFromCallReply(RedisModuleCallReply *reply);
	const char *CallReplyProto(RedisModuleCallReply *reply, size_t *len);
	void FreeCallReply(RedisModuleCallReply *reply);
	int CallReplyType(RedisModuleCallReply *reply);
	long long CallReplyInteger(RedisModuleCallReply *reply);
	size_t CallReplyLength(RedisModuleCallReply *reply);
	RedisModuleCallReply *CallReplyArrayElement(RedisModuleCallReply *reply, size_t idx);

	RedisModuleCtx *GetThreadSafeContext(RedisModuleBlockedClient *bc);
	int UnblockClient(RedisModuleBlockedClient *bc, void *privdata);
	int AbortBlock(RedisModuleBlockedClient *bc);

	int CreateCommand(RedisModuleCtx *ctx, const char *name, RedisModuleCmdFunc cmdfunc, const char *strflags, int firstkey, int lastkey, int keystep);
	int SetModuleAttribs(RedisModuleCtx *ctx, const char *name, int ver, int apiver);
	int WrongArity(RedisModuleCtx *ctx);
	int ReplyWithLongLong(RedisModuleCtx *ctx, long long ll);
	int GetSelectedDb(RedisModuleCtx *ctx);
	int SelectDb(RedisModuleCtx *ctx, int newid);
	void *OpenKey(RedisModuleCtx *ctx, RedisModuleString *keyname, int mode);
	RedisModuleCallReply *Call(RedisModuleCtx *ctx, const char *cmdname, const char *fmt, ...);
	RedisModuleString *CreateString(RedisModuleCtx *ctx, const char *ptr, size_t len);
	RedisModuleString *CreateStringFromLongLong(RedisModuleCtx *ctx, long long ll);
	RedisModuleString *CreateStringFromString(RedisModuleCtx *ctx, const RedisModuleString *str);
	RedisModuleString *CreateStringPrintf(RedisModuleCtx *ctx, const char *fmt, ...);
	void FreeString(RedisModuleCtx *ctx, RedisModuleString *str);
	int ReplyWithError(RedisModuleCtx *ctx, const char *err);
	int ReplyWithSimpleString(RedisModuleCtx *ctx, const char *msg);
	int ReplyWithArray(RedisModuleCtx *ctx, long len);
	void ReplySetArrayLength(RedisModuleCtx *ctx, long len);
	int ReplyWithStringBuffer(RedisModuleCtx *ctx, const char *buf, size_t len);
	int ReplyWithString(RedisModuleCtx *ctx, RedisModuleString *str);
	int ReplyWithNull(RedisModuleCtx *ctx);
	int ReplyWithDouble(RedisModuleCtx *ctx, double d);
	int ReplyWithCallReply(RedisModuleCtx *ctx, RedisModuleCallReply *reply);
	void AutoMemory(RedisModuleCtx *ctx);
	int Replicate(RedisModuleCtx *ctx, const char *cmdname, const char *fmt, ...);
	int ReplicateVerbatim(RedisModuleCtx *ctx);
	int IsKeysPositionRequest(RedisModuleCtx *ctx);
	void KeyAtPos(RedisModuleCtx *ctx, int pos);
	unsigned long long GetClientId(RedisModuleCtx *ctx);
	void *PoolAlloc(RedisModuleCtx *ctx, size_t bytes);
	RedisModuleType *CreateDataType(RedisModuleCtx *ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods);
	void Log(RedisModuleCtx *ctx, const char *level, const char *fmt, ...);
	int StringAppendBuffer(RedisModuleCtx *ctx, RedisModuleString *str, const char *buf, size_t len);
	void RetainString(RedisModuleCtx *ctx, RedisModuleString *str);
	RedisModuleBlockedClient *BlockClient(RedisModuleCtx *ctx, RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback, void (*free_privdata)(void*), long long timeout_ms);
	int IsBlockedReplyRequest(RedisModuleCtx *ctx);
	int IsBlockedTimeoutRequest(RedisModuleCtx *ctx);
	void *GetBlockedClientPrivateData(RedisModuleCtx *ctx);
	void FreeThreadSafeContext(RedisModuleCtx *ctx);
	void ThreadSafeContextLock(RedisModuleCtx *ctx);
	void ThreadSafeContextUnlock(RedisModuleCtx *ctx);
};

///////////////////////////////////////////////////////////////////////////////////////////////

struct Context {
	RedisModuleCtx *ctx;
	
	int CreateCommand(const char *name, RedisModuleCmdFunc cmdfunc, const char *strflags, int firstkey, int lastkey, int keystep);
	int SetModuleAttribs(const char *name, int ver, int apiver);
	int WrongArity();

	int GetSelectedDb();
	int SelectDb(int newid);
	
	void *OpenKey(RedisModuleString *keyname, int mode);
	
	RedisModuleCallReply *Call(const char *cmdname, const char *fmt, ...);
	
	RedisModuleString *CreateString(const char *ptr, size_t len);
	RedisModuleString *CreateStringFromLongLong(long long ll);
	RedisModuleString *CreateStringFromString(const RedisModuleString *str);
	RedisModuleString *CreateStringPrintf(const char *fmt, ...);
	int StringAppendBuffer(RedisModuleString *str, const char *buf, size_t len);
	void RetainString(RedisModuleString *str);
	void FreeString(RedisModuleString *str);

	int ReplyWithLongLong(long long ll);
	int ReplyWithError(const char *err);
	int ReplyWithSimpleString(const char *msg);
	int ReplyWithArray(long len);
	void ReplySetArrayLength(long len);
	int ReplyWithStringBuffer(const char *buf, size_t len);
	int ReplyWithString(RedisModuleString *str);
	int ReplyWithNull();
	int ReplyWithDouble(double d);
	int ReplyWithCallReply(RedisModuleCallReply *reply);

	void AutoMemory();
	void *PoolAlloc(size_t bytes);

	int Replicate(const char *cmdname, const char *fmt, ...);
	int ReplicateVerbatim();
	int IsKeysPositionRequest();
	void KeyAtPos(int pos);
	unsigned long long GetClientId();
	RedisModuleType *CreateDataType(const char *name, int encver, RedisModuleTypeMethods *typemethods);
	void Log(const char *level, const char *fmt, ...);
	RedisModuleBlockedClient *BlockClient(RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback, void (*free_privdata)(void*), long long timeout_ms);
	int IsBlockedReplyRequest();
	int IsBlockedTimeoutRequest();
	void *GetBlockedClientPrivateData();
	
	void FreeThreadSafeContext();
	void ThreadSafeContextLock();
	void ThreadSafeContextUnlock();
};

//---------------------------------------------------------------------------------------------

struct SafeContext {
	RedisModuleCtx *ctx;
};

//---------------------------------------------------------------------------------------------

enum KEYTYPE {
	EMPTY,
	STRING,
	LIST,
	HASH,
	SET,
	ZSET,
	MODULE,
	STREAM
};
class RMType {
public:
	RMType(KEYTYPE _type = EMPTY) : m_type{_type} {}

	KEYTYPE GetType() const { return m_type; }
	void SetType(KEYTYPE _type) { m_type = _type; }
	operator KEYTYPE() const { return m_type; }

private:
	enum KEYTYPE m_type;
};

//---------------------------------------------------------------------------------------------

class String : RMType {
public:
    String(Context &ctx, const char *ptr, size_t len)
        : _str(RedisModule_CreateString(ctx, ptr, len))
        /*, _ctx(ctx) */ 
    { }

    String(RedisModuleString *rm_str)
        : _str(rm_str)
    { }

    ~String() { RedisModule_FreeString(/* _ctx ?? */, _str); }
	
	const char *PtrLen(size_t &len) const {
        return RedisModule_StringPtrLen(_str, &len);
    }

	int ToLongLong(long long &ll) const;
	int ToDouble(double &d) const;
    int ToLongDouble(long double &ld) const;
	
    int AppendBuffer(const char *buf, size_t len) {
        return RedisModule_StringAppendBuffer(/* _ctx ?? */, _str, buf, len);
    }

	const char *CallReplyStringPtr(RedisModuleCallReply *reply, size_t *len);
	String CreateStringFromCallReply(RedisModuleCallReply *reply);
	
	operator RedisModuleString *() { return _str; }
	operator const RedisModuleString *() const { return _str; }

private:
	RedisModuleString *_str;
    // Context &_ctx;
};

int StringCompare(String &s1, String &s2) {
	return RedisModule_StringCompare(s1, s2);
}

//---------------------------------------------------------------------------------------------

class Key {
public:
	Key(Context &ctx, String &keyname, int mode) // OpenKey
		: _key((RedisModuleKey *)RedisModule_OpenKey(ctx, keyname, mode))
	{ }
	~Key(); // CloseKey
	int DeleteKey();
	
	int Type();
	
	size_t ValueLength();
	
	mstime_t GetExpire();
	int SetExpire(mstime_t expire);
	
	RMType& GetType();
	const RMType& GetType() const;

	void *GetValue() { return RedisModule_ModuleTypeGetValue(_key); }
	int SetValue(RedisModuleType *mt, void *value) { return RedisModule_ModuleTypeSetValue(_key, mt, value); }

	operator RedisModuleKey *() { return _key; }
	operator const RedisModuleKey *() const { return _key; }

protected:
    RedisModuleKey *_key;
};

class StringKey : Key {
	int Set(String &other) {
        return RedisModule_StringSet(_key, other);
    }
	char *DMA(size_t &len, int mode) { // direct memory access
        return RedisModule_StringDMA(_key, &len, mode);
    }
	int Truncate(size_t newlen) { 
        return RedisModule_StringTruncate(_key, newlen);
    }
}

//---------------------------------------------------------------------------------------------

struct Zset : Type {
	RedisModuleKey *key;
	
	int add(double score, RedisModuleString *ele, int *flagsptr);
	int incrby(double score, RedisModuleString *ele, int *flagsptr, double *newscore);
	int score(RedisModuleString *ele, double *score);
	int rem(RedisModuleString *ele, int *deleted);
	void rangeStop();

	int firstInScoreRange(double min, double max, int minex, int maxex);
	int lastInScoreRange(double min, double max, int minex, int maxex);
	int firstInLexRange(RedisModuleString *min, RedisModuleString *max);
	int lastInLexRange(RedisModuleString *min, RedisModuleString *max);
	RedisModuleString *rangeCurrentElement(double *score);
	int rangeNext();
	int rangePrev();
	int rangeEndReached();
};

//---------------------------------------------------------------------------------------------

struct List : Type {
	RedisModuleKey *key;
	
	int ListPush(int where, RedisModuleString *ele);
	RedisModuleString *ListPop(int where);
};

	
//---------------------------------------------------------------------------------------------

struct Hash : Type {
	RedisModuleKey *key;
	
	int HashSet(int flags, ...);
	int HashGet(int flags, ...);
};

//---------------------------------------------------------------------------------------------

struct IO {
	RedisModuleIO *io;
	
	RedisModuleCtx *GetContextFromIO();

	void SaveUnsigned(uint64_t value);
	uint64_t LoadUnsigned();

	void SaveSigned(int64_t value);
	int64_t LoadSigned();
	
	void SaveString(RedisModuleString *s);
	RedisModuleString *LoadString();

	void SaveStringBuffer(const char *str, size_t len);
	char *LoadStringBuffer(size_t &len);
	
	void SaveDouble(double value);
	double LoadDouble();
	
	void SaveFloat(float value);
	float LoadFloat();
	
	void EmitAOF(const char *cmdname, const char *fmt, ...);

	void LogIOError(const char *levelstr, const char *fmt, ...);
};

//---------------------------------------------------------------------------------------------

struct CallReply {
	RedisModuleCallReply *reply;
	
	const char *CallReplyStringPtr(size_t &len);
	RedisModuleString *CreateStringFromCallReply();
	const char *CallReplyProto(size_t *len);
	void FreeCallReply();
	int CallReplyType();
	long long CallReplyInteger();
	size_t CallReplyLength();
	RedisModuleCallReply *CallReplyArrayElement(size_t idx);
};

//---------------------------------------------------------------------------------------------

struct BlockedClient {
	RedisModuleBlockedClient *bc;
	
	RedisModuleCtx *GetThreadSafeContext();
	int UnblockClient(void *privdata);
	int AbortBlock();
};

//---------------------------------------------------------------------------------------------

struct Args {
	int argc;
	RedisModuleString **argv;
};

//---------------------------------------------------------------------------------------------

template <class T>
struct Command {
	Command() {}
	virtual int Run(const Args &args) { return REDISMODULE_OK; }
	
	static int cmdfunc(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
		Args args(argv, argc);
		T cmd{ctx}
		return cmd.Run(args);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
