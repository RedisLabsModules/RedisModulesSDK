#pragma once

#include <utility> // std::swap
#include "moduleapi.h"

namespace Redis {

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

int Context::WrongArity() {
	return RedisModule_WrongArity(_ctx);
}
int Context::ReplyWithError(const char *err) {
	return RedisModule_ReplyWithError(_ctx, err);
}
int Context::ReplyWithNull() {
	return RedisModule_ReplyWithNull(_ctx);
}
int Context::ReplyWithArray(long len) {
	return RedisModule_ReplyWithArray(_ctx, len);
}
void Context::ReplySetArrayLength(long len) {
	RedisModule_ReplySetArrayLength(_ctx, len);
}
int Context::ReplyWithMap(long len) {
	return RedisModule_ReplyWithMap(_ctx, len);
}
void Context::ReplySetMapLength(long len) {
	RedisModule_ReplySetMapLength(_ctx, len);
}
int Context::ReplyWithSet(long len) {
	return RedisModule_ReplyWithSet(_ctx, len);
}
void Context::ReplySetSetLength(long len) {
	RedisModule_ReplySetSetLength(_ctx, len);
}
int Context::ReplyWithAttribute(long len) {
	return RedisModule_ReplyWithAttribute(_ctx, len);
}
void Context::ReplySetAttributeLength(long len) {
	RedisModule_ReplySetAttributeLength(_ctx, len);
}
int Context::ReplyWithBool(bool b) {
	return RedisModule_ReplyWithBool(_ctx, b);
}
int Context::ReplyWithLongLong(long long ll) {
	return RedisModule_ReplyWithLongLong(_ctx, ll);
}
int Context::ReplyWithDouble(double d) {
	return RedisModule_ReplyWithDouble(_ctx, d);
}
int Context::ReplyWithBigNumber(const char *bignum, size_t len) {
	return RedisModule_ReplyWithBigNumber(_ctx, bignum, len);
}
int Context::ReplyWithSimpleString(const char *msg) {
	return RedisModule_ReplyWithSimpleString(_ctx, msg);
}
int Context::ReplyWithStringBuffer(const char *buf, size_t len) {
	return RedisModule_ReplyWithStringBuffer(_ctx, buf, len);
}
int Context::ReplyWithVerbatimStringType(const char *buf, size_t len, const char *ext) {
	return RedisModule_ReplyWithVerbatimStringType(_ctx, buf, len, ext);
}
int Context::ReplyWithString(Redis::String& str) {
	return RedisModule_ReplyWithString(_ctx, str);
}
int Context::ReplyWithCallReply(Redis::CallReply& reply) {
	return RedisModule_ReplyWithCallReply(_ctx, reply);
}

///////////////////////////////////////////////////////////////////////////////////////////////

template<CmdFunc cmdfunc>
int callback_wrapper(RedisModuleCtx* ctx, RedisModuleString** argv, int argc) {
	return cmdfunc(ctx, Args(argc, argv));
}

template<CmdFunc cmdfunc>
int Command::Create(Context ctx, const char *name,
	const char *strflags, int firstkey, int lastkey, int keystep) {
	return RedisModule_CreateCommand(ctx, name, &callback_wrapper<cmdfunc>,
		strflags, firstkey, lastkey, keystep);
}
RedisModuleCommand *Command::Get(Context ctx, const char *name) {
	return RedisModule_GetCommand(ctx, name);
}
int Command::CreateSubCommand(RedisModuleCommand *parent, const char *name, RedisModuleCmdFunc cmdfunc,
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

Context::Context(RedisModuleCtx *ctx) : _ctx(ctx) {}

// No AutoMemory. To be deprecated.
// void Context::AutoMemory(Context ctx) noexcept {
// 	RedisModule_AutoMemory(ctx);
// }

template<typename... Vargs>
CallReply Context::Call(const char *cmdname, const char *fmt, Vargs... vargs) {
	return CallReply(RedisModule_Call(_ctx, cmdname, fmt, Redis::Unwrap(vargs)...), true);
}

bool Context::IsKeysPositionRequest() noexcept {
	return RedisModule_IsKeysPositionRequest(_ctx);
}
void Context::KeyAtPos(int pos) noexcept {
	RedisModule_KeyAtPos(_ctx, pos);
}
void Context::KeyAtPosWithFlags(int pos, int flags) {
	RedisModule_KeyAtPosWithFlags(_ctx, pos, flags);
}
bool Context::KeyExists(const String& keyname) {
	return RedisModule_KeyExists(_ctx, const_cast<String&>(keyname));
}
int *Context::GetCommandKeysWithFlags(Args& args, int *num_keys, int **out_flags) {
	return RedisModule_GetCommandKeysWithFlags(_ctx, args, args.Size(), num_keys, out_flags);
}

int Context::GetSelectedDb() noexcept {
	return RedisModule_GetSelectedDb(_ctx);
}
void Context::SelectDb(int newid) {
	if (RedisModule_SelectDb(_ctx, newid) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
unsigned long long Context::GetClientId() noexcept {
	return RedisModule_GetClientId(_ctx);
}
int Context::SetClientNameById(uint64_t id, String& name) {
	return RedisModule_SetClientNameById(id, name);
}
String Context::GetClientNameById(uint64_t id) {
	return RedisModule_GetClientNameById(_ctx, id);
}

bool Context::IsChannelsPositionRequest() {
	return RedisModule_IsChannelsPositionRequest(_ctx);
}
void Context::ChannelAtPosWithFlags(int pos, int flags) {
	RedisModule_ChannelAtPosWithFlags(_ctx, pos, flags);
}
void Context::Yield(int flags, const char *busy_reply) {
	RedisModule_Yield(_ctx, flags, busy_reply);
}
int Context::PublishMessageShard(String& channel, String& message) {
	return RedisModule_PublishMessageShard(_ctx, channel, message);
}
int Context::SendClusterMessage(const char *target_id, uint8_t type, const char *msg, uint32_t len) {
	return RedisModule_SendClusterMessage(_ctx, target_id, type, msg, len);
}

template<typename... Vargs>
void Context::Replicate(const char *cmdname, const char *fmt, Vargs... vargs) {
	if (RedisModule_Replicate(_ctx, cmdname, fmt, Redis::Unwrap(vargs)...) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
void Context::Replicate() noexcept {
	RedisModule_ReplicateVerbatim(_ctx);
}

BlockedClient Context::BlockClient(RedisModuleCmdFunc reply_callback, RedisModuleCmdFunc timeout_callback,
	void (*free_privdata)(RedisModuleCtx *, void*), long long timeout_ms) {
	return RedisModule_BlockClient(_ctx, reply_callback, timeout_callback, free_privdata, timeout_ms);
}
bool Context::IsBlockedReplyRequest() {
	return RedisModule_IsBlockedReplyRequest(_ctx);
}
bool Context::IsBlockedTimeoutRequest() {
	return RedisModule_IsBlockedTimeoutRequest(_ctx);
}
void *Context::GetBlockedClientPrivateData() {
	return RedisModule_GetBlockedClientPrivateData(_ctx);
}

template<typename... Vargs>
void Context::Log(const char *level, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_Log(_ctx, level, fmt, Redis::Unwrap(vargs)...);
}

RedisModuleCtx *Context::Unwrap() noexcept {
	return _ctx;
}
Context::operator RedisModuleCtx*() noexcept { return _ctx; }

//---------------------------------------------------------------------------------------------

BlockedClient::BlockedClient(RedisModuleBlockedClient* bc)
	: _bc(bc, &Deleter)
{ }
void BlockedClient::Deleter(RedisModuleBlockedClient *bc) noexcept {
	if (bc == nullptr) return;
	RedisModule_AbortBlock(bc); // you had your chance to catch errors yourself
}

void BlockedClient::UnblockClient(void *privdata) {
	if (_bc == nullptr) return;
	if (RedisModule_UnblockClient(_bc.get(), privdata) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
	_bc = nullptr;
}

RedisModuleBlockedClient *BlockedClient::Unwrap() noexcept {
	return _bc.get();
}
BlockedClient::operator RedisModuleBlockedClient *() { return _bc.get(); }

//---------------------------------------------------------------------------------------------

ThreadSafeContext::ThreadSafeContext(BlockedClient bc) 
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
String::String(const std::string& str)
	: String(str.c_str(), str.size())
{ }
String::String(std::string_view& str)
	: String(&str.front(), str.size())
{ }
String::String(long long ll)
	: _str(RedisModule_CreateStringFromLongLong(NULL, ll))
{ }
String::String(unsigned long long ull)
	: _str(RedisModule_CreateStringFromULongLong(NULL, ull))
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

// void String::Append(const char *buf, size_t len) {
// 	if (RedisModule_StringAppendBuffer(NULL, _str, buf, len) != REDISMODULE_OK) {
// 		throw REDISMODULE_ERR;
// 	}
// }
void String::Append(std::string_view& str) {
	if (RedisModule_StringAppendBuffer(NULL, _str, &str.front(), str.size()) != REDISMODULE_OK) {
		throw REDISMODULE_ERR;
	}
}
void String::Trim() noexcept {
	RedisModule_TrimStringAllocation(_str);
}
size_t String::Length() const noexcept {
	size_t len;
	RedisModule_StringPtrLen(_str, &len);
	return len;
}
// const char *String::PtrLen(size_t &len) const noexcept {
// 	return RedisModule_StringPtrLen(_str, &len);
// }
String::operator std::string() const {
	std::size_t len;
	const char *s = RedisModule_StringPtrLen(_str, &len);
	return std::string(s, len);
}
String::operator std::string_view() const {
	std::size_t len;
	const char *s = RedisModule_StringPtrLen(_str, &len);
	return std::string_view(s, len);
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

// int String::Compare(const String& s1, const String& s2) noexcept {
// 	return RedisModule_StringCompare(s1, s2);
// }

auto String::operator==(const String& other) const {
	return 0 == RedisModule_StringCompare(_str, other._str);
}
auto String::operator<=>(const String& other) const {
	return RedisModule_StringCompare(_str, other._str);
}

//---------------------------------------------------------------------------------------------

Key::Key(Context ctx, const String& keyname, int mode) // OpenKey
	: _key(RedisModule_OpenKey(ctx, const_cast<String&>(keyname), mode), RedisModule_CloseKey)
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
	return RedisModule_HashSet(_key.get(), flags, Redis::Unwrap(vargs)...);
}

template<typename... Vargs>
int Hash::Get(int flags, Vargs... vargs) {
	return RedisModule_HashGet(_key.get(), flags, Redis::Unwrap(vargs)...);
}

//---------------------------------------------------------------------------------------------

IO::IO(RedisModuleIO* io) : _io(io) {}

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
	s = RedisModule_LoadString(_io);
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
	RedisModule_EmitAOF(_io, cmdname, fmt, Redis::Unwrap(vargs)...);
}
template<typename... Vargs>
void IO::LogIOError(const char *levelstr, const char *fmt, Vargs... vargs) noexcept {
	RedisModule_LogIOError(_io, levelstr, fmt, Redis::Unwrap(vargs)...);
}

//---------------------------------------------------------------------------------------------

CallReply::CallReply(RedisModuleCallReply *reply, bool owning)
	: _reply(reply, owning ? RedisModule_FreeCallReply : [](RedisModuleCallReply*){}) {}

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

CallReply CallReply::Attribute() {
	return RedisModule_CallReplyAttribute(_reply.get());
}
const char *CallReply::Protocol(size_t &len) {
	return RedisModule_CallReplyProto(_reply.get(), &len);
}

RedisModuleCallReply *CallReply::Unwrap() noexcept {
	return _reply.get();
}
CallReply::operator RedisModuleCallReply *() noexcept { return _reply.get(); }

CallReply::KVP::KVP(RedisModuleCallReply *key, RedisModuleCallReply *val)
	: _kvp(std::make_pair(key, val))
{ }
CallReply CallReply::KVP::Key() {
	return _kvp.first;
}
CallReply CallReply::KVP::Val() {
	return _kvp.second;
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

int Args::Size() const {
	return _argc;
}
Args::operator RedisModuleString **() const {
	return _argv;
}

String Args::operator[](int idx) {
	return _argv[idx];
}

//---------------------------------------------------------------------------------------------

template <class T>
Cmd<T>::Cmd(Context ctx, const Args& args) : _ctx(ctx), _args(args) {}

template <class T>
int Cmd<T>::cmdfunc(Context ctx, const Args& args) {
	try {
		return T(ctx, args)();
	} catch(...) {
		return REDISMODULE_ERR;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
