#include <node.h>
#include <ruby.h>

namespace ripperjs {
  v8::Local<v8::Array> arrayToV8(v8::Isolate *isolate, VALUE sexps) {
    const long sexpsLen = RARRAY_LEN(sexps);
    v8::Local<v8::Array> result = v8::Array::New(isolate, sexpsLen);

    VALUE sexp;
    const char *cstr;

    for (long idx = 0; idx < sexpsLen; idx++) {
      sexp = rb_ary_entry(sexps, idx);

      switch (TYPE(sexp)) {
        case T_SYMBOL:
          cstr = rb_id2name(SYM2ID(sexp));
          result->Set(idx, v8::String::NewFromUtf8(isolate, cstr));
          break;
        case T_FALSE:
          result->Set(idx, v8::False(isolate));
          break;
        case T_NIL:
          result->Set(idx, v8::Null(isolate));
          break;
        case T_FIXNUM:
          result->Set(idx, v8::Integer::New(isolate, FIX2LONG(sexp)));
          break;
        case T_STRING:
          cstr = StringValueCStr(sexp);
          result->Set(idx, v8::String::NewFromUtf8(isolate, cstr));
          break;
        case T_ARRAY:
          result->Set(idx, arrayToV8(isolate, sexp));
          break;
      }
    }

    return result;
  }

  v8::Local<v8::Array> codeToV8(v8::Isolate *isolate, const char *code) {
    ruby_init();
    ruby_init_loadpath();
    rb_require("ripper");

    VALUE rb_cRipper = rb_const_get(rb_cObject, rb_intern("Ripper"));
    VALUE sexps = rb_funcall(
      rb_cRipper, rb_intern("sexp"), 1, rb_str_new2(code)
    );
    return arrayToV8(isolate, sexps);
  }

  void Sexp(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();

    if (args.Length() != 1) {
      v8::Local<v8::String> error = v8::String::NewFromUtf8(
        isolate, "Wrong number of arguments"
      );
      isolate->ThrowException(v8::Exception::TypeError(error));
      return;
    }

    if (!args[0]->IsString()) {
      v8::Local<v8::String> error = v8::String::NewFromUtf8(
        isolate, "Code must be a string"
      );
      isolate->ThrowException(v8::Exception::TypeError(error));
      return;
    }

    v8::String::Utf8Value code(isolate, args[0]->ToString());
    args.GetReturnValue().Set(codeToV8(isolate, *code));
  }

  void init(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "sexp", Sexp);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, init)
}
