#pragma once

#include <string>
#include "redismodule.h"

namespace RedisModule {

///////////////////////////////////////////////////////////////////////////////////////////////

namespace Alloc {
	void *Alloc(size_t bytes) {
		return RedisModule_Alloc(bytes);
	}
	void *Calloc(size_t nmemb, size_t size) {
		return RedisModule_Calloc(nmemb, size);
	}
	void *Realloc(void *ptr, size_t bytes) {
		return RedisModule_Realloc(ptr, bytes);
	}
	void Free(void *ptr) {
		RedisModule_Free(ptr);
	}
	char *Strdup(const char *str) {
		return RedisModule_Strdup(str);
	}
	void *PoolAlloc(Context &ctx, size_t bytes) {
		return RedisModule_PoolAlloc(ctx, bytes);
	}
} // namespace Alloc

struct Module {

	int GetApi(const char *, void *);
	long long Milliseconds(void);

	void CloseKey(RedisModuleKey *kp);
	int KeyType(RedisModuleKey *kp);
	size_t ValueLength(RedisModuleKey *kp);
	int DeleteKey(RedisModuleKey *key);

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

class RMType {
public:
	RMType(Context &ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods)
		: _type(RedisModule_CreateDataType(ctx, name, encver, typemethods))
	{ }
	RMType(RedisModuleType *type) : _type(type) {}

	operator RedisModuleType *() { return _type; }
	operator const RedisModuleType *() const { return _type; }

private:
	RedisModuleType *_type;
};

//---------------------------------------------------------------------------------------------

class String {
public:
    String(Context &ctx, const char *ptr, size_t len)
        : _str(RedisModule_CreateString(ctx, ptr, len))
        /*, _ctx(ctx) */ 
    { }

    String(RedisModuleString *rm_str)
        : _str(rm_str)
    { }

    ~String() {
		RedisModule_FreeString(/* _ctx ?? */, _str);
	}
	
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
	Key(RedisModuleKey *key) : _key(key) { }

	~Key() { // CloseKey
		RedisModule_CloseKey(_key);
	}
	int DeleteKey() {
		return RedisModule_DeleteKey(_key);
	}
	
	int Type() {
		return RedisModule_KeyType(_key);
	}
	
	size_t ValueLength() {
		return RedisModule_ValueLength(_key);
	}
	
	mstime_t GetExpire() {
		return RedisModule_GetExpire(_key);
	}
	int SetExpire(mstime_t expire){
		return RedisModule_SetExpire(_key, expire);
	}	

	RMType GetType() {
		return RedisModule_ModuleTypeGetType(_key);
	}

	void *GetValue() {
		return RedisModule_ModuleTypeGetValue(_key);
	}
	int SetValue(RMType &mt, void *value) {
		return RedisModule_ModuleTypeSetValue(_key, mt, value);
	}

	operator RedisModuleKey *() { return _key; }
	operator const RedisModuleKey *() const { return _key; }

protected:
    RedisModuleKey *_key;
};

class StringKey : Key {
	StringKey(Context &ctx, String &keyname, int mode) : Key(ctx, keyname, mode) {}

	int Set(String &other) {
        return RedisModule_StringSet(_key, other);
    }
	char *DMA(size_t &len, int mode) { // direct memory access
        return RedisModule_StringDMA(_key, &len, mode);
    }
	int Truncate(size_t newlen) { 
        return RedisModule_StringTruncate(_key, newlen);
    }
};

//---------------------------------------------------------------------------------------------

struct ListKey : Key {
	ListKey(Context &ctx, String &keyname, int mode) : Key(ctx, keyname, mode) {}

	int Push(int where, String &ele) {
		return RedisModule_ListPush(_key, where, ele);
	}
	RedisModuleString *Pop(int where) {
		return RedisModule_ListPop(_key, where);
	}
};

//---------------------------------------------------------------------------------------------

struct ZsetKey : Key {
	ZsetKey(Context &ctx, String &keyname, int mode) : Key(ctx, keyname, mode) {}
	
	int Add(double score, String &ele, int *flagsptr) {
		return RedisModule_ZsetAdd(_key, score, ele, flagsptr);
	}
	int Incrby(double score, String &ele, int *flagsptr, double *newscore) {
		return RedisModule_ZsetIncrby(_key, score, ele, flagsptr, newscore);
	}
	int Rem(String &ele, int *deleted) {
		return RedisModule_ZsetRem(_key, ele, deleted);
	}
	int Score(String &ele, double *score) {
		return RedisModule_ZsetScore(_key, ele, score);
	}

	void RangeStop() {
		RedisModule_ZsetRangeStop(_key);
	}
	int RangeEndReached(){
		return RedisModule_ZsetRangeEndReached(_key);
	}
	int FirstInScoreRange(double min, double max, int minex, int maxex) {
		return RedisModule_ZsetFirstInScoreRange(_key, min, max, minex, maxex);
	}
	int LastInScoreRange(double min, double max, int minex, int maxex) {
		return RedisModule_ZsetLastInScoreRange(_key, min, max, minex, maxex);
	}
	int FirstInLexRange(String &min, String &max) {
		return RedisModule_ZsetFirstInLexRange(_key, min, max);
	}
	int LastInLexRange(String &min, String &max) {
		return RedisModule_ZsetLastInLexRange(_key, min, max);
	}
	RedisModuleString *RangeCurrentElement(double *score) {
		return RedisModule_ZsetRangeCurrentElement(_key, score);
	}
	int RangeNext() {
		return RedisModule_ZsetRangeNext(_key);
	}
	int RangePrev() {
		return RedisModule_ZsetRangePrev(_key);
	}
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
