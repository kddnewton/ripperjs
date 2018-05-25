#include <node.h>
#include <ruby.h>

namespace ripperjs {
  using v8::Isolate;
  using v8::Local;

  using v8::Array;
  using v8::Exception;
  using v8::False;
  using v8::FunctionCallbackInfo;
  using v8::Integer;
  using v8::Null;
  using v8::Object;
  using v8::String;
  using v8::Value;

  VALUE rb_cRipperJS;
  ID rb_sexp;

  Local<Value> translate(Isolate *isolate, VALUE value) {
    switch (TYPE(value)) {
      case T_SYMBOL:
        return String::NewFromUtf8(isolate, rb_id2name(SYM2ID(value)));
      case T_FALSE:
        return False(isolate);
      case T_NIL:
        return Null(isolate);
      case T_FIXNUM:
        return Integer::New(isolate, FIX2LONG(value));
      case T_STRING:
        return String::NewFromUtf8(isolate, StringValueCStr(value));
      case T_ARRAY: {
        long size = RARRAY_LEN(value);
        Local<Array> array = Array::New(isolate, size);

        long idx;
        for (idx = 0; idx < size; idx++) {
          array->Set(idx, translate(isolate, rb_ary_entry(value, idx)));
        }

        return array;
      }
      case T_HASH: {
        Local<Object> object = Object::New(isolate);

        VALUE keys = rb_funcall(value, rb_intern("keys"), 0);
        VALUE key;

        long idx;
        long size = RARRAY_LEN(keys);

        for (idx = 0; idx < size; idx++) {
          key = rb_ary_entry(keys, idx);
          object->Set(
            translate(isolate, key),
            translate(isolate, rb_hash_aref(value, key))
          );
        }

        return object;
      }
    }
  }

  void throwException(Isolate *isolate, const char *message) {
    isolate->ThrowException(
      Exception::TypeError(String::NewFromUtf8(isolate, message))
    );
  }

  void sexp(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 1) {
      return throwException(isolate, "Wrong number of arguments");
    }

    if (!args[0]->IsString()) {
      return throwException(isolate, "Code must be a string");
    }

    String::Utf8Value code(isolate, args[0]->ToString());
    VALUE root = rb_funcall(rb_cRipperJS, rb_sexp, 1, rb_str_new2(*code));

    if (root == Qnil) {
      return throwException(isolate, "Invalid Ruby code");
    }

    args.GetReturnValue().Set(translate(isolate, root));
  }

  void init(Local<Object> exports) {
    ruby_init();
    ruby_init_loadpath();

    char *filename = realpath("./src/ripperjs.rb", NULL);
    rb_require(filename);

    rb_cRipperJS = rb_const_get(rb_cObject, rb_intern("RipperJS"));
    rb_sexp = rb_intern("sexp");

    NODE_SET_METHOD(exports, "sexp", sexp);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, init)
}
