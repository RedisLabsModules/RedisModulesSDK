#pragma once

#include <utility> // std::swap
#include "moduleapi.h"

namespace RedisModule {

///////////////////////////////////////////////////////////////////////////////////////////////

void *Alloc::Alloc(size_t bytes) {
	return RedisModule_Alloc(bytes);
}
void *Alloc::Calloc(size_t nmemb, size_t size) {
	return RedisModule_Calloc(nmemb, size);
}
void *Alloc::Realloc(void *ptr, size_t bytes) {
	return RedisModule_Realloc(ptr, bytes);
}
void Alloc::Free(void *ptr) {
	RedisModule_Free(ptr);
}
char *Alloc::Strdup(const char *str) {
	return RedisModule_Strdup(str);
}
void *Alloc::PoolAlloc(Context ctx, size_t bytes) {
	return RedisModule_PoolAlloc(ctx, bytes);
}
size_t Alloc::Size(void *ptr) {
	return RedisModule_MallocSize(ptr);
}
size_t Alloc::Size(String& str) {
	return RedisModule_MallocSizeString(str);
}
size_t Alloc::Size(Dict dict) {
	return RedisModule_MallocSizeDict(dict);
}
size_t Alloc::UsableSize(void *ptr) {
	return RedisModule_MallocUsableSize(ptr);
}

mstime_t Time::Milliseconds() noexcept {
	return RedisModule_Milliseconds();
}
uint64_t Time::MonotonicMicroseconds() noexcept {
	return RedisModule_MonotonicMicroseconds();
}
ustime_t Time::Microseconds() noexcept {
	return RedisModule_Microseconds();
}
ustime_t Time::CachedMicroseconds() noexcept {
	return RedisModule_CachedMicroseconds();
}

int EventLoop::Add(int fd, int mask, RedisModuleEventLoopFunc func, void *user_data) {
	return RedisModule_EventLoopAdd(fd, mask, func, user_data);
}
int EventLoop::Add(RedisModuleEventLoopOneShotFunc func, void *user_data) {
	return RedisModule_EventLoopAddOneShot(func, user_data);
}
int EventLoop::Del(int fd, int mask) {
	return RedisModule_EventLoopDel(fd, mask);
}

int Reply::WrongArity(Context ctx) {
	return RedisModule_WrongArity(ctx);
}
int Reply::Error(Context ctx, const char *err) {
	return RedisModule_ReplyWithError(ctx, err);
}
int Reply::Null(Context ctx) {
	return RedisModule_ReplyWithNull(ctx);
}
int Reply::Array(Context ctx, long len) {
	return RedisModule_ReplyWithArray(ctx, len);
}
void Reply::SetArrayLength(Context ctx, long len) {
	RedisModule_ReplySetArrayLength(ctx, len);
}
int Reply::Map(Context ctx, long len) {
	return RedisModule_ReplyWithMap(ctx, len);
}
void Reply::SetMapLength(Context ctx, long len) {
	RedisModule_ReplySetMapLength(ctx, len);
}
int Reply::Set(Context ctx, long len) {
	return RedisModule_ReplyWithSet(ctx, len);
}
void Reply::SetSetLength(Context ctx, long len) {
	RedisModule_ReplySetSetLength(ctx, len);
}
int Reply::Attribute(Context ctx, long len) {
	return RedisModule_ReplyWithAttribute(ctx, len);
}
void Reply::SetAttributeLength(Context ctx, long len) {
	RedisModule_ReplySetAttributeLength(ctx, len);
}
int Reply::Bool(Context ctx, int b) {
	return RedisModule_ReplyWithBool(ctx, b);
}
int Reply::LongLong(Context ctx, long long ll) {
	return RedisModule_ReplyWithLongLong(ctx, ll);
}
int Reply::Double(Context ctx, double d) {
	return RedisModule_ReplyWithDouble(ctx, d);
}
int Reply::BigNumber(Context ctx, const char *bignum, size_t len) {
	return RedisModule_ReplyWithBigNumber(ctx, bignum, len);
}
int Reply::SimpleString(Context ctx, const char *msg) {
	return RedisModule_ReplyWithSimpleString(ctx, msg);
}
int Reply::StringBuffer(Context ctx, const char *buf, size_t len) {
	return RedisModule_ReplyWithStringBuffer(ctx, buf, len);
}
int Reply::VerbatimStringType(Context ctx, const char *buf, size_t len, const char *ext) {
	return RedisModule_ReplyWithVerbatimStringType(ctx, buf, len, ext);
}
int Reply::String(Context ctx, RedisModule::String& str) {
	return RedisModule_ReplyWithString(ctx, str);
}
int Reply::CallReply(Context ctx, RedisModule::CallReply reply) {
	return RedisModule_ReplyWithCallReply(ctx, reply);
}

///////////////////////////////////////////////////////////////////////////////////////////////

int Command::Create(Context ctx, const char *name, RedisModuleCmdFunc cmdfunc,
	const char *strflags, int firstkey, int lastkey, int keystep) {
	return RedisModule_CreateCommand(ctx, name, cmdfunc, strflags, firstkey, lastkey, keystep);
}
RedisModuleCommand *Command::Get(Context ctx, const char *name) {
	return RedisModule_GetCommand(ctx, name);
}
int Command::CreateSub(RedisModuleCommand *parent, const char *name, RedisModuleCmdFunc cmdfunc,
	const char *strflags, int firstkey, int lastkey, int keystep) {
	return RedisModule_CreateSubcommand(parent, name, cmdfunc, strflags, firstkey, lastkey, keystep);
}
int Command::SetInfo(RedisModuleCommand *command, const RedisModuleCommandInfo *info) {
	return RedisModule_SetCommandInfo(command, info);
}
String Command::FilterArgGet(RedisModuleCommandFilterCtx *fctx, int pos) {
	return RedisModule_CommandFilterArgGet(fctx, pos);
}

template<typename... Vargs>
void Log::Log(Context ctx, const char *level, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_Log(ctx, level, fmt, vargs...);
}
template<typename... Vargs>
void Log::LogIOError(IO io, const char *levelstr, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_LogIOError(io, levelstr, fmt, vargs...);
}

void DB_KEY::AutoMemory(Context ctx) noexcept {
	RedisModule_AutoMemory(ctx);
}

bool DB_KEY::IsKeysPositionRequest(Context ctx) noexcept {
	return RedisModule_IsKeysPositionRequest(ctx);
}
void DB_KEY::KeyAtPos(Context ctx, int pos) noexcept {
	RedisModule_KeyAtPos(ctx, pos);
}

int DB_KEY::GetSelectedDb(Context ctx) noexcept {
	return RedisModule_GetSelectedDb(ctx);
}
void DB_KEY::SelectDb(Context ctx, int newid) {
	if (RedisModule_SelectDb(ctx, newid) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
unsigned long long DB_KEY::GetClientId(Context ctx) noexcept {
	return RedisModule_GetClientId(ctx);
}

template<typename... Vargs>
void DB_KEY::Replicate(Context ctx, const char *cmdname, const char *fmt, Vargs... vargs) {
	if (RedisModule_Replicate(ctx, cmdname, fmt, vargs...) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
void DB_KEY::Replicate(Context ctx) noexcept {
	RedisModule_ReplicateVerbatim(ctx);
}

//---------------------------------------------------------------------------------------------

BlockedClient::BlockedClient(Context ctx, RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback,
	void (*free_privdata)(RedisModuleCtx *, void*), long long timeout_ms)
	: _bc(RedisModule_BlockClient(ctx, reply_callback, timeout_callback, free_privdata, timeout_ms))
{ }
int BlockedClient::UnblockClient(void *privdata) {
	return RedisModule_UnblockClient(_bc, privdata);
}
int BlockedClient::AbortBlock() {
	return RedisModule_AbortBlock(_bc);
}

BlockedClient::operator RedisModuleBlockedClient *() { return _bc; }

//---------------------------------------------------------------------------------------------

ThreadSafeContext::ThreadSafeContext(BlockedClient bc) 
	: _ctx(RedisModule_GetThreadSafeContext(bc))
{ }
ThreadSafeContext::ThreadSafeContext(Context ctx) 
	: _ctx(RedisModule_GetDetachedThreadSafeContext(ctx))
{ }
ThreadSafeContext::~ThreadSafeContext() {
	RedisModule_FreeThreadSafeContext(_ctx);
}

void ThreadSafeContext::Lock() {
	RedisModule_ThreadSafeContextLock(_ctx);
}
int ThreadSafeContext::TryLock() {
	return RedisModule_ThreadSafeContextTryLock(_ctx);
}
void ThreadSafeContext::Unlock() {
	RedisModule_ThreadSafeContextUnlock(_ctx);
}

//---------------------------------------------------------------------------------------------

RMType::RMType(Context ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods)
	: _type(RedisModule_CreateDataType(ctx, name, encver, typemethods))
{ }
RMType::RMType(RedisModuleType *type) : _type(type) {}

RMType::operator RedisModuleType *() noexcept { return _type; }
RMType::operator const RedisModuleType *() const noexcept { return _type; }

//---------------------------------------------------------------------------------------------

String::String(const char *ptr, size_t len)
	: _str(RedisModule_CreateString(NULL, ptr, len))
{ }

String::String(long long ll)
	: _str(RedisModule_CreateStringFromLongLong(NULL, ll))
{ }

String::String(const RedisModuleString *str)
	: _str(RedisModule_CreateStringFromString(NULL, str))
{ }

void String::Retain() {
	RedisModule_RetainString(NULL, _str);
}

String::~String() noexcept {
	RedisModule_FreeString(NULL, _str);
}

void String::AppendBuffer(const char *buf, size_t len) {
	if (RedisModule_StringAppendBuffer(NULL, _str, buf, len) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
void String::Trim() noexcept {
	RedisModule_TrimStringAllocation(_str);
}
const char *String::PtrLen(size_t &len) const noexcept {
	return RedisModule_StringPtrLen(_str, &len);
}

int String::ToLongLong(long long& ll) const {
	return RedisModule_StringToLongLong(_str, &ll);
}
long long String::ToLongLong() const {
	long long ll;
	if (ToLongLong(ll) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return ll;
}
int String::ToDouble(double& d) const {
	return RedisModule_StringToDouble(_str, &d);
}
double String::ToDouble() const {
	double d;
	if (ToDouble(d) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return d;
}
int String::ToLongDouble(long double& ld) const {
	return RedisModule_StringToLongDouble(_str, &ld);
}
long double String::ToLongDouble() const {
	long double ld = 0;
	if (ToLongDouble(ld) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return ld;
}
int String::ToULongLong(unsigned long long& ull) const {
	return RedisModule_StringToULongLong(_str, &ull);
}
unsigned long long String::ToULongLong() const {
	unsigned long long ull = 0;
	if (ToULongLong(ull) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return ull;
}

String::operator RedisModuleString *() noexcept { return _str; }
String::operator const RedisModuleString *() const noexcept { return _str; }

int String::Compare(String& s1, String& s2) noexcept {
	return RedisModule_StringCompare(s1, s2);
}

void swap(String& s1, String& s2) noexcept {
	std::swap(s1._str, s2._str);
}

//---------------------------------------------------------------------------------------------

Key::Key(Context ctx, String& keyname, int mode) // OpenKey
	: _key((RedisModuleKey *)RedisModule_OpenKey(ctx, keyname, mode))
{ }
Key::Key(RedisModuleKey *key) : _key(key) { }

Key::~Key() noexcept { // CloseKey
	RedisModule_CloseKey(_key);
}
int Key::DeleteKey() {
	return RedisModule_DeleteKey(_key);
}

mstime_t Key::GetExpire() noexcept {
	return RedisModule_GetExpire(_key);
}
int Key::SetExpire(mstime_t expire){
	return RedisModule_SetExpire(_key, expire);
}	

int Key::Type() noexcept {
	return RedisModule_KeyType(_key);
}
RMType Key::GetType() noexcept {
	return RedisModule_ModuleTypeGetType(_key);
}

size_t Key::ValueLength() noexcept {
	return RedisModule_ValueLength(_key);
}
void *Key::GetValue() noexcept {
	return RedisModule_ModuleTypeGetValue(_key);
}
int Key::SetValue(RMType mt, void *value) {
	return RedisModule_ModuleTypeSetValue(_key, mt, value);
}

Key::operator RedisModuleKey *() noexcept { return _key; }
Key::operator const RedisModuleKey *() const noexcept { return _key; }

//---------------------------------------------------------------------------------------------

StringKey::StringKey(Context ctx, String& keyname, int mode) : Key(ctx, keyname, mode) {}

int StringKey::Set(String& str) {
	return RedisModule_StringSet(_key, str);
}
char *StringKey::DMA(size_t &len, int mode) {
	return RedisModule_StringDMA(_key, &len, mode);
}
int StringKey::Truncate(size_t newlen) {
	return RedisModule_StringTruncate(_key, newlen);
}

//---------------------------------------------------------------------------------------------

List::List(Context ctx, String& keyname, int mode) : Key(ctx, keyname, mode) {}

int List::Push(int where, String& ele) {
	return RedisModule_ListPush(_key, where, ele);
}
String List::Pop(int where) {
	return RedisModule_ListPop(_key, where);
}
String List::Get(long index) {
	return RedisModule_ListGet(_key, index);
}
int List::Set(long index, String& value) {
	return RedisModule_ListSet(_key, index, value);
}
int List::Insert(long index, String& value) {
	return RedisModule_ListInsert(_key, index, value);
}
int List::Delete(long index) {
	return RedisModule_ListDelete(_key, index);
}

//---------------------------------------------------------------------------------------------

Zset::Zset(Context ctx, String& keyname, int mode) : Key(ctx, keyname, mode) {}

int Zset::Add(double score, String& ele, int *flagsptr) {
	return RedisModule_ZsetAdd(_key, score, ele, flagsptr);
}
int Zset::Incrby(double score, String& ele, int *flagsptr, double *newscore) {
	return RedisModule_ZsetIncrby(_key, score, ele, flagsptr, newscore);
}
int Zset::Rem(String& ele, int *deleted) {
	return RedisModule_ZsetRem(_key, ele, deleted);
}
int Zset::Score(String& ele, double *score) {
	return RedisModule_ZsetScore(_key, ele, score);
}

void Zset::RangeStop() {
	RedisModule_ZsetRangeStop(_key);
}
int Zset::RangeEndReached(){
	return RedisModule_ZsetRangeEndReached(_key);
}
int Zset::FirstInScoreRange(double min, double max, int minex, int maxex) {
	return RedisModule_ZsetFirstInScoreRange(_key, min, max, minex, maxex);
}
int Zset::LastInScoreRange(double min, double max, int minex, int maxex) {
	return RedisModule_ZsetLastInScoreRange(_key, min, max, minex, maxex);
}
int Zset::FirstInLexRange(String& min, String& max) {
	return RedisModule_ZsetFirstInLexRange(_key, min, max);
}
int Zset::LastInLexRange(String& min, String& max) {
	return RedisModule_ZsetLastInLexRange(_key, min, max);
}
String Zset::RangeCurrentElement(double *score) {
	return RedisModule_ZsetRangeCurrentElement(_key, score);
}
int Zset::RangeNext() {
	return RedisModule_ZsetRangeNext(_key);
}
int Zset::RangePrev() {
	return RedisModule_ZsetRangePrev(_key);
}

//---------------------------------------------------------------------------------------------

Hash::Hash(Context ctx, String& keyname, int mode) : Key(ctx, keyname, mode) {}

template<typename... Vargs>
int Hash::Set(int flags, Vargs... vargs) {
	return RedisModule_HashSet(_key, flags, vargs...);
}

template<typename... Vargs>
int Hash::Get(int flags, Vargs... vargs) {
	return RedisModule_HashGet(_key, flags, vargs...);
}

//---------------------------------------------------------------------------------------------

Context IO::GetContext() {
	return Context(RedisModule_GetContextFromIO(_io));
}

void IO::Save(uint64_t value) {
	RedisModule_SaveUnsigned(_io, value);
}
void IO::Load(uint64_t& value) {
	value = RedisModule_LoadUnsigned(_io);
}

void IO::Save(int64_t value) {
	RedisModule_SaveSigned(_io, value);
}
void IO::Load(int64_t& value) {
	value = RedisModule_LoadSigned(_io);
}

void IO::Save(String& s) {
	RedisModule_SaveString(_io, s);
}
void IO::Load(String& s) {
	String ss(RedisModule_LoadString(_io));
	swap(s, ss);
}

void IO::Save(const char *str, size_t len) {
	RedisModule_SaveStringBuffer(_io, str, len);
}
void IO::Load(char **str, size_t &len) {
	*str = RedisModule_LoadStringBuffer(_io, &len);
}

void IO::Save(double value) {
	RedisModule_SaveDouble(_io, value);
}
void IO::Load(double& d) {
	d = RedisModule_LoadDouble(_io);
}

void IO::Save(float value) {
	RedisModule_SaveFloat(_io, value);
}
void IO::Load(float& value) {
	value = RedisModule_LoadFloat(_io);
}

template<typename... Vargs>
void IO::EmitAOF(const char *cmdname, const char *fmt, Vargs... vargs) {
	RedisModule_EmitAOF(_io, cmdname, fmt, vargs...);
}

//---------------------------------------------------------------------------------------------

template<typename... Vargs>
CallReply::CallReply(Context ctx, const char *cmdname, const char *fmt, Vargs... vargs) 
	: _reply(RedisModule_Call(ctx, cmdname, fmt, vargs...))
{ }
CallReply::CallReply(RedisModuleCallReply *reply) : _reply(reply) {}
CallReply::~CallReply() noexcept {
	RedisModule_FreeCallReply(_reply);
}
int CallReply::Type() {
	return RedisModule_CallReplyType(_reply);
}
size_t CallReply::Length() {
	return RedisModule_CallReplyLength(_reply);
}
long long CallReply::Integer() noexcept {
	return RedisModule_CallReplyInteger(_reply);
}
double CallReply::Double() noexcept {
	return RedisModule_CallReplyDouble(_reply);
}
const char *CallReply::BigNumber(size_t& len) {
	return RedisModule_CallReplyBigNumber(_reply, &len);
}
const char *CallReply::Verbatim(size_t& len, const char **format) {
	return RedisModule_CallReplyVerbatim(_reply, &len, format);
}
bool CallReply::Bool() noexcept {
	return RedisModule_CallReplyBool(_reply);
}
const char *CallReply::StringPtr(size_t &len) {
	return RedisModule_CallReplyStringPtr(_reply, &len);
}
String CallReply::CreateString() {
	return RedisModule_CreateStringFromCallReply(_reply);
}

CallReply CallReply::ArrayElement(size_t idx) {
	return RedisModule_CallReplyArrayElement(_reply, idx);
}
CallReply CallReply::SetElement(size_t idx) {
	return RedisModule_CallReplySetElement(_reply, idx);
}
int CallReply::MapElement(size_t idx, CallReply& key, CallReply& val) {
	// TODO: not be vile
	// &key doesn't work: cannot convert ‘RM::CallReply*’ to ‘RMCallReply**’
	// &static_cast<RMCallReply*>(key): lvalue required as unary ‘&’ operand
	// &key._reply would work, except that breaks encapsulation.
	RedisModuleCallReply *tempkey = key;
	RedisModuleCallReply *tempval = val;
	int res = RedisModule_CallReplyMapElement(_reply, idx, &tempkey, &tempval);
	key = tempkey;
	val = tempval;
	return res;
}
int CallReply::AttributeElement(size_t idx, CallReply& key, CallReply& val) {
	RedisModuleCallReply *tempkey = key;
	RedisModuleCallReply *tempval = val;
	int res = RedisModule_CallReplyAttributeElement(_reply, idx, &tempkey, &tempval);
	key = tempkey;
	val = tempval;
	return res;
}
CallReply CallReply::Attribute() {
	return RedisModule_CallReplyAttribute(_reply);
}

const char *CallReply::Protocol(size_t &len) {
	return RedisModule_CallReplyProto(_reply, &len);
}
CallReply::operator RedisModuleCallReply *() noexcept { return _reply; }

//---------------------------------------------------------------------------------------------

User::User(const char *name) : _user(RedisModule_CreateModuleUser(name)) {}
User::User(String& name) : _user(RedisModule_GetModuleUserFromUserName(name)) {}
User::~User() noexcept {
	RedisModule_FreeModuleUser(_user);
}

int User::SetACL(const char* acl) {
	return RedisModule_SetModuleUserACL(_user, acl);
}
int User::ACLCheckCommandPermissions(String *argv, int argc) {
	return RedisModule_ACLCheckCommandPermissions(_user, argv, argc);
}
int User::ACLCheckKeyPermissions(String& key, int flags) {
	return RedisModule_ACLCheckKeyPermissions(_user, key, flags);
}
int User::ACLCheckChannelPermissions(String& ch, int flags) {
	return RedisModule_ACLCheckChannelPermissions(_user, ch, flags);
}
void User::ACLAddLogEntry(Context ctx, String& object, RedisModuleACLLogEntryReason reason) {
	return RedisModule_ACLAddLogEntry(ctx, _user, object, reason);
}

String User::GetCurrentUserName(Context ctx) {
	return RedisModule_GetCurrentUserName(ctx);
}
int User::RedactClientCommandArgument(Context ctx, int pos) {
	return RedisModule_RedactClientCommandArgument(ctx, pos);
}

User::operator RedisModuleUser *() noexcept { return _user; }


//---------------------------------------------------------------------------------------------

Args::Args(int argc, RedisModuleString **argv)
	: _args(std::vector<String>())
{
	_args.reserve(argc);
	RedisModuleString *arg = NULL;
	while ((arg = *argv++) != NULL) {
		_args.emplace_back(arg);
	}
}

//---------------------------------------------------------------------------------------------

template <class T>
CmdFunctor<T>::CmdFunctor() {}
template <class T>
int CmdFunctor<T>::Run(const Args &args) { return REDISMODULE_OK; }
template <class T>
int CmdFunctor<T>::cmdfunc(Context ctx, Args& args) {
	T cmd{ctx};
	return cmd.Run(args);
}

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
