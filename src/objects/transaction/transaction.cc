#include <sqlite3.h>
#include <nan.h>
#include "transaction.h"
#include "../database/database.h"

#include "new.cc"
#include "delete-handles.cc"

Transaction::Transaction() : Nan::ObjectWrap(),
	db(NULL),
	handles(NULL),
	config_locked(false),
	bound(false),
	busy(false),
	readonly(true),
	pluck_column(false) {}
Transaction::~Transaction() {
	if (handles && db) {
		db->transs.Remove(this);
	}
	DeleteHandles()(this);
}
void Transaction::Init() {
	Nan::HandleScope scope;
	
	v8::Local<v8::FunctionTemplate> t = Nan::New<v8::FunctionTemplate>(New);
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(Nan::New("Transaction").ToLocalChecked());
	
	constructor.Reset(Nan::GetFunction(t).ToLocalChecked());
}
CONSTRUCTOR(Transaction::constructor);