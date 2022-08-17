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
	RedisModule_Log(ctx, level, fmt, vargs...);
}
template<typename... Vargs>
void Log::LogIOError(IO io, const char *levelstr, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_LogIOError(io, levelstr, fmt, vargs...);
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
BlockedClient::~BlockedClient() noexcept {
	try { AbortBlock(); }
	catch (...) { } // you had your chance to catch it yourself
}

void BlockedClient::UnblockClient(void *privdata) {
	if (RedisModule_UnblockClient(_bc, privdata) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
void BlockedClient::AbortBlock() {
	if (RedisModule_AbortBlock(_bc) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}

RedisModuleBlockedClient *BlockedClient::Unwrap() noexcept {
	return _bc;
}
BlockedClient::operator RedisModuleBlockedClient *() { return _bc; }

//---------------------------------------------------------------------------------------------

ThreadSafeContext::ThreadSafeContext(BlockedClient& bc) 
	: _ctx(RedisModule_GetThreadSafeContext(bc))
{ }
ThreadSafeContext::ThreadSafeContext(Context ctx) 
	: _ctx(RedisModule_GetDetachedThreadSafeContext(ctx))
{ }
ThreadSafeContext::~ThreadSafeContext() {
	RedisModule_FreeThreadSafeContext(_ctx);
}

void ThreadSafeContext::Lock() noexcept {
	RedisModule_ThreadSafeContextLock(_ctx);
}
int ThreadSafeContext::TryLock() noexcept {
	return RedisModule_ThreadSafeContextTryLock(_ctx);
}
void ThreadSafeContext::Unlock() noexcept {
	RedisModule_ThreadSafeContextUnlock(_ctx);
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
String::String(const RedisModuleString *str)
	: _str(RedisModule_CreateStringFromString(NULL, str))
{ }
String::String(const String& other) : String(other._str) {
	Retain();
}
String::String(String&& other) : _str(std::move(other._str))
{ }
String& String::operator=(String other) {
	swap(*this, other);
	return *this;
}

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

void swap(String& s1, String& s2) noexcept {
	std::swap(s1._str, s2._str);
}

//---------------------------------------------------------------------------------------------

Key::Key(Context ctx, String keyname, int mode) // OpenKey
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

RedisModuleKey *Key::Unwrap() noexcept {
	return _key;
}
Key::operator RedisModuleKey *() noexcept { return _key; }
Key::operator const RedisModuleKey *() const noexcept { return _key; }

//---------------------------------------------------------------------------------------------

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

template<typename... Vargs>
int Hash::Set(int flags, Vargs... vargs) {
	return RedisModule_HashSet(_key, flags, vargs...);
}

template<typename... Vargs>
int Hash::Get(int flags, Vargs... vargs) {
	return RedisModule_HashGet(_key, flags, vargs...);
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
	RedisModule_EmitAOF(io, cmdname, fmt, vargs...);
}

//---------------------------------------------------------------------------------------------

template<typename... Vargs>
CallReply::CallReply(Context ctx, const char *cmdname, const char *fmt, Vargs... vargs) 
	: _reply(RedisModule_Call(ctx, cmdname, fmt, RedisModule::Unwrap(vargs)...))
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
	return RedisModule_CallReplyMapElement(_reply, idx, &(key._reply), &(val._reply));
}
int CallReply::AttributeElement(size_t idx, CallReply& key, CallReply& val) {
	return RedisModule_CallReplyAttributeElement(_reply, idx, &(key._reply), &(val._reply));
}
CallReply CallReply::Attribute() {
	return RedisModule_CallReplyAttribute(_reply);
}

const char *CallReply::Protocol(size_t &len) {
	return RedisModule_CallReplyProto(_reply, &len);
}

RedisModuleCallReply *CallReply::Unwrap() noexcept {
	return _reply;
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
int User::ACLCheckCommandPermissions(Args& args) {
	return RedisModule_ACLCheckCommandPermissions(_user, args, args.Size());
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

RedisModuleUser *User::Unwrap() noexcept {
	return _user;
}
User::operator RedisModuleUser *() noexcept { return _user; }

//---------------------------------------------------------------------------------------------

Dict::Dict() : _dict(RedisModule_CreateDict(NULL)) {}
Dict::~Dict() noexcept {
	RedisModule_FreeDict(NULL, _dict);
}

uint64_t Dict::Size() {
	return RedisModule_DictSize(_dict);
}

int Dict::Set(void *key, size_t keylen, void *ptr) {
	return RedisModule_DictSetC(_dict, key, keylen, ptr);
}
int Dict::Set(String& key, void *ptr) {
	return RedisModule_DictSet(_dict, key, ptr);
}
int Dict::Replace(void *key, size_t keylen, void *ptr) {
	return RedisModule_DictReplaceC(_dict, key, keylen, ptr);
}
int Dict::Replace(String& key, void *ptr) {
	return RedisModule_DictReplace(_dict, key, ptr);
}
void *Dict::Get(void *key, size_t keylen, int *nokey) {
	return RedisModule_DictGetC(_dict, key, keylen, nokey);
}
void *Dict::Get(String& key, int *nokey) {
	return RedisModule_DictGet(_dict, key, nokey);
}
int Dict::Del(void *key, size_t keylen, void *oldval) {
	return RedisModule_DictDelC(_dict, key, keylen, oldval);
}
int Dict::Del(String& key, void *oldval) {
	return RedisModule_DictDel(_dict, key, oldval);
}
Dict::Iter Dict::Start(const char *op, void *key, size_t keylen) {
	return RedisModule_DictIteratorStartC(_dict, op, key, keylen);
}
Dict::Iter Dict::Start(const char *op, String& key) {
	return RedisModule_DictIteratorStart(_dict, op, key);
}

RedisModuleDict *Dict::Unwrap() noexcept {
	return _dict;
}
Dict::operator RedisModuleDict *() { return _dict; }

Dict::Iter::Iter(RedisModuleDictIter *iter) : _iter(iter) {}
Dict::Iter::~Iter() noexcept {
	RedisModule_DictIteratorStop(_iter);
}

int Dict::Iter::Reseek(const char *op, void *key, size_t keylen) {
	return RedisModule_DictIteratorReseekC(_iter, op, key, keylen);
}
int Dict::Iter::Reseek(const char *op, String& key) {
	return RedisModule_DictIteratorReseek(_iter, op, key);
}
void *Dict::Iter::Next(size_t *keylen, void **dataptr) {
	return RedisModule_DictNextC(_iter, keylen, dataptr);
}
void *Dict::Iter::Prev(size_t *keylen, void **dataptr) {
	return RedisModule_DictPrevC(_iter, keylen, dataptr);
}
String Dict::Iter::Next(void **dataptr) {
	return RedisModule_DictNext(NULL, _iter, dataptr);
}
String Dict::Iter::Prev(void **dataptr) {
	return RedisModule_DictPrev(NULL, _iter, dataptr);
}
int Dict::Iter::Compare(const char *op, void *key, size_t keylen) {
	return RedisModule_DictCompareC(_iter, op, key, keylen);
}
int Dict::Iter::Compare(const char *op, String& key) {
	return RedisModule_DictCompare(_iter, op, key);
}

RedisModuleDictIter *Dict::Iter::Unwrap() noexcept {
	return _iter;
}
Dict::Iter::operator RedisModuleDictIter *() { return _iter; }


//---------------------------------------------------------------------------------------------

ServerInfo::ServerInfo(const char *section)
	: _info(RedisModule_GetServerInfo(NULL, section))
{}
ServerInfo::~ServerInfo() {
	RedisModule_FreeServerInfo(NULL, _info);
}
void ServerInfo::GetField(const char* field, String& str) {
	str = RedisModule_ServerInfoGetField(NULL, _info, field);
}
void ServerInfo::GetField(const char* field, const char **str) {
	*str = RedisModule_ServerInfoGetFieldC(_info, field);
}
int ServerInfo::GetField(const char* field, long long& ll) {
	int out_err;
	ll = RedisModule_ServerInfoGetFieldSigned(_info, field, &out_err);
	return out_err;
}
int ServerInfo::GetField(const char* field, unsigned long long& ull) {
	int out_err;
	ull = RedisModule_ServerInfoGetFieldUnsigned(_info, field, &out_err);
	return out_err;
}
int ServerInfo::GetField(const char* field, double& d) {
	int out_err;
	d = RedisModule_ServerInfoGetFieldDouble(_info, field, &out_err);
	return out_err;
}

RedisModuleServerInfoData *ServerInfo::Unwrap() noexcept {
	return _info;
}
ServerInfo::operator RedisModuleServerInfoData *() { return _info; }

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
