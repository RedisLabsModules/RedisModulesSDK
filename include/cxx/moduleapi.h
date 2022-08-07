#pragma once

#include "redismodule.h"

namespace RedisModule {

///////////////////////////////////////////////////////////////////////////////////////////////

namespace Alloc {
void *Alloc(size_t bytes);
void *Calloc(size_t nmemb, size_t size);
void *Realloc(void *ptr, size_t bytes);
void Free(void *ptr);
char *Strdup(const char *str);
void *PoolAlloc(size_t bytes);
} // namespace Alloc

namespace Time {
long long Milliseconds();
}

namespace AutoMemory {
void AutoMemory();
}

struct Module {
	/*
	int GetApi(const char *, void *);

	const char *CallReplyProto(RedisModuleCallReply *reply, size_t *len);
	void FreeCallReply(RedisModuleCallReply *reply);
	int CallReplyType(RedisModuleCallReply *reply);
	long long CallReplyInteger(RedisModuleCallReply *reply);
	size_t CallReplyLength(RedisModuleCallReply *reply);
	RedisModuleCallReply *CallReplyArrayElement(RedisModuleCallReply *reply, size_t idx);

	int SetModuleAttribs(RedisModuleCtx *ctx, const char *name, int ver, int apiver);
	int WrongArity(RedisModuleCtx *ctx);
	int ReplyWithLongLong(RedisModuleCtx *ctx, long long ll);
	int GetSelectedDb(RedisModuleCtx *ctx);
	int SelectDb(RedisModuleCtx *ctx, int newid);
	int ReplyWithError(RedisModuleCtx *ctx, const char *err);
	int ReplyWithSimpleString(RedisModuleCtx *ctx, const char *msg);
	int ReplyWithArray(RedisModuleCtx *ctx, long len);
	void ReplySetArrayLength(RedisModuleCtx *ctx, long len);
	int ReplyWithStringBuffer(RedisModuleCtx *ctx, const char *buf, size_t len);
	int ReplyWithString(RedisModuleCtx *ctx, RedisModuleString *str);
	int ReplyWithNull(RedisModuleCtx *ctx);
	int ReplyWithDouble(RedisModuleCtx *ctx, double d);
	int ReplyWithCallReply(RedisModuleCtx *ctx, RedisModuleCallReply *reply);
	int Replicate(RedisModuleCtx *ctx, const char *cmdname, const char *fmt, ...);
	int ReplicateVerbatim(RedisModuleCtx *ctx);
	unsigned long long GetClientId(RedisModuleCtx *ctx);
	RedisModuleType *CreateDataType(RedisModuleCtx *ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods);
	void Log(RedisModuleCtx *ctx, const char *level, const char *fmt, ...);
	int StringAppendBuffer(RedisModuleCtx *ctx, RedisModuleString *str, const char *buf, size_t len);
	int IsBlockedReplyRequest(RedisModuleCtx *ctx);
	int IsBlockedTimeoutRequest(RedisModuleCtx *ctx);
	void *GetBlockedClientPrivateData(RedisModuleCtx *ctx);
	*/
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Context {
public:
	int IsKeysPositionRequest();
	void KeyAtPos(int pos);
	int CreateCommand(const char *name, RedisModuleCmdFunc cmdfunc,
		const char *strflags, int firstkey, int lastkey, int keystep);

	/*
	int SetModuleAttribs(const char *name, int ver, int apiver);
	int WrongArity();

	int GetSelectedDb();
	int SelectDb(int newid);
	
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

	int Replicate(const char *cmdname, const char *fmt, ...);
	int ReplicateVerbatim();
	unsigned long long GetClientId();
	void Log(const char *level, const char *fmt, ...);
	RedisModuleBlockedClient *BlockClient(RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback, void (*free_privdata)(void*), long long timeout_ms);
	int IsBlockedReplyRequest();
	int IsBlockedTimeoutRequest();
	void *GetBlockedClientPrivateData();
	*/

	operator RedisModuleCtx *();
	operator const RedisModuleCtx *() const;
protected:
	RedisModuleCtx *_ctx;
};

//---------------------------------------------------------------------------------------------

class ThreadSafeContext : Context {
public:
	ThreadSafeContext(BlockedClient bc);
	~ThreadSafeContext();

	void Lock();
	int TryLock();
	void Unlock();
};

//---------------------------------------------------------------------------------------------

class BlockedClient {
public:
	BlockedClient(Context ctx, RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback,
		void (*free_privdata)(RedisModuleCtx *, void*), long long timeout_ms);
	int UnblockClient(void *privdata);
	int AbortBlock();

