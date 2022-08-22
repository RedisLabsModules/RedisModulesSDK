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
int Reply::CallReply(Context ctx, RedisModule::CallReply& reply) {
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

void Info::RegisterFunc(Context ctx, RedisModuleInfoFunc cb) {
	RedisModule_RegisterInfoFunc(ctx, cb);
}
int Info::AddSection(InfoContext ctx, const char *name) {
	return RedisModule_InfoAddSection(ctx, name);
}
int Info::BeginDictField(InfoContext ctx, const char *name) {
	return RedisModule_InfoBeginDictField(ctx, name);
}
int Info::EndDictField(InfoContext ctx) {
	return RedisModule_InfoEndDictField(ctx);
}
int Info::AddField(InfoContext ctx, const char *field, String& value) {
	return RedisModule_InfoAddFieldString(ctx, field, value);
}
int Info::AddField(InfoContext ctx, const char *field, const char *value) {
	return RedisModule_InfoAddFieldCString(ctx, field, value);
}
int Info::AddField(InfoContext ctx, const char *field, double value) {
	return RedisModule_InfoAddFieldDouble(ctx, field, value);
}
int Info::AddField(InfoContext ctx, const char *field, long long value) {
	return RedisModule_InfoAddFieldLongLong(ctx, field, value);
}
int Info::AddField(InfoContext ctx, const char *field, unsigned long long value) {
	return RedisModule_InfoAddFieldULongLong(ctx, field, value);
}

template<typename... Vargs>
void Log::Log(Context ctx, const char *level, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_Log(ctx, level, fmt, RedisModule::Unwrap(vargs)...);
}
template<typename... Vargs>
void Log::LogIOError(IO io, const char *levelstr, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_LogIOError(io, levelstr, fmt, RedisModule::Unwrap(vargs)...);
}

// No AutoMemory. To be deprecated.
// void DB_KEY::AutoMemory(Context ctx) noexcept {
// 	RedisModule_AutoMemory(ctx);
// }

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
	if (RedisModule_Replicate(ctx, cmdname, fmt, RedisModule::Unwrap(vargs)...) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
void DB_KEY::Replicate(Context ctx) noexcept {
	RedisModule_ReplicateVerbatim(ctx);
}

//---------------------------------------------------------------------------------------------

BlockedClient::BlockedClient(Context ctx, RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback,
	void (*free_privdata)(RedisModuleCtx *, void*), long long timeout_ms)
	: _bc(RedisModule_BlockClient(ctx, reply_callback, timeout_callback, free_privdata, timeout_ms), &Deleter)
{ }
void BlockedClient::Deleter(RedisModuleBlockedClient *bc) noexcept {
	RedisModule_AbortBlock(bc); // you had your chance to catch errors yourself
}

void BlockedClient::UnblockClient(void *privdata) {
	if (RedisModule_UnblockClient(_bc.get(), privdata) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
void BlockedClient::AbortBlock() {
	if (RedisModule_AbortBlock(_bc.get()) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}

RedisModuleBlockedClient *BlockedClient::Unwrap() noexcept {
	return _bc.get();
}
BlockedClient::operator RedisModuleBlockedClient *() { return _bc.get(); }

//---------------------------------------------------------------------------------------------

ThreadSafeContext::ThreadSafeContext(BlockedClient& bc) 
	: _ctx(RedisModule_GetThreadSafeContext(bc), RedisModule_FreeThreadSafeContext)
{ }
ThreadSafeContext::ThreadSafeContext(Context ctx) 
	: _ctx(RedisModule_GetDetachedThreadSafeContext(ctx), RedisModule_FreeThreadSafeContext)
{ }

void ThreadSafeContext::Lock() noexcept {
	RedisModule_ThreadSafeContextLock(_ctx.get());
}
int ThreadSafeContext::TryLock() noexcept {
	return RedisModule_ThreadSafeContextTryLock(_ctx.get());
}
void ThreadSafeContext::Unlock() noexcept {
	RedisModule_ThreadSafeContextUnlock(_ctx.get());
}

//---------------------------------------------------------------------------------------------

RMType::RMType(Context ctx, const char *name, int encver, RedisModuleTypeMethods *typemethods)
	: _type(RedisModule_CreateDataType(ctx, name, encver, typemethods))
{ }
RMType::RMType(RedisModuleType *type) : _type(type) {}

RedisModuleType *RMType::Unwrap() noexcept {
	return _type;
}
RMType::operator RedisModuleType *() noexcept { return _type; }
RMType::operator const RedisModuleType *() const noexcept { return _type; }

//---------------------------------------------------------------------------------------------

String::String(const char *ptr, size_t len)
	: _str(RedisModule_CreateString(NULL, ptr, len))
{ }
String::String(long long ll)
	: _str(RedisModule_CreateStringFromLongLong(NULL, ll))
{ }
String::String(RedisModuleString *str) : _str(str)
{
	Retain();
}
String::String(const String& other) : _str(other._str) {
	Retain();
}
void String::Retain() {
	RedisModule_RetainString(NULL, _str);
}
String::String(String&& other) : _str(std::exchange(other._str, nullptr))
{ }
String& String::operator=(String other) {
	swap(other);
	return *this;
}
void String::swap(String& other) noexcept {
	using std::swap;
	swap(_str, other._str);
}
void swap(String& s1, String& s2) noexcept {
	s1.swap(s2);
}
String::~String() noexcept {
	if (_str != nullptr)
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

int String::ToLongLong(long long& ll) const noexcept {
	return RedisModule_StringToLongLong(_str, &ll);
}
long long String::ToLongLong() const {
	long long ll;
	if (ToLongLong(ll) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return ll;
}
int String::ToDouble(double& d) const noexcept {
	return RedisModule_StringToDouble(_str, &d);
}
double String::ToDouble() const {
	double d;
	if (ToDouble(d) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return d;
}
int String::ToLongDouble(long double& ld) const noexcept {
	return RedisModule_StringToLongDouble(_str, &ld);
}
long double String::ToLongDouble() const {
	long double ld = 0;
	if (ToLongDouble(ld) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return ld;
}
int String::ToULongLong(unsigned long long& ull) const noexcept {
	return RedisModule_StringToULongLong(_str, &ull);
}
unsigned long long String::ToULongLong() const {
	unsigned long long ull = 0;
	if (ToULongLong(ull) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return ull;
}

RedisModuleString *String::Unwrap() noexcept {
	return _str;
}
String::operator RedisModuleString *() noexcept { return _str; }
String::operator const RedisModuleString *() const noexcept { return _str; }

int String::Compare(const String& s1, const String& s2) noexcept {
	return RedisModule_StringCompare(s1, s2);
}

//---------------------------------------------------------------------------------------------

Key::Key(Context ctx, String keyname, int mode) // OpenKey
	: _key(RedisModule_OpenKey(ctx, keyname, mode), RedisModule_CloseKey)
{ }
Key::Key(RedisModuleKey *key) : _key(key, RedisModule_CloseKey) { }

int Key::DeleteKey() {
	return RedisModule_DeleteKey(_key.get());
}

mstime_t Key::GetExpire() noexcept {
	return RedisModule_GetExpire(_key.get());
}
int Key::SetExpire(mstime_t expire){
	return RedisModule_SetExpire(_key.get(), expire);
}	

int Key::Type() noexcept {
	return RedisModule_KeyType(_key.get());
}
RMType Key::GetType() noexcept {
	return RedisModule_ModuleTypeGetType(_key.get());
}

size_t Key::ValueLength() noexcept {
	return RedisModule_ValueLength(_key.get());
}
void *Key::GetValue() noexcept {
	return RedisModule_ModuleTypeGetValue(_key.get());
}
int Key::SetValue(RMType mt, void *value) {
	return RedisModule_ModuleTypeSetValue(_key.get(), mt, value);
}

RedisModuleKey *Key::Unwrap() noexcept {
	return _key.get();
}
Key::operator RedisModuleKey *() noexcept { return _key.get(); }
Key::operator const RedisModuleKey *() const noexcept { return _key.get(); }

//---------------------------------------------------------------------------------------------

int StringKey::Set(String& str) {
	return RedisModule_StringSet(_key.get(), str);
}
char *StringKey::DMA(size_t &len, int mode) {
	return RedisModule_StringDMA(_key.get(), &len, mode);
}
int StringKey::Truncate(size_t newlen) {
	return RedisModule_StringTruncate(_key.get(), newlen);
}

//---------------------------------------------------------------------------------------------

int List::Push(int where, String& ele) {
	return RedisModule_ListPush(_key.get(), where, ele);
}
String List::Pop(int where) {
	return RedisModule_ListPop(_key.get(), where);
}
String List::Get(long index) {
	return RedisModule_ListGet(_key.get(), index);
}
int List::Set(long index, String& value) {
	return RedisModule_ListSet(_key.get(), index, value);
}
int List::Insert(long index, String& value) {
	return RedisModule_ListInsert(_key.get(), index, value);
}
int List::Delete(long index) {
	return RedisModule_ListDelete(_key.get(), index);
}

//---------------------------------------------------------------------------------------------

int Zset::Add(double score, String& ele, int *flagsptr) {
	return RedisModule_ZsetAdd(_key.get(), score, ele, flagsptr);
}
int Zset::Incrby(double score, String& ele, int *flagsptr, double *newscore) {
	return RedisModule_ZsetIncrby(_key.get(), score, ele, flagsptr, newscore);
}
int Zset::Rem(String& ele, int *deleted) {
	return RedisModule_ZsetRem(_key.get(), ele, deleted);
}
int Zset::Score(String& ele, double *score) {
	return RedisModule_ZsetScore(_key.get(), ele, score);
}

void Zset::RangeStop() {
	RedisModule_ZsetRangeStop(_key.get());
}
int Zset::RangeEndReached(){
	return RedisModule_ZsetRangeEndReached(_key.get());
}
int Zset::FirstInScoreRange(double min, double max, int minex, int maxex) {
	return RedisModule_ZsetFirstInScoreRange(_key.get(), min, max, minex, maxex);
}
int Zset::LastInScoreRange(double min, double max, int minex, int maxex) {
	return RedisModule_ZsetLastInScoreRange(_key.get(), min, max, minex, maxex);
}
int Zset::FirstInLexRange(String& min, String& max) {
	return RedisModule_ZsetFirstInLexRange(_key.get(), min, max);
}
int Zset::LastInLexRange(String& min, String& max) {
	return RedisModule_ZsetLastInLexRange(_key.get(), min, max);
}
String Zset::RangeCurrentElement(double *score) {
	return RedisModule_ZsetRangeCurrentElement(_key.get(), score);
}
int Zset::RangeNext() {
	return RedisModule_ZsetRangeNext(_key.get());
}
int Zset::RangePrev() {
	return RedisModule_ZsetRangePrev(_key.get());
}

//---------------------------------------------------------------------------------------------

template<typename... Vargs>
int Hash::Set(int flags, Vargs... vargs) {
	return RedisModule_HashSet(_key.get(), flags, RedisModule::Unwrap(vargs)...);
}

template<typename... Vargs>
int Hash::Get(int flags, Vargs... vargs) {
	return RedisModule_HashGet(_key.get(), flags, RedisModule::Unwrap(vargs)...);
}

//---------------------------------------------------------------------------------------------

Context RDB::GetContext(IO io) {
	return Context(RedisModule_GetContextFromIO(io));
}

void RDB::Save(IO io, uint64_t value) {
	RedisModule_SaveUnsigned(io, value);
}
void RDB::Load(IO io, uint64_t& value) {
	value = RedisModule_LoadUnsigned(io);
}

void RDB::Save(IO io, int64_t value) {
	RedisModule_SaveSigned(io, value);
}
void RDB::Load(IO io, int64_t& value) {
	value = RedisModule_LoadSigned(io);
}

void RDB::Save(IO io, String& s) {
	RedisModule_SaveString(io, s);
}
void RDB::Load(IO io, String& s) {
	s = RedisModule_LoadString(io);
}

void RDB::Save(IO io, const char *str, size_t len) {
	RedisModule_SaveStringBuffer(io, str, len);
}
void RDB::Load(IO io, char **str, size_t &len) {
	*str = RedisModule_LoadStringBuffer(io, &len);
}

void RDB::Save(IO io, double value) {
	RedisModule_SaveDouble(io, value);
}
void RDB::Load(IO io, double& d) {
	d = RedisModule_LoadDouble(io);
}

void RDB::Save(IO io, float value) {
	RedisModule_SaveFloat(io, value);
}
void RDB::Load(IO io, float& value) {
	value = RedisModule_LoadFloat(io);
}

template<typename... Vargs>
void RDB::EmitAOF(IO io, const char *cmdname, const char *fmt, Vargs... vargs) {
	RedisModule_EmitAOF(io, cmdname, fmt, RedisModule::Unwrap(vargs)...);
}

//---------------------------------------------------------------------------------------------

template<typename... Vargs>
CallReply::CallReply(Context ctx, const char *cmdname, const char *fmt, Vargs... vargs) 
	: _reply(RedisModule_Call(ctx, cmdname, fmt, RedisModule::Unwrap(vargs)...), RedisModule_FreeCallReply)
{ }
CallReply::CallReply(RedisModuleCallReply *reply)
	: _reply(reply, [](RedisModuleCallReply*){}) {}

int CallReply::Type() {
	return RedisModule_CallReplyType(_reply.get());
}
size_t CallReply::Length() {
	return RedisModule_CallReplyLength(_reply.get());
}
long long CallReply::Integer() noexcept {
	return RedisModule_CallReplyInteger(_reply.get());
}
double CallReply::Double() noexcept {
	return RedisModule_CallReplyDouble(_reply.get());
}
const char *CallReply::BigNumber(size_t& len) {
	return RedisModule_CallReplyBigNumber(_reply.get(), &len);
}
const char *CallReply::Verbatim(size_t& len, const char **format) {
	return RedisModule_CallReplyVerbatim(_reply.get(), &len, format);
}
bool CallReply::Bool() noexcept {
	return RedisModule_CallReplyBool(_reply.get());
}
const char *CallReply::StringPtr(size_t &len) {
	return RedisModule_CallReplyStringPtr(_reply.get(), &len);
}
String CallReply::CreateString() {
	return RedisModule_CreateStringFromCallReply(_reply.get());
}

CallReply CallReply::ArrayElement(size_t idx) {
	return RedisModule_CallReplyArrayElement(_reply.get(), idx);
}
CallReply CallReply::SetElement(size_t idx) {
	return RedisModule_CallReplySetElement(_reply.get(), idx);
}
CallReply CallReply::Attribute() {
	return RedisModule_CallReplyAttribute(_reply.get());
}

const char *CallReply::Protocol(size_t &len) {
	return RedisModule_CallReplyProto(_reply.get(), &len);
}
CallReply::KVP CallReply::MapElement(size_t idx) {
	RedisModuleCallReply *key, *val;
	if (RedisModule_CallReplyMapElement(_reply.get(), idx, &key, &val) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return KVP(key, val);
}
CallReply::KVP CallReply::AttributeElement(size_t idx) {
	RedisModuleCallReply *key, *val;
	if (RedisModule_CallReplyAttributeElement(_reply.get(), idx, &key, &val) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	return KVP(key, val);
}

RedisModuleCallReply *CallReply::Unwrap() noexcept {
	return _reply.get();
}
CallReply::operator RedisModuleCallReply *() noexcept { return _reply.get(); }

CallReply::KVP::KVP(RedisModuleCallReply *key, RedisModuleCallReply *val)
	: _kvp(std::make_pair(key, val))
{ }
CallReply CallReply::KVP::Key() {
	return CallReply(_kvp.first);
}
CallReply CallReply::KVP::Val() {
	return CallReply(_kvp.second);
}

//---------------------------------------------------------------------------------------------

User::User(const char *name)
	: _user(RedisModule_CreateModuleUser(name), RedisModule_FreeModuleUser) {}
User::User(String& name)
	: _user(RedisModule_GetModuleUserFromUserName(name), RedisModule_FreeModuleUser) {}


int User::SetACL(const char* acl) {
	return RedisModule_SetModuleUserACL(_user.get(), acl);
}
int User::ACLCheckCommandPermissions(Args& args) {
	return RedisModule_ACLCheckCommandPermissions(_user.get(), args, args.Size());
}
int User::ACLCheckKeyPermissions(String& key, int flags) {
	return RedisModule_ACLCheckKeyPermissions(_user.get(), key, flags);
}
int User::ACLCheckChannelPermissions(String& ch, int flags) {
	return RedisModule_ACLCheckChannelPermissions(_user.get(), ch, flags);
}
void User::ACLAddLogEntry(Context ctx, String& object, RedisModuleACLLogEntryReason reason) {
	return RedisModule_ACLAddLogEntry(ctx, _user.get(), object, reason);
}

String User::GetCurrentUserName(Context ctx) {
	return RedisModule_GetCurrentUserName(ctx);
}
int User::RedactClientCommandArgument(Context ctx, int pos) {
	return RedisModule_RedactClientCommandArgument(ctx, pos);
}

RedisModuleUser *User::Unwrap() noexcept {
	return _user.get();
}
User::operator RedisModuleUser *() noexcept { return _user.get(); }

//---------------------------------------------------------------------------------------------

Dict::Dict() : _dict(RedisModule_CreateDict(NULL), Deleter) {}
void Dict::Deleter(RedisModuleDict *dict) noexcept {
	RedisModule_FreeDict(NULL, dict);
}

uint64_t Dict::Size() {
	return RedisModule_DictSize(_dict.get());
}

int Dict::Set(void *key, size_t keylen, void *ptr) {
	return RedisModule_DictSetC(_dict.get(), key, keylen, ptr);
}
int Dict::Set(String& key, void *ptr) {
	return RedisModule_DictSet(_dict.get(), key, ptr);
}
int Dict::Replace(void *key, size_t keylen, void *ptr) {
	return RedisModule_DictReplaceC(_dict.get(), key, keylen, ptr);
}
int Dict::Replace(String& key, void *ptr) {
	return RedisModule_DictReplace(_dict.get(), key, ptr);
}
void *Dict::Get(void *key, size_t keylen, int *nokey) {
	return RedisModule_DictGetC(_dict.get(), key, keylen, nokey);
}
void *Dict::Get(String& key, int *nokey) {
	return RedisModule_DictGet(_dict.get(), key, nokey);
}
int Dict::Del(void *key, size_t keylen, void *oldval) {
	return RedisModule_DictDelC(_dict.get(), key, keylen, oldval);
}
int Dict::Del(String& key, void *oldval) {
	return RedisModule_DictDel(_dict.get(), key, oldval);
}
Dict::Iter Dict::Start(const char *op, void *key, size_t keylen) {
	return RedisModule_DictIteratorStartC(_dict.get(), op, key, keylen);
}
Dict::Iter Dict::Start(const char *op, String& key) {
	return RedisModule_DictIteratorStart(_dict.get(), op, key);
}

RedisModuleDict *Dict::Unwrap() noexcept {
	return _dict.get();
}
Dict::operator RedisModuleDict *() { return _dict.get(); }

Dict::Iter::Iter(RedisModuleDictIter *iter)
	: _iter(iter, RedisModule_DictIteratorStop)
{}

int Dict::Iter::Reseek(const char *op, void *key, size_t keylen) {
	return RedisModule_DictIteratorReseekC(_iter.get(), op, key, keylen);
}
int Dict::Iter::Reseek(const char *op, String& key) {
	return RedisModule_DictIteratorReseek(_iter.get(), op, key);
}
void *Dict::Iter::Next(size_t *keylen, void **dataptr) {
	return RedisModule_DictNextC(_iter.get(), keylen, dataptr);
}
void *Dict::Iter::Prev(size_t *keylen, void **dataptr) {
	return RedisModule_DictPrevC(_iter.get(), keylen, dataptr);
}
String Dict::Iter::Next(void **dataptr) {
	return RedisModule_DictNext(NULL, _iter.get(), dataptr);
}
String Dict::Iter::Prev(void **dataptr) {
	return RedisModule_DictPrev(NULL, _iter.get(), dataptr);
}
int Dict::Iter::Compare(const char *op, void *key, size_t keylen) {
	return RedisModule_DictCompareC(_iter.get(), op, key, keylen);
}
int Dict::Iter::Compare(const char *op, String& key) {
	return RedisModule_DictCompare(_iter.get(), op, key);
}

RedisModuleDictIter *Dict::Iter::Unwrap() noexcept {
	return _iter.get();
}
Dict::Iter::operator RedisModuleDictIter *() { return _iter.get(); }


//---------------------------------------------------------------------------------------------

ServerInfo::ServerInfo(const char *section)
	: _info(RedisModule_GetServerInfo(NULL, section), &Deleter)
{}
void ServerInfo::Deleter(RedisModuleServerInfoData *info) noexcept {
	RedisModule_FreeServerInfo(NULL, info);
}
void ServerInfo::GetField(const char* field, String& str) {
	str = RedisModule_ServerInfoGetField(NULL, _info.get(), field);
}
void ServerInfo::GetField(const char* field, const char **str) {
	*str = RedisModule_ServerInfoGetFieldC(_info.get(), field);
}
int ServerInfo::GetField(const char* field, long long& ll) {
	int out_err;
	ll = RedisModule_ServerInfoGetFieldSigned(_info.get(), field, &out_err);
	return out_err;
}
int ServerInfo::GetField(const char* field, unsigned long long& ull) {
	int out_err;
	ull = RedisModule_ServerInfoGetFieldUnsigned(_info.get(), field, &out_err);
	return out_err;
}
int ServerInfo::GetField(const char* field, double& d) {
	int out_err;
	d = RedisModule_ServerInfoGetFieldDouble(_info.get(), field, &out_err);
	return out_err;
}

RedisModuleServerInfoData *ServerInfo::Unwrap() noexcept {
	return _info.get();
}
ServerInfo::operator RedisModuleServerInfoData *() { return _info.get(); }

//---------------------------------------------------------------------------------------------

Args::Args(int argc, RedisModuleString **argv)
	: _argc(argc), _argv(argv)
{}

int Args::Size() {
	return _argc;
}
Args::operator RedisModuleString **() {
	return _argv;
}

String Args::operator[](int idx) {
	return _argv[idx];
}

//---------------------------------------------------------------------------------------------

template <class T>
Cmd<T>::Cmd(Context ctx, const Args& args) : _ctx(ctx), _args(args) {}

template <class T>
int Cmd<T>::cmdfunc(Context ctx, RedisModuleString **argv, int argc) {
	try {
		Args args(argc, argv);
		T cmd(ctx, args);
		return cmd();
	} catch(...) {
		return REDISMODULE_ERR;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
