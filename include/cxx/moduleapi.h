#pragma once

#include "redismodule.h"
#include <vector>

namespace RedisModule {

///////////////////////////////////////////////////////////////////////////////////////////////

namespace Alloc {
void *Alloc(size_t bytes);
void *Calloc(size_t nmemb, size_t size);
void *Realloc(void *ptr, size_t bytes);
void Free(void *ptr);
char *Strdup(const char *str);
void *PoolAlloc(Context ctx, size_t bytes);
} // namespace Alloc

namespace Time {
long long Milliseconds();
}

///////////////////////////////////////////////////////////////////////////////////////////////

class Context {
public:
	void AutoMemory() noexcept;

	int CreateCommand(const char *name, RedisModuleCmdFunc cmdfunc,
		const char *strflags, int firstkey, int lastkey, int keystep);

	bool IsKeysPositionRequest() noexcept;
	void KeyAtPos(int pos) noexcept;

	int GetSelectedDb() noexcept;
	void SelectDb(int newid);

	unsigned long long GetClientId() noexcept;

	template<typename... Vargs>
	void Log(const char *level, const char *fmt, Vargs... vargs) noexcept;

	template<typename... Vargs>
	void Replicate(const char *cmdname, const char *fmt, Vargs... vargs);
	void ReplicateVerbatim() noexcept;

	/* only relevant ifdef REDISMODULE_EXPERIMENTAL_API
	int IsBlockedReplyRequest();
	int IsBlockedTimeoutRequest();
	void *GetBlockedClientPrivateData();
	*/
	Context(RedisModuleCtx *ctx);

	operator RedisModuleCtx *() noexcept;
	operator const RedisModuleCtx *() const noexcept;

private:
	RedisModuleCtx *_ctx;
};

//---------------------------------------------------------------------------------------------
/* only relevant ifdef REDISMODULE_EXPERIMENTAL_API
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
*/
//---------------------------------------------------------------------------------------------
/* only relevant ifdef REDISMODULE_EXPERIMENTAL_API
class ThreadSafeContext : Context {
public:
	ThreadSafeContext(BlockedClient bc);
	~ThreadSafeContext();

	void Lock();
	int TryLock();
	void Unlock();
};
*/
//---------------------------------------------------------------------------------------------

class RMType {
public:
	RMType(Context ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods);
	RMType(RedisModuleType *type);

	operator RedisModuleType *() noexcept;
	operator const RedisModuleType *() const noexcept;

private:
	RedisModuleType *_type;
};

//---------------------------------------------------------------------------------------------

class String {
public:
    String(const char *ptr, size_t len);
    String(long long ll);
    String(const RedisModuleString *str);

	String(const String& other) = delete;
	String(String&& other) = default;
	String& operator=(const String&) = delete;
	String& operator=(String&&) = delete;
    ~String();

	void Retain();

	const char *PtrLen(size_t &len) const noexcept;

	long long ToLongLong() const;
	double ToDouble() const;
    long double ToLongDouble() const;
	
    void AppendBuffer(const char *buf, size_t len);

	operator RedisModuleString *() noexcept;
	operator const RedisModuleString *() const noexcept;

private:
	RedisModuleString *_str;
};

int StringCompare(String& s1, String& s2) noexcept;

//---------------------------------------------------------------------------------------------

class Key {
public:
	Key(Context ctx, String& keyname, int mode);
	Key(RedisModuleKey *key);

	Key(const Key&) = delete;
	Key(Key&&) = delete;
	Key& operator=(const Key&) = delete;
	Key& operator=(Key&&) = delete;

	~Key() noexcept;
	int DeleteKey();
	
	size_t ValueLength() noexcept;
	
	mstime_t GetExpire() noexcept;
	int SetExpire(mstime_t expire);

	int Type() noexcept;
	RMType GetType() noexcept;

	void *GetValue() noexcept;
	int SetValue(RMType mt, void *value);

