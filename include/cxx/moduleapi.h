#pragma once

#include "redismodule.h"
#include <vector>

namespace RedisModule {

///////////////////////////////////////////////////////////////////////////////////////////////

void * (*RedisModule_TryAlloc)(size_t bytes);

RedisModuleCommand *(*GetCommand)(RedisModuleCtx *ctx, const char *name);
int (*CreateSubcommand)(RedisModuleCommand *parent, const char *name, RedisModuleCmdFunc cmdfunc, const char *strflags, int firstkey, int lastkey, int keystep);
int (*SetCommandInfo)(RedisModuleCommand *command, const RedisModuleCommandInfo *info);

int (*KeyExists)(RedisModuleCtx *ctx, RedisModuleString *keyname);

RedisModuleString * (*ListGet)(RedisModuleKey *key, long index);
int (*ListSet)(RedisModuleKey *key, long index, RedisModuleString *value);
int (*ListInsert)(RedisModuleKey *key, long index, RedisModuleString *value);
int (*ListDelete)(RedisModuleKey *key, long index);

double (*CallReplyDouble)(RedisModuleCallReply *reply);
int (*CallReplyBool)(RedisModuleCallReply *reply);
const char* (*CallReplyBigNumber)(RedisModuleCallReply *reply, size_t *len);
const char* (*CallReplyVerbatim)(RedisModuleCallReply *reply, size_t *len, const char **format);
RedisModuleCallReply * (*CallReplySetElement)(RedisModuleCallReply *reply, size_t idx);
int (*CallReplyMapElement)(RedisModuleCallReply *reply, size_t idx, RedisModuleCallReply **key, RedisModuleCallReply **val);
int (*CallReplyAttributeElement)(RedisModuleCallReply *reply, size_t idx, RedisModuleCallReply **key, RedisModuleCallReply **val);
RedisModuleCallReply * (*CallReplyAttribute)(RedisModuleCallReply *reply);

RedisModuleString * (*CreateStringFromULongLong)(RedisModuleCtx *ctx, unsigned long long ull);

int (*ReplyWithMap)(RedisModuleCtx *ctx, long len);
int (*ReplyWithSet)(RedisModuleCtx *ctx, long len);
int (*ReplyWithAttribute)(RedisModuleCtx *ctx, long len);

void (*ReplySetMapLength)(RedisModuleCtx *ctx, long len);
void (*ReplySetSetLength)(RedisModuleCtx *ctx, long len);
void (*ReplySetAttributeLength)(RedisModuleCtx *ctx, long len);
void (*ReplySetPushLength)(RedisModuleCtx *ctx, long len);

int (*ReplyWithVerbatimStringType)(RedisModuleCtx *ctx, const char *buf, size_t len, const char *ext);

int (*ReplyWithBool)(RedisModuleCtx *ctx, int b);

int (*ReplyWithDouble)(RedisModuleCtx *ctx, double d);
int (*ReplyWithBigNumber)(RedisModuleCtx *ctx, const char *bignum, size_t len);

int (*StringToULongLong)(const RedisModuleString *str, unsigned long long *ull);

void (*KeyAtPosWithFlags)(RedisModuleCtx *ctx, int pos, int flags);
int (*IsChannelsPositionRequest)(RedisModuleCtx *ctx);
void (*ChannelAtPosWithFlags)(RedisModuleCtx *ctx, int pos, int flags);

RedisModuleString * (*GetClientNameById)(RedisModuleCtx *ctx, uint64_t id);
int (*SetClientNameById)(uint64_t id, RedisModuleString *name);

int (*PublishMessageShard)(RedisModuleCtx *ctx, RedisModuleString *channel, RedisModuleString *message);

void * (*LoadDataTypeFromStringEncver)(const RedisModuleString *str, const RedisModuleType *mt, int encver);

void (*TrimStringAllocation)(RedisModuleString *str);

uint64_t (*MonotonicMicroseconds)(void);
ustime_t (*Microseconds)(void);
ustime_t (*CachedMicroseconds)(void);
void (*DigestAddStringBuffer)(RedisModuleDigest *md, const char *ele, size_t len);

int (*InfoAddSection)(RedisModuleInfoCtx *ctx, const char *name);
int (*InfoBeginDictField)(RedisModuleInfoCtx *ctx, const char *name);

int (*InfoAddFieldString)(RedisModuleInfoCtx *ctx, const char *field, RedisModuleString *value);
int (*InfoAddFieldCString)(RedisModuleInfoCtx *ctx, const char *field,const  char *value);
int (*InfoAddFieldDouble)(RedisModuleInfoCtx *ctx, const char *field, double value);
int (*InfoAddFieldLongLong)(RedisModuleInfoCtx *ctx, const char *field, long long value);
int (*InfoAddFieldULongLong)(RedisModuleInfoCtx *ctx, const char *field, unsigned long long value);

void (*Yield)(RedisModuleCtx *ctx, int flags, const char *busy_reply);

int (*SendClusterMessage)(RedisModuleCtx *ctx, const char *target_id, uint8_t type, const char *msg, uint32_t len);

RedisModuleString * (*CommandFilterArgGet)(RedisModuleCommandFilterCtx *fctx, int pos);

size_t (*MallocUsableSize)(void *ptr);
size_t (*MallocSizeString)(RedisModuleString* str);
size_t (*MallocSizeDict)(RedisModuleDict* dict);

RedisModuleString * (*GetCurrentUserName)(RedisModuleCtx *ctx);
RedisModuleUser * (*GetModuleUserFromUserName)(RedisModuleString *name);
int (*ACLCheckCommandPermissions)(RedisModuleUser *user, RedisModuleString **argv, int argc);
int (*ACLCheckKeyPermissions)(RedisModuleUser *user, RedisModuleString *key, int flags);
int (*ACLCheckChannelPermissions)(RedisModuleUser *user, RedisModuleString *ch, int literal);
void (*ACLAddLogEntry)(RedisModuleCtx *ctx, RedisModuleUser *user, RedisModuleString *object, RedisModuleACLLogEntryReason reason);

int (*RedactClientCommandArgument)(RedisModuleCtx *ctx, int pos);

int *(*GetCommandKeysWithFlags)(RedisModuleCtx *ctx, RedisModuleString **argv, int argc, int *num_keys, int **out_flags);

int (*EventLoopAdd)(int fd, int mask, RedisModuleEventLoopFunc func, void *user_data);
int (*EventLoopDel)(int fd, int mask);
int (*EventLoopAddOneShot)(RedisModuleEventLoopOneShotFunc func, void *user_data);
int (*RegisterBoolConfig)(RedisModuleCtx *ctx, const char *name, int default_val, unsigned int flags, RedisModuleConfigGetBoolFunc getfn, RedisModuleConfigSetBoolFunc setfn, RedisModuleConfigApplyFunc applyfn, void *privdata);
int (*RegisterNumericConfig)(RedisModuleCtx *ctx, const char *name, long long default_val, unsigned int flags, long long min, long long max, RedisModuleConfigGetNumericFunc getfn, RedisModuleConfigSetNumericFunc setfn, RedisModuleConfigApplyFunc applyfn, void *privdata);
int (*RegisterStringConfig)(RedisModuleCtx *ctx, const char *name, const char *default_val, unsigned int flags, RedisModuleConfigGetStringFunc getfn, RedisModuleConfigSetStringFunc setfn, RedisModuleConfigApplyFunc applyfn, void *privdata);
int (*RegisterEnumConfig)(RedisModuleCtx *ctx, const char *name, int default_val, unsigned int flags, const char **enum_values, const int *int_values, int num_enum_vals, RedisModuleConfigGetEnumFunc getfn, RedisModuleConfigSetEnumFunc setfn, RedisModuleConfigApplyFunc applyfn, void *privdata);
int (*LoadConfigs)(RedisModuleCtx *ctx);

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
mstime_t Milliseconds();
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

	int IsBlockedReplyRequest();
	int IsBlockedTimeoutRequest();
	void *GetBlockedClientPrivateData();
	Context(RedisModuleCtx *ctx);

	operator RedisModuleCtx *() noexcept;
	operator const RedisModuleCtx *() const noexcept;

private:
	RedisModuleCtx *_ctx;
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

class ThreadSafeContext : Context {
public:
	ThreadSafeContext(BlockedClient bc);
	~ThreadSafeContext();

	void Lock();
	int TryLock();
	void Unlock();
};

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
