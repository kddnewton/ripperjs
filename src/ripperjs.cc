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

  VALUE rb_cRipper;
  ID rb_sexp;

  Local<Value> objectToTree(Isolate *isolate, VALUE object);

  Local<Array> arrayToArray(Isolate *isolate, VALUE array, long offset = 0) {
    long size = RARRAY_LEN(array);
    Local<Array> node = Array::New(isolate, size - offset);

    long idx;
    for (idx = offset; idx < size; idx++) {
      node->Set(idx - offset, objectToTree(isolate, rb_ary_entry(array, idx)));
    }

    return node;
  }

  Local<Object> arrayToObject(Isolate *isolate, VALUE array) {
    Local<Object> base = Object::New(isolate);

    base->Set(
      String::NewFromUtf8(isolate, "type"),
      String::NewFromUtf8(isolate, rb_id2name(SYM2ID(rb_ary_entry(array, 0))))
    );

    return base;
  }

  Local<Object> arrayToLiteral(Isolate *isolate, VALUE array) {
    Local<Object> literal = arrayToObject(isolate, array);
    VALUE location = rb_ary_entry(array, 2);

    literal->Set(
      String::NewFromUtf8(isolate, "body"),
      objectToTree(isolate, rb_ary_entry(array, 1))
    );

    literal->Set(
      String::NewFromUtf8(isolate, "lineno"),
      Integer::New(isolate, FIX2LONG(rb_ary_entry(location, 0)))
    );

    literal->Set(
      String::NewFromUtf8(isolate, "colno"),
      Integer::New(isolate, FIX2LONG(rb_ary_entry(location, 1)))
    );

    return literal;
  }

  Local<Object> arrayToNode(Isolate *isolate, VALUE array) {
    Local<Object> node = arrayToObject(isolate, array);

    node->Set(
      String::NewFromUtf8(isolate, "body"),
      arrayToArray(isolate, array, 1)
    );

    return node;
  }

  Local<Value> objectToTree(Isolate *isolate, VALUE object) {
    switch (TYPE(object)) {
      case T_SYMBOL:
        return String::NewFromUtf8(isolate, rb_id2name(SYM2ID(object)));
      case T_FALSE:
        return False(isolate);
      case T_NIL:
        return Null(isolate);
      case T_FIXNUM:
        return Integer::New(isolate, FIX2LONG(object));
      case T_STRING:
        return String::NewFromUtf8(isolate, StringValueCStr(object));
      case T_ARRAY:
        if (RARRAY_LEN(object) == 0) {
          return Array::New(isolate, 0);
        }

        VALUE name = rb_ary_entry(object, 0);

        switch (TYPE(name)) {
          case T_SYMBOL:
            if (rb_id2name(SYM2ID(name))[0] == '@') {
              return arrayToLiteral(isolate, object);
            }
            return arrayToNode(isolate, object);
          case T_ARRAY:
            return arrayToArray(isolate, object);
        }
    }
  }

  void Sexp(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 1) {
      Local<String> error = String::NewFromUtf8(
        isolate, "Wrong number of arguments"
      );
      isolate->ThrowException(Exception::TypeError(error));
      return;
    }

    if (!args[0]->IsString()) {
      Local<String> error = String::NewFromUtf8(
        isolate, "Code must be a string"
      );
      isolate->ThrowException(Exception::TypeError(error));
      return;
    }

    String::Utf8Value code(isolate, args[0]->ToString());
    VALUE array = rb_funcall(rb_cRipper, rb_sexp, 1, rb_str_new2(*code));

    args.GetReturnValue().Set(objectToTree(isolate, array));
  }

  void init(Local<Object> exports) {
    ruby_init();
    ruby_init_loadpath();

    rb_require("ripper");
    rb_cRipper = rb_const_get(rb_cObject, rb_intern("Ripper"));
    rb_sexp = rb_intern("sexp");

    NODE_SET_METHOD(exports, "sexp", Sexp);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, init)
}