	operator RedisModuleKey *() noexcept;
	operator const RedisModuleKey *() const noexcept;

protected:
    RedisModuleKey *_key;
};

//---------------------------------------------------------------------------------------------

class StringKey : Key {
	StringKey(Context ctx, String& keyname, int mode);

	int Set(String& str);
	char *DMA(size_t &len, int mode); // direct memory access
	int Truncate(size_t newlen);
};

//---------------------------------------------------------------------------------------------

struct List : Key {
	List(Context ctx, String& keyname, int mode);

	int Push(int where, String& ele);
	String Pop(int where);
};

//---------------------------------------------------------------------------------------------

class Zset : Key {
public:
	Zset(Context ctx, String& keyname, int mode);

	int Add(double score, String& ele, int *flagsptr);
	int Incrby(double score, String& ele, int *flagsptr, double *newscore);
	int Rem(String& ele, int *deleted);
	int Score(String& ele, double *score);

	void RangeStop();
	int RangeEndReached();
	int FirstInScoreRange(double min, double max, int minex, int maxex);
	int LastInScoreRange(double min, double max, int minex, int maxex);
	int FirstInLexRange(String& min, String& max);
	int LastInLexRange(String& min, String& max);
	String RangeCurrentElement(double *score);
	int RangeNext();
	int RangePrev();
};

//---------------------------------------------------------------------------------------------

class Hash : Key {
public:
	Hash(Context ctx, String& keyname, int mode);

	template<typename... Vargs>
	int Set(int flags, Vargs... vargs);
	template<typename... Vargs>
	int Get(int flags, Vargs... vargs);
};

//---------------------------------------------------------------------------------------------

class IO {
public:
	Context GetContext();

	void SaveUnsigned(uint64_t value);
	uint64_t LoadUnsigned();

	void SaveSigned(int64_t value);
	int64_t LoadSigned();
	
	void SaveString(String& s);
	String LoadString();

	void SaveStringBuffer(const char *str, size_t len);
	char *LoadStringBuffer(size_t &len);
	
	void SaveDouble(double value);
	double LoadDouble();
	
	void SaveFloat(float value);
	float LoadFloat();
	
	template<typename... Vargs>
	void EmitAOF(const char *cmdname, const char *fmt, Vargs... vargs);

	template<typename... Vargs>
	void LogIOError(const char *levelstr, const char *fmt, Vargs... vargs);
private:
	RedisModuleIO *_io;
};

//---------------------------------------------------------------------------------------------

class CallReply {
public:
	template<typename... Vargs>
	CallReply(const char *cmdname, const char *fmt, Vargs... vargs);
	CallReply(RedisModuleCallReply *reply);
	~CallReply();
	const char *StringPtr(size_t &len);
	String CreateString();

	const char *Proto(size_t &len);
	int Type();
	long long Integer();
	size_t Length();
	CallReply ArrayElement(size_t idx);

	operator RedisModuleCallReply *();
private:
	RedisModuleCallReply *_reply;
};

//---------------------------------------------------------------------------------------------

class Args {
public:
	Args(int argc, RedisModuleString **argv);

private:
	std::vector<String> _args;
};

//---------------------------------------------------------------------------------------------

template <class T>
struct Command {
	Command();
	virtual int Run(const Args& args);
	
	static int cmdfunc(Context ctx, Args& args);
};

//---------------------------------------------------------------------------------------------

namespace Reply {
int WrongArity(Context ctx);
int LongLong(Context ctx, long long ll);
int Error(Context ctx, const char *err);
int SimpleString(Context ctx, const char *msg);
int Array(Context ctx, long len);
void SetArrayLength(Context ctx, long len);
int StringBuffer(Context ctx, const char *buf, size_t len);
int String(Context ctx, RedisModule::String& str);
int Null(Context ctx);
int Double(Context ctx, double d);
int CallReply(Context ctx, RedisModule::CallReply reply);
}

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
