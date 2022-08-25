#pragma once

#include <memory>     // std::unique_ptr
#include <string>     // std::string
#include <functional> // std::function
#include "redismodule.h"


namespace Redis {

///////////////////////////////////////////////////////////////////////////////////////////////

typedef ::RedisModuleInfoCtx* InfoContext; // class InfoContext

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
concept Unwrapable = requires(T t) {
	t.Unwrap();
};

auto Unwrap(Unwrapable auto t) -> decltype(t.Unwrap()) {
	return t.Unwrap();
}

template <class T>
T Unwrap(T t) {
	return t;
}

///////////////////////////////////////////////////////////////////////////////////////////////

class String {
public:
	// No Context. Used only for AutoMemory. To be deprecated.
    String(const char *ptr, size_t len);
	String(const std::string& str);
	String(std::string_view& str);
    String(long long ll);
	String(unsigned long long ull);
    String(RedisModuleString *str);
	String(const String& other);
	void Retain();
	String(String&& other);
	String& operator=(String other);
	void swap(String& other) noexcept;
	friend void swap(String& s1, String& s2) noexcept;
    ~String() noexcept;

	void Append(std::string_view& sv);
	void Trim() noexcept;
	size_t Length() const noexcept;

	int ToLongLong(long long& ll) const noexcept;
	long long ToLongLong() const;
	int ToDouble(double& d) const noexcept;
	double ToDouble() const;
    int ToLongDouble(long double& ld) const noexcept;
    long double ToLongDouble() const;
	int ToULongLong(unsigned long long& ull) const noexcept;
	unsigned long long ToULongLong() const;

	RedisModuleString *Unwrap() noexcept;
	operator RedisModuleString *() noexcept;
	operator const RedisModuleString *() const noexcept;

	operator std::string() const;
	operator std::string_view() const;

	auto operator==(const String& other) const;
	auto operator<=>(const String& other) const;
private:
	RedisModuleString *_str;
};

//---------------------------------------------------------------------------------------------

class Args {
public:
	Args(int argc, RedisModuleString **argv);
	int Size() const;
	operator RedisModuleString **() const;

	String operator[](int idx);
private:
	int _argc;
	RedisModuleString **_argv;
};

//---------------------------------------------------------------------------------------------

class CallReply {
public:
	class KVP {
	public:
		KVP(RedisModuleCallReply *key, RedisModuleCallReply *val);
		CallReply Key();
		CallReply Val();
	private:
		std::pair<RedisModuleCallReply*, RedisModuleCallReply*> _kvp;
	};

	int Type();
	size_t Length();

	long long Integer() noexcept;
	double Double() noexcept;
	const char *BigNumber(size_t& len);
	const char *Verbatim(size_t& len, const char **format);
	bool Bool() noexcept;
	const char *StringPtr(size_t& len);
	String CreateString();
	
	CallReply ArrayElement(size_t idx);
	CallReply SetElement(size_t idx);
	KVP MapElement(size_t idx);
	KVP AttributeElement(size_t idx);

	CallReply Attribute();
	const char *Protocol(size_t& len);

	RedisModuleCallReply *Unwrap() noexcept;
	operator RedisModuleCallReply *() noexcept;
private:
	friend class Context;
	friend class KVP;
	CallReply(RedisModuleCallReply *reply, bool owning = false);
	std::unique_ptr<RedisModuleCallReply, decltype(RedisModule_FreeCallReply)> _reply;
};

//---------------------------------------------------------------------------------------------

class BlockedClient {
public:
	// destruction of a BlockedClient may throw.
	// if throwing is a concern, call UnblockClient
	void UnblockClient(void *privdata);

	RedisModuleBlockedClient *Unwrap() noexcept;
	operator RedisModuleBlockedClient *();
private:
	friend class Context;
	BlockedClient(RedisModuleBlockedClient* bc);
	static void Deleter(RedisModuleBlockedClient *) noexcept;
	friend class std::unique_ptr<RedisModuleBlockedClient, decltype(&Deleter)>;
	std::unique_ptr<RedisModuleBlockedClient, decltype(&Deleter)> _bc;
};

//---------------------------------------------------------------------------------------------

class Context {
public:
	// No AutoMemory. To be deprecated.
	// void AutoMemory() noexcept;

	Context(RedisModuleCtx* ctx = NULL);

