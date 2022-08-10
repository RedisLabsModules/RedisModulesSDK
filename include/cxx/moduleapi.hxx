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
size_t Alloc::UsableSize(void *ptr) {
	return RedisModule_MallocUsableSize(ptr);
}
size_t Alloc::SizeString(String& str) {
	return RedisModule_MallocSizeString(str);
}
size_t Alloc::SizeDict(Dict dict) {
	return RedisModule_MallocSizeDict(dict);
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
int EventLoop::Del(int fd, int mask) {
	return RedisModule_EventLoopDel(fd, mask);
}
int EventLoop::AddOneShot(RedisModuleEventLoopOneShotFunc func, void *user_data) {
	return RedisModule_EventLoopAddOneShot(func, user_data);
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

String FilterArgGet(RedisModuleCommandFilterCtx *fctx, int pos);

void DB_KEY::AutoMemory(Context ctx) {
	RedisModule_AutoMemory(ctx);
}

bool DB_KEY::IsKeysPositionRequest(Context ctx) {
	return RedisModule_IsKeysPositionRequest(ctx);
}
void DB_KEY::KeyAtPos(Context ctx, int pos) {
	RedisModule_KeyAtPos(ctx, pos);
}

int DB_KEY::GetSelectedDb(Context ctx) {
	return RedisModule_GetSelectedDb(ctx);
}
void DB_KEY::SelectDb(Context ctx, int newid) {
	if (RedisModule_SelectDb(ctx, newid) != REDISMODULE_OK) {
		throw ...;
	}
}
unsigned long long DB_KEY::GetClientId(Context ctx) {
	return RedisModule_GetClientId(ctx);
}
template<typename... Vargs>
void DB_KEY::Log(Context ctx, const char *level, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_Log(ctx, level, fmt, vargs...);
}

template<typename... Vargs>
void DB_KEY::Replicate(Context ctx, const char *cmdname, const char *fmt, Vargs... vargs) {
	if (RedisModule_Replicate(ctx, cmdname, fmt, vargs...) != REDISMODULE_OK) {
		throw ...;
	}
}
void DB_KEY::ReplicateVerbatim(Context ctx) noexcept {
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

ThreadSafeContext::ThreadSafeContext(BlockedClient bc) {
	Context::_ctx = RedisModule_GetThreadSafeContext(bc);
}
ThreadSafeContext::~ThreadSafeContext() {
	RedisModule_FreeThreadSafeContext(Context::_ctx);
}

void ThreadSafeContext::Lock() {
	RedisModule_ThreadSafeContextLock(Context::_ctx);
}
int ThreadSafeContext::TryLock() {
	return RedisModule_ThreadSafeContextTryLock(Context::_ctx);
}
void ThreadSafeContext::Unlock() {
	RedisModule_ThreadSafeContextUnlock(Context::_ctx);
}

//---------------------------------------------------------------------------------------------

RMType::RMType(Context ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods)
	: _type(RedisModule_CreateDataType(ctx, name, encver, typemethods))
{ }
RMType::RMType(RedisModuleType *type) : _type(type) {}

RMType::operator RedisModuleType *() { return _type; }
RMType::operator const RedisModuleType *() const { return _type; }

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

String::~String() {
	RedisModule_FreeString(NULL, _str);
}

const char *String::PtrLen(size_t &len) const {
	return RedisModule_StringPtrLen(_str, &len);
}

long long String::ToLongLong() const {
	long long ll;
	if (RedisModule_StringToLongLong(_str, &ll) != REDISMODULE_OK) {
		throw ...;
	}
	return ll;
}

double String::ToDouble() const {
	double d;
	if (RedisModule_StringToDouble(_str, &d) != REDISMODULE_OK) {
		throw ...;
	}
	return d;
}

long double String::ToLongDouble() const {
	long double ld = 0;
	if (RedisModule_StringToLongDouble(_str, &ld) != REDISMODULE_OK) {
		throw ...;
	}
	return ld;
}

void String::AppendBuffer(const char *buf, size_t len) {
	if (RedisModule_StringAppendBuffer(NULL, _str, buf, len) != REDISMODULE_OK) {
		throw ...;
	}
}

String::operator RedisModuleString *() { return _str; }
String::operator const RedisModuleString *() const { return _str; }

int StringCompare(String& s1, String& s2) noexcept {
	return RedisModule_StringCompare(s1, s2);
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

int Key::Type() {
	return RedisModule_KeyType(_key);
}

size_t Key::ValueLength() {
	return RedisModule_ValueLength(_key);
}

mstime_t Key::GetExpire() {
	return RedisModule_GetExpire(_key);
}
int Key::SetExpire(mstime_t expire){
	return RedisModule_SetExpire(_key, expire);
}	

RMType Key::GetType() {
	return RedisModule_ModuleTypeGetType(_key);
}

void *Key::GetValue() {
	return RedisModule_ModuleTypeGetValue(_key);
}
int Key::SetValue(RMType mt, void *value) {
	return RedisModule_ModuleTypeSetValue(_key, mt, value);
}

Key::operator RedisModuleKey *() { return _key; }
Key::operator const RedisModuleKey *() const { return _key; }

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
	return String(RedisModule_ListPop(_key, where));
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
	return String(RedisModule_ZsetRangeCurrentElement(_key, score));
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

void IO::SaveUnsigned(uint64_t value) {
	RedisModule_SaveUnsigned(_io, value);
}
uint64_t IO::LoadUnsigned() {
	return RedisModule_LoadUnsigned(_io);
}

void IO::SaveSigned(int64_t value) {
	RedisModule_SaveSigned(_io, value);
}
int64_t IO::LoadSigned() {
	return RedisModule_LoadSigned(_io);
}

void IO::SaveString(String& s) {
	RedisModule_SaveString(_io, s);
}
String IO::LoadString() {
	return String(RedisModule_LoadString(_io));
}

void IO::SaveStringBuffer(const char *str, size_t len) {
	RedisModule_SaveStringBuffer(_io, str, len);
}
char *IO::LoadStringBuffer(size_t &len) {
	return RedisModule_LoadStringBuffer(_io, &len);
}

void IO::SaveDouble(double value) {
	RedisModule_SaveDouble(_io, value);
}
double IO::LoadDouble() {
	return RedisModule_LoadDouble(_io);
}

void IO::SaveFloat(float value) {
	RedisModule_SaveFloat(_io, value);
}
float IO::LoadFloat() {
	return RedisModule_LoadFloat(_io);
}

template<typename... Vargs>
void IO::EmitAOF(const char *cmdname, const char *fmt, Vargs... vargs) {
	RedisModule_EmitAOF(_io, cmdname, fmt, vargs...);
}

template<typename... Vargs>
void IO::LogIOError(const char *levelstr, const char *fmt, Vargs... vargs) {
	RedisModule_LogIOError(_io, levelstr, fmt, vargs...);
}

//---------------------------------------------------------------------------------------------

template<typename... Vargs>
CallReply::CallReply(const char *cmdname, const char *fmt, Vargs... vargs) 
	: _reply(RedisModule_Call(Context::_ctx, cmdname, fmt, vargs...))
{ }
CallReply::CallReply(RedisModuleCallReply *reply) : _reply(reply) {}
CallReply::~CallReply() {
	RedisModule_FreeCallReply(_reply);
}
const char *CallReply::StringPtr(size_t &len) {
	return RedisModule_CallReplyStringPtr(_reply, &len);
}
String CallReply::CreateString() {
	return String(RedisModule_CreateStringFromCallReply(_reply));
}

const char *CallReply::Proto(size_t &len) {
	return RedisModule_CallReplyProto(_reply, &len);
}
int CallReply::Type() {
	return RedisModule_CallReplyType(_reply);
}
long long CallReply::Integer() {
	return RedisModule_CallReplyInteger(_reply);
}
size_t CallReply::Length() {
	return RedisModule_CallReplyLength(_reply);
}
CallReply CallReply::ArrayElement(size_t idx) {
	return CallReply(RedisModule_CallReplyArrayElement(_reply, idx));
}
CallReply::operator RedisModuleCallReply *() { return _reply; }

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
