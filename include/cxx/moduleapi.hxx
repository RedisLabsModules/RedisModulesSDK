#pragma once

#include "moduleapi.h"

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
	void *PoolAlloc(size_t bytes) {
		return RedisModule_PoolAlloc(_ctx, bytes);
	}
} // namespace Alloc

namespace Time {
	long long Milliseconds() {
		return RedisModule_Milliseconds();
	}
}

namespace AutoMemory {
	void AutoMemory() {
		RedisModule_AutoMemory(_ctx);
	}
}

struct Module {
	
};

///////////////////////////////////////////////////////////////////////////////////////////////

int Context::IsKeysPositionRequest() {
	RedisModule_IsKeysPositionRequest(_ctx);
}
void Context::KeyAtPos(int pos) {
	RedisModule_KeyAtPos(_ctx, pos);
}
int Context::CreateCommand(const char *name, RedisModuleCmdFunc cmdfunc,
	const char *strflags, int firstkey, int lastkey, int keystep) {
	return RedisModule_CreateCommand(_ctx, name, cmdfunc, strflags, firstkey, lastkey, keystep);
}

Context::operator RedisModuleCtx *() { return _ctx; }
Context::operator const RedisModuleCtx *() const { return _ctx; }

//---------------------------------------------------------------------------------------------

ThreadSafeContext::ThreadSafeContext(BlockedClient bc) {
	_ctx = RedisModule_GetThreadSafeContext(bc);
}
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

RMType::RMType(const char *name, int encver, RedisModuleTypeMethods *typemethods)
	: _type(RedisModule_CreateDataType(_ctx, name, encver, typemethods))
{ }
RMType::RMType(RedisModuleType *type) : _type(type) {}

RMType::operator RedisModuleType *() { return _type; }
RMType::operator const RedisModuleType *() const { return _type; }

//---------------------------------------------------------------------------------------------

String::String(const char *ptr, size_t len)
	: _str(RedisModule_CreateString(_ctx, ptr, len))
{ }

String::String(long long ll)
	: _str(RedisModule_CreateStringFromLongLong(_ctx, ll))
{ }

String::String(const RedisModuleString *str)
	: _str(RedisModule_CreateStringFromString(_ctx, str))
{ }

void String::Retain() {
	RedisModule_RetainString(_ctx, _str);
}

String::~String() {
	RedisModule_FreeString(_ctx, _str);
}

const char *String::PtrLen(size_t &len) const {
	return RedisModule_StringPtrLen(_str, &len);
}

int String::ToLongLong(long long &ll) const {
	return RedisModule_StringToLongLong(_str, &ll);
}
int String::ToDouble(double &d) const {
	return RedisModule_StringToDouble(_str, &d);
}
int String::ToLongDouble(long double &ld) const {
	return RedisModule_StringToLongDouble(_str, &ld);
}

int String::AppendBuffer(const char *buf, size_t len) {
	return RedisModule_StringAppendBuffer(_ctx, _str, buf, len);
}

String::operator RedisModuleString *() { return _str; }
String::operator const RedisModuleString *() const { return _str; }

int StringCompare(String s1, String s2) {
	return RedisModule_StringCompare(s1, s2);
}

//---------------------------------------------------------------------------------------------

Key::Key(String keyname, int mode) // OpenKey
	: _key((RedisModuleKey *)RedisModule_OpenKey(_ctx, keyname, mode))
{ }
Key::Key(RedisModuleKey *key) : _key(key) { }

Key::~Key() { // CloseKey
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

StringKey::StringKey(String keyname, int mode) : Key(keyname, mode) {}

int StringKey::Set(String str) {
	return RedisModule_StringSet(_key, str);
}
char *StringKey::DMA(size_t &len, int mode) {
	return RedisModule_StringDMA(_key, &len, mode);
}
int StringKey::Truncate(size_t newlen) {
	return RedisModule_StringTruncate(_key, newlen);
}

//---------------------------------------------------------------------------------------------

List::List(String keyname, int mode) : Key(keyname, mode) {}

int List::Push(int where, String ele) {
	return RedisModule_ListPush(_key, where, ele);
}
RedisModuleString *List::Pop(int where) {
	return RedisModule_ListPop(_key, where);
}

//---------------------------------------------------------------------------------------------

Zset::Zset(String keyname, int mode) : Key(keyname, mode) {}

int Zset::Add(double score, String ele, int *flagsptr) {
	return RedisModule_ZsetAdd(_key, score, ele, flagsptr);
}
int Zset::Incrby(double score, String ele, int *flagsptr, double *newscore) {
	return RedisModule_ZsetIncrby(_key, score, ele, flagsptr, newscore);
}
int Zset::Rem(String ele, int *deleted) {
	return RedisModule_ZsetRem(_key, ele, deleted);
}
int Zset::Score(String ele, double *score) {
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
int Zset::FirstInLexRange(String min, String max) {
	return RedisModule_ZsetFirstInLexRange(_key, min, max);
}
int Zset::LastInLexRange(String min, String max) {
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

Hash::Hash(String keyname, int mode) : Key(keyname, mode) {}

int Hash::Set(int flags, ...);
int Hash::Get(int flags, ...);

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

void IO::SaveString(String s) {
	RedisModule_SaveString(_io, s);
}
String IO::LoadString() {
	return RedisModule_LoadString(_io);
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

void IO::EmitAOF(const char *cmdname, const char *fmt, ...) {
	RedisModule_EmitAOF(_io, cmdname, fmt, ...);
}

void IO::LogIOError(const char *levelstr, const char *fmt, ...) {
	RedisModule_LogIOError(_io, levelstr, fmt, ...);
}

//---------------------------------------------------------------------------------------------

CallReply::CallReply(const char *cmdname, const char *fmt, ...) 
	: _reply(RedisModule_Call(_ctx, cmdname, fmt, ...))
{ }
CallReply::~CallReply() {
	RedisModule_FreeCallReply(_reply);
}
const char *CallReply::StringPtr(size_t &len) {
	return RedisModule_CallReplyStringPtr(_reply, &len);
}
String CallReply::CreateString() {
	return String(RedisModule_CreateStringFromCallReply(_reply));
}

const char *CallReply::Proto(size_t *len);
int CallReply::Type();
long long CallReply::Integer();
size_t CallReply::Length();
CallReply CallReply::ArrayElement(size_t idx);

//---------------------------------------------------------------------------------------------

template <class T>
Command<T>::Command() {}
template <class T>
int Command<T>::Run(const Args &args) { return REDISMODULE_OK; }

template <class T>
int Command<T>::cmdfunc(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
	Args args(argv, argc);
	T cmd{ctx}
	return cmd.Run(args);
}

///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace RedisModule