	template<typename... Vargs>
	CallReply Call(const char *cmdname, const char *fmt, Vargs... vargs);

	bool IsKeysPositionRequest() noexcept;
	void KeyAtPos(int pos) noexcept;
	void KeyAtPosWithFlags(int pos, int flags);
	bool KeyExists(const String& keyname);

	int *GetCommandKeysWithFlags(Args& args, int *num_keys, int **out_flags);

	int GetSelectedDb() noexcept;
	void SelectDb(int newid);

	bool IsChannelsPositionRequest();
	void ChannelAtPosWithFlags(int pos, int flags);

	void Yield(int flags, const char *busy_reply);
	int PublishMessageShard(String& channel, String& message);
	int SendClusterMessage(const char *target_id, uint8_t type, const char *msg, uint32_t len);

	template<typename... Vargs>
	void Replicate(const char *cmdname, const char *fmt, Vargs... vargs);
	void Replicate() noexcept;

	unsigned long long GetClientId() noexcept;
	static int SetClientNameById(uint64_t id, String& name);
	String GetClientNameById(uint64_t id);

	BlockedClient BlockClient(RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback,
		void (*free_privdata)(RedisModuleCtx *, void *), long long timeout_ms);
	bool IsBlockedReplyRequest();
	bool IsBlockedTimeoutRequest();
	void *GetBlockedClientPrivateData();

	template<typename... Vargs>
	void Log(const char *level, const char *fmt, Vargs... vargs) noexcept;

	int WrongArity();
	int ReplyWithError(const char *err);
	int ReplyWithNull();
	int ReplyWithArray(long len);
	void ReplySetArrayLength(long len);
	int ReplyWithMap(long len);
	void ReplySetMapLength(long len);
	int ReplyWithSet(long len);
	void ReplySetSetLength(long len);
	int ReplyWithAttribute(long len);
	void ReplySetAttributeLength(long len);
	int ReplyWithBool(bool b);
	int ReplyWithLongLong(long long ll);
	int ReplyWithDouble(double d);
	int ReplyWithBigNumber(const char *bignum, size_t len);
	int ReplyWithSimpleString(const char *msg);
	int ReplyWithStringBuffer(const char *buf, size_t len);
	int ReplyWithVerbatimStringType(const char *buf, size_t len, const char *ext);
	int ReplyWithString(String& str);
	int ReplyWithCallReply(CallReply& reply);

	RedisModuleCtx *Unwrap() noexcept;
	operator RedisModuleCtx*() noexcept;
private:
	RedisModuleCtx* _ctx;
};

//---------------------------------------------------------------------------------------------

template <class T>
struct Cmd {
	Context _ctx;
	Args _args;

	Cmd(Context ctx, const Args& args);

	virtual int operator()() = 0;
	
	static int cmdfunc(Context ctx, const Args& args);
};

//---------------------------------------------------------------------------------------------

class ThreadSafeContext {
public:
	ThreadSafeContext(BlockedClient bc);
	ThreadSafeContext(Context ctx);

	void Lock() noexcept;
	int TryLock() noexcept;
	void Unlock() noexcept;
private:
	std::unique_ptr<RedisModuleCtx, decltype(RedisModule_FreeThreadSafeContext)> _ctx;
};

//---------------------------------------------------------------------------------------------

class RMType {
public:
	RMType(Context ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods);
	RMType(RedisModuleType *type);

	RedisModuleType *Unwrap() noexcept;
	operator RedisModuleType *() noexcept;
	operator const RedisModuleType *() const noexcept;
private:
	RedisModuleType *_type;
};

//---------------------------------------------------------------------------------------------

class Key {
public:
	Key(Context ctx, const String& keyname, int mode);
	Key(RedisModuleKey *key);
	int DeleteKey();
	
	size_t ValueLength() noexcept;
	
	mstime_t GetExpire() noexcept;
	int SetExpire(mstime_t expire);

	int Type() noexcept;
	RMType GetType() noexcept;

	void *GetValue() noexcept;
	int SetValue(RMType mt, void *value);