	operator RedisModuleBlockedClient *();
private:
	RedisModuleBlockedClient *_bc;
};

//---------------------------------------------------------------------------------------------

class RMType {
public:
	RMType(const char *name, int encver, RedisModuleTypeMethods *typemethods);
	RMType(RedisModuleType *type);

	operator RedisModuleType *();
	operator const RedisModuleType *() const;

private:
	RedisModuleType *_type;
};

//---------------------------------------------------------------------------------------------

class String {
public:
    String(const char *ptr, size_t len);

    String(long long ll);

    String(const RedisModuleString *str);

	String(const String&) = delete;
	String(String&&) = delete;
	String& operator=(const String&) = delete;
	String& operator=(String&&) = delete;

	void Retain();

    ~String();
	
	const char *PtrLen(size_t &len) const;

	int ToLongLong(long long &ll) const;
	int ToDouble(double &d) const;
    int ToLongDouble(long double &ld) const;
	
    int AppendBuffer(const char *buf, size_t len);
	
	operator RedisModuleString *();
	operator const RedisModuleString *() const;

private:
	RedisModuleString *_str;
};

int StringCompare(String s1, String s2);

//---------------------------------------------------------------------------------------------

class Key {
public:
	Key(String keyname, int mode);
	Key(RedisModuleKey *key);

	Key(const Key&) = delete;
	Key(Key&&) = delete;
	Key& operator=(const Key&) = delete;
	Key& operator=(Key&&) = delete;

	~Key();
	int DeleteKey();
	
	int Type();
	
	size_t ValueLength();
	
	mstime_t GetExpire();
	int SetExpire(mstime_t expire);

	RMType GetType();

	void *GetValue();
	int SetValue(RMType mt, void *value);

	operator RedisModuleKey *();
	operator const RedisModuleKey *() const;

protected:
    RedisModuleKey *_key;
};

//---------------------------------------------------------------------------------------------

class StringKey : Key {
	StringKey(String keyname, int mode);

	int Set(String str);
	char *DMA(size_t &len, int mode); // direct memory access
	int Truncate(size_t newlen);
};

//---------------------------------------------------------------------------------------------

struct List : Key {
	List(String keyname, int mode);

	int Push(int where, String ele);
	RedisModuleString *Pop(int where);
};

//---------------------------------------------------------------------------------------------

class Zset : Key {
public:
	Zset(String keyname, int mode);

	int Add(double score, String ele, int *flagsptr);
	int Incrby(double score, String ele, int *flagsptr, double *newscore);
	int Rem(String ele, int *deleted);
	int Score(String ele, double *score);

	void RangeStop();
	int RangeEndReached();
	int FirstInScoreRange(double min, double max, int minex, int maxex);
	int LastInScoreRange(double min, double max, int minex, int maxex);
	int FirstInLexRange(String min, String max);
	int LastInLexRange(String min, String max);
	RedisModuleString *RangeCurrentElement(double *score);
	int RangeNext();
	int RangePrev();
};

//---------------------------------------------------------------------------------------------

class Hash : Key {
public:
	Hash(String keyname, int mode);

	int Set(int flags, ...);
	int Get(int flags, ...);
};

//---------------------------------------------------------------------------------------------

class IO {
public:
	Context GetContext();

	void SaveUnsigned(uint64_t value);
	uint64_t LoadUnsigned();

	void SaveSigned(int64_t value);
	int64_t LoadSigned();
	
	void SaveString(String s);
	String LoadString();

	void SaveStringBuffer(const char *str, size_t len);
	char *LoadStringBuffer(size_t &len);
	
	void SaveDouble(double value);
	double LoadDouble();
	
	void SaveFloat(float value);
	float LoadFloat();
	
	void EmitAOF(const char *cmdname, const char *fmt, ...);

	void LogIOError(const char *levelstr, const char *fmt, ...);
private:
	RedisModuleIO *_io;
};

//---------------------------------------------------------------------------------------------

class CallReply {
public:
	CallReply(const char *cmdname, const char *fmt, ...);
	~CallReply();
	const char *StringPtr(size_t &len);
	String CreateString();

	const char *Proto(size_t *len);
	int Type();
	long long Integer();
	size_t Length();
	CallReply ArrayElement(size_t idx);
private:
	RedisModuleCallReply *_reply;
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
	virtual int Run(const Args &args);
	
	static int cmdfunc(RedisModuleCtx *ctx, RedisModuleString **argv, int argc);
};

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