	RedisModuleKey *Unwrap() noexcept;
	operator RedisModuleKey *() noexcept;
	operator const RedisModuleKey *() const noexcept;
protected:
    std::unique_ptr<RedisModuleKey, decltype(RedisModule_CloseKey)> _key;
};

//---------------------------------------------------------------------------------------------

class StringKey : Key {
	using Key::Key;
public:
	int Set(String& str);
	char *DMA(size_t &len, int mode); // direct memory access
	int Truncate(size_t newlen);
};

//---------------------------------------------------------------------------------------------

struct List : Key {
	using Key::Key;
public:
	int Push(int where, String& ele);
	String Pop(int where);

	String Get(long index);
	int Set(long index, String& value);
	int Insert(long index, String& value);
	int Delete(long index);
};

//---------------------------------------------------------------------------------------------

class Zset : Key {
	using Key::Key;
public:
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
	using Key::Key;
public:
	template<typename... Vargs>
	int Set(int flags, Vargs... vargs);
	template<typename... Vargs>
	int Get(int flags, Vargs... vargs);
};

//---------------------------------------------------------------------------------------------

class User {
public:
	User(const char *name);
	User(String& name);

	int SetACL(const char* acl);
	int ACLCheckCommandPermissions(Args& args);
	int ACLCheckKeyPermissions(String& key, int flags);
	int ACLCheckChannelPermissions(String& ch, int flags);
	void ACLAddLogEntry(Context ctx, String& object, RedisModuleACLLogEntryReason reason);

	static String GetCurrentUserName(Context ctx);
	static int RedactClientCommandArgument(Context ctx, int pos);

	RedisModuleUser *Unwrap() noexcept;
	operator RedisModuleUser *() noexcept;
private:
	std::unique_ptr<RedisModuleUser, decltype(RedisModule_FreeModuleUser)> _user;
};

//---------------------------------------------------------------------------------------------

class Dict {
public:
	class Iter {
	public:
		Iter(RedisModuleDictIter *iter);

		int Reseek(const char *op, void *key, size_t keylen);
		int Reseek(const char *op, String& key);
		void *Next(size_t *keylen, void **dataptr);
		void *Prev(size_t *keylen, void **dataptr);
		// No Context. Used only for String AutoMemory.
		String Next(void **dataptr);
		String Prev(void **dataptr);
		int Compare(const char *op, void *key, size_t keylen);
		int Compare(const char *op, String& key);

		RedisModuleDictIter *Unwrap() noexcept;
		operator RedisModuleDictIter *();
	private:
		std::unique_ptr<RedisModuleDictIter, decltype(RedisModule_DictIteratorStop)> _iter;
	};

	// No Context. Used only for AutoMemory. To be deprecated.
	Dict();

	uint64_t Size();
	
	int Set(void *key, size_t keylen, void *ptr);
	int Set(String& key, void *ptr);
	int Replace(void *key, size_t keylen, void *ptr);
	int Replace(String& key, void *ptr);
	void *Get(void *key, size_t keylen, int *nokey);
	void *Get(String& key, int *nokey);
	int Del(void *key, size_t keylen, void *oldval);
	int Del(String& key, void *oldval);
	Iter Start(const char *op, void *key, size_t keylen);
	Iter Start(const char *op, String& key);
	
	RedisModuleDict *Unwrap() noexcept;
	operator RedisModuleDict *();
private:
	static void Deleter(RedisModuleDict *) noexcept;
	friend class std::unique_ptr<RedisModuleDict, decltype(&Deleter)>;
	std::unique_ptr<RedisModuleDict, decltype(&Deleter)> _dict;
};

//---------------------------------------------------------------------------------------------

class ServerInfo {
	// No Context. Used only for AutoMemory. To be deprecated.
	ServerInfo(const char *section);

	void GetField(const char* field, String& str);
	void GetField(const char* field, const char **str);
	int GetField(const char* field, long long& ll);
	int GetField(const char* field, unsigned long long& ull);
	int GetField(const char* field, double& d);

	RedisModuleServerInfoData *Unwrap() noexcept;
	operator RedisModuleServerInfoData *();
private:
	static void Deleter(RedisModuleServerInfoData *) noexcept;
	friend class std::unique_ptr<RedisModuleServerInfoData, decltype(&Deleter)>;
	std::unique_ptr<RedisModuleServerInfoData, decltype(&Deleter)> _info;
};

//---------------------------------------------------------------------------------------------

namespace Alloc {
void *Alloc(size_t bytes);
void *TryAlloc(size_t bytes);
void *Calloc(size_t nmemb, size_t size);
void *Realloc(void *ptr, size_t bytes);
void Free(void *ptr);
char *Strdup(const char *str);
void *PoolAlloc(Context ctx, size_t bytes);

size_t Size(void *ptr);
size_t Size(String& str);
size_t Size(Dict dict);
size_t UsableSize(void *ptr);
} // namespace Alloc

//---------------------------------------------------------------------------------------------

// replace with std::chrono
namespace Time {
mstime_t Milliseconds() noexcept;
uint64_t MonotonicMicroseconds() noexcept;
ustime_t Microseconds() noexcept;
ustime_t CachedMicroseconds() noexcept;
} // namespace Time

//---------------------------------------------------------------------------------------------

namespace EventLoop {
int Add(int fd, int mask, RedisModuleEventLoopFunc func, void *user_data);
int Add(RedisModuleEventLoopOneShotFunc func, void *user_data);
int Del(int fd, int mask);
} // namespace EventLoop

//---------------------------------------------------------------------------------------------

using CmdFunc = int(Context, const Args&);
namespace Command {
template <CmdFunc cmdfunc>
int Create(Context ctx, const char *name,
	const char *strflags, int firstkey, int lastkey, int keystep);
RedisModuleCommand *Get(Context ctx, const char *name);
int CreateSubCommand(RedisModuleCommand *parent, const char *name, RedisModuleCmdFunc cmdfunc,
	const char *strflags, int firstkey, int lastkey, int keystep);
int SetInfo(RedisModuleCommand *command, const RedisModuleCommandInfo *info);

String FilterArgGet(RedisModuleCommandFilterCtx *fctx, int pos);
}

//---------------------------------------------------------------------------------------------

namespace Info {  // should be class
void RegisterFunc(Context ctx, RedisModuleInfoFunc cb);
int AddSection(InfoContext ctx, const char *name);
int BeginDictField(InfoContext ctx, const char *name);
int EndDictField(InfoContext ctx);
int AddField(InfoContext ctx, const char *field, String& value);
int AddField(InfoContext ctx, const char *field, const char *value);
int AddField(InfoContext ctx, const char *field, double value);
int AddField(InfoContext ctx, const char *field, long long value);
int AddField(InfoContext ctx, const char *field, unsigned long long value);
}

//---------------------------------------------------------------------------------------------

namespace Config {
int RegisterBool(Context ctx, const char *name, int default_val, unsigned int flags,
	RedisModuleConfigGetBoolFunc getfn, RedisModuleConfigSetBoolFunc setfn,
	RedisModuleConfigApplyFunc applyfn, void *privdata);
int RegisterNumeric(Context ctx, const char *name, long long default_val, unsigned int flags,
	long long min, long long max, RedisModuleConfigGetNumericFunc getfn,
	RedisModuleConfigSetNumericFunc setfn, RedisModuleConfigApplyFunc applyfn, void *privdata);
int RegisterString(Context ctx, const char *name, const char *default_val, unsigned int flags,
	RedisModuleConfigGetStringFunc getfn, RedisModuleConfigSetStringFunc setfn,
	RedisModuleConfigApplyFunc applyfn, void *privdata);
int RegisterEnum(Context ctx, const char *name, int default_val, unsigned int flags,
	const char **enum_values, const int *int_values, int num_enum_vals,
	RedisModuleConfigGetEnumFunc getfn, RedisModuleConfigSetEnumFunc setfn,
	RedisModuleConfigApplyFunc applyfn, void *privdata);
int Load(Context ctx);
}

//---------------------------------------------------------------------------------------------

class IO {
public:
	IO(RedisModuleIO* io);
	Context GetContext();

	void Save(uint64_t value);
	void Load(uint64_t& value);

	void Save(int64_t value);
	void Load(int64_t& value);

	void Save(String& s);
	void Load(String& s);

	void Save(const char *str, size_t len);
	void Load(char **str, size_t& len);

	void Save(double value);
	void Load(double& value);

	void Save(float value);
	void Load(float& vlaue);

	template<typename... Vargs>
	void EmitAOF(const char *cmdname, const char *fmt, Vargs... vargs);
	template<typename... Vargs>
	void LogIOError(const char *levelstr, const char *fmt, Vargs... vargs) noexcept;
private:
	RedisModuleIO* _io;
};

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
